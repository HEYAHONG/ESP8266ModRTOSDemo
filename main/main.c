

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "init.h"
#include "wifi_station.h"
#include "mqttapp.h"

static const char *TAG = "esp8266 main";

void app_main(void)
{

    system_init();

    while(!wifi_station_isconnected())
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG,"waiting for network !!!");
    }

    mqtt_app_start();

}
