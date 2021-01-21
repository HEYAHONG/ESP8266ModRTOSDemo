#ifndef WIFI_STATION_H_INCLUDED
#define WIFI_STATION_H_INCLUDED

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "string.h"

#define WIFI_STATION_SSID "Test"
#define WIFI_STATION_PASSWORD "12345678"

extern EventGroupHandle_t s_wifi_station_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_station_init();

void wifi_station_setconfig(const wifi_config_t *cfg);

const wifi_config_t * wifi_station_getconfig();

void wifi_station_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

#endif // WIFI_STATION_H_INCLUDED
