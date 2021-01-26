#include "wifi_station.h"
#include "stdbool.h"
#include "esp_smartconfig.h"
#include "smartconfig_ack.h"
//WIFI SSID 及 Password缓存
static wifi_config_t wifi_config = {0};
static const char *TAG = "wifi station";

static int s_retry_num = 0;
EventGroupHandle_t s_wifi_station_event_group;


static bool IsConnect=false;

bool wifi_station_isconnected()
{
    return IsConnect;
}



static void obtain_time(void)
{
    //initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    ESP_LOGI(TAG,"time now: %s",asctime(localtime(&now)));
}



static void save_wifi_station_config()
{
    FILE* f = fopen("/spiffs/wifistationconfig.bin", "wb");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open wifistationconfig.bin for writing");
        return;
    }

    fwrite(&wifi_config,sizeof(wifi_config),1,f);

    ESP_LOGI(TAG, "save file wifistationconfig.bin");

    fclose(f);
}

static void load_wifi_station_config()
{
    memset(&wifi_config.sta.ssid,0,sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.ssid,WIFI_STATION_SSID,sizeof(WIFI_STATION_SSID));
    memset(&wifi_config.sta.password,0,sizeof(wifi_config.sta.password));
    memcpy(wifi_config.sta.password,WIFI_STATION_PASSWORD,sizeof(WIFI_STATION_PASSWORD));

    //以下均需要spiffs支持
    struct stat st;
    if (stat("/spiffs/wifistationconfig.bin", &st) == 0)
    {
        if(st.st_size != sizeof(wifi_config))
        {
            //文件大小与结构结构体大小不一致
            save_wifi_station_config();
        }
        else
        {
            FILE* f = fopen("/spiffs/wifistationconfig.bin", "rb");
            if (f == NULL)
            {
                ESP_LOGE(TAG, "Failed to open wifistationconfig.bin for read");
                return;
            }

            fread(&wifi_config,sizeof(wifi_config),1,f);

            ESP_LOGI(TAG, "load file wifistationconfig.bin");


            fclose(f);
        }
    }
    else
    {
        //保存文件
        save_wifi_station_config();
    }
}




void wifi_station_setconfig(const wifi_config_t *cfg)
{
    if(cfg!=NULL && cfg!=&wifi_config)
        memcpy(&wifi_config,cfg,sizeof(wifi_config));
    save_wifi_station_config();
}

wifi_config_t * wifi_station_getconfig()
{
    load_wifi_station_config();
    return &wifi_config;
}

static void smartconfig_task(void* parm)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK(esp_smartconfig_set_type(CONFIG_ESP_SMARTCONFIG_TYPE));
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));

    while (1)
    {
        uxBits = xEventGroupWaitBits(s_wifi_station_event_group, WIFI_CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);

        if (uxBits &  WIFI_CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "WiFi Connected to ap");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }

        if (uxBits & ESPTOUCH_DONE_BIT)
        {
            ESP_LOGI(TAG, "smartconfig over");
            esp_smartconfig_stop();
            vTaskDelete(NULL);
        }
    }
}

void wifi_station_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        IsConnect=false;
        if (s_retry_num < 10)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        {
            xTaskCreate(smartconfig_task, "smartconfig_task", 4096, NULL, 3, NULL);//启动smartconfig配置wifi
            xEventGroupSetBits(s_wifi_station_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGI(TAG,"the AP is found!");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_station_event_group, WIFI_CONNECTED_BIT);
        IsConnect=true;
        obtain_time();//同步时间
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        ESP_LOGI(TAG, "Scan done");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        ESP_LOGI(TAG, "Found channel");
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t* evt = (smartconfig_event_got_ssid_pswd_t*)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };
        uint8_t rvd_data[33] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        /*
        wifi_config.sta.bssid_set = evt->bssid_set;

        if (wifi_config.sta.bssid_set == true)
        {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }
        */
        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);
        if (evt->type == SC_TYPE_ESPTOUCH_V2)
        {
            ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
            ESP_LOGI(TAG, "RVD_DATA:%s", rvd_data);
        }

        wifi_station_setconfig(&wifi_config);

        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_connect());
        s_retry_num=0;//清空wifi重置计数
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE)
    {
        xEventGroupSetBits(s_wifi_station_event_group, ESPTOUCH_DONE_BIT);
    }

}


void wifi_station_init()
{
    load_wifi_station_config();

    s_wifi_station_event_group = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_station_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_station_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &wifi_station_event_handler, NULL));






    /* Setting a password implies station will connect to all security modes including WEP/WPA.
        * However these modes are deprecated and not advisable to be used. Incase your Access point
        * doesn't support WPA2, these mode can be enabled by commenting below line */

    if (strlen((char *)wifi_config.sta.password))
    {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

}
