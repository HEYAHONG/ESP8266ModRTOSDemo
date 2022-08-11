

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "init.h"
#include "wifi_station.h"
#include "wifi_station.h"
#include "sdkconfig.h"
#include "tftpd.h"
#include "mqtt.h"
#include "app.h"
#include "RC.h"

static const char *TAG = "esp8266 main";

static void main_task()
{
    system_init();

    wifi_station_init();

    while(!wifi_station_isconnected())
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG,"waiting for network !!!");
    }

#if CONFIG_LWIP_TFTPD_ON_BOOT == 1
    tftpd_start();
#endif // LWIP_TFTPD_ON_BOOT

#if CONFIG_NETWORK_PROTOCAL_MQTT == 1
#if CONFIG_MQTT_CLIENT_ON_BOOT == 1
    mqttc_start(NULL,NULL);
#endif // CONFIG_MQTT_CLIENT_ON_BOOT
#endif // CONFIG_NETWORK_PROTOCAL_MQTT
 
    app_init();

    while(1)
    {
        app_loop();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(main_task, "main_task", 4096, NULL, 10, NULL);
}
