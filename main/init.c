#include "init.h"
#include "wifi_station.h"
#include "appcmd.h"

#include "sdkconfig.h"

static const char *TAG = "esp8266 init";

//初始化文件系统用于文件存储
static void init_spiffs()
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 100,
      .format_if_mount_failed = true
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

static void  deinit_spiffs()
{
    // All done, unmount partition and disable SPIFFS
    esp_vfs_spiffs_unregister(NULL);
    ESP_LOGI(TAG, "SPIFFS unmounted");
}

//等待按键按下
static void  waitforhit()
{
    int count = 0;
    while(count++ <INIT_MAX_WAIT_HIT)
    {
         ESP_LOGI(TAG, "hit any key to change settings(%d/%d)",count,INIT_MAX_WAIT_HIT);
         vTaskDelay(1000 / portTICK_PERIOD_MS);
         if(getchar_unlocked()>0)
         {
             ESP_LOGI(TAG, "key hit");//按下按键进入配置模式
             enter_app_cmd();
             break;
         }
    }

}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(2,"pool.ntp.org");
    sntp_setservername(1,"ntp.hyhsystem.cn");
    sntp_setservername(0,"ntp.ntsc.ac.cn");
    sntp_init();
}

void system_init()
{
    init_spiffs();

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    tcpip_adapter_init();

    initialize_sntp();
    //设置时区为中国
    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();

    waitforhit();



}

void system_deinit()
{
    deinit_spiffs();
}

