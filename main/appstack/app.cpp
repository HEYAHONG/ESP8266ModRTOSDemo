#include "appconfig.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "app.h"
#include <chrono>


#ifdef CONFIG_MQTT_CLIENT_USE_SMGS
void SMGS_Init();
#endif // CONFIG_MQTT_CLIENT_USE_SMGS

#ifdef CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE
void OneNETDevice_Init();
#endif // CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE

static const char *TAG = "App";

static uint8_t mac[6] = {0};
char macstr[20] = {0};
void app_init()
{
    ESP_LOGI(TAG, "Started!\r\n");

    {
        //获取mac地址用于唯一标识
        bool IsFailed = false;
        if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK)
        {
            ESP_LOGI(TAG, "Get Mac Failed!Try WIFI Init!\r\n");
            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_wifi_init(&cfg));
            if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK)
            {
                IsFailed = true;
                ESP_LOGI(TAG, "Get Mac Failed!\r\n");
                ESP_ERROR_CHECK(esp_wifi_deinit());
            }

        }

        if (!IsFailed)
        {
            for (size_t i = 0; i < sizeof(mac); i++)
            {
                char buff[6] = {0};
                sprintf(buff, "%02X", mac[i]);
                strcat(macstr, buff);
            }

            ESP_LOGI(TAG, "Mac Is %s!\r\n", macstr);

        }
    }

#ifdef CONFIG_MQTT_CLIENT_USE_SMGS
    SMGS_Init();
#endif // CONFIG_MQTT_CLIENT_USE_SMGS

#ifdef CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE
    OneNETDevice_Init();
#endif // CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE

}

/*
保留内存检查
*/
static bool is_reserved_heap_memory_normal()
{
    if(esp_get_free_heap_size()>CONFIG_APPSTACK_RESERVED_HEAP_MEMORY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static std::chrono::system_clock::time_point reserved_heap_memory_normal_tp;
static void check_reserved_heap_memory_normal()
{
    if(is_reserved_heap_memory_normal())
    {
        reserved_heap_memory_normal_tp=std::chrono::system_clock::now();
    }
    else
    {
        if((reserved_heap_memory_normal_tp+std::chrono::seconds(30))<=std::chrono::system_clock::now())
        {
            ESP_LOGI(TAG,"memory is low,it will restart!\n");
            esp_restart();
        }
    }
}

void app_loop()
{
    //检查保留内存
    check_reserved_heap_memory_normal();

    vTaskDelay(100);
}


