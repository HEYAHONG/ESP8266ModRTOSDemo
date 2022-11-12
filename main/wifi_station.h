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
#include "lwip/apps/sntp.h"
#include "stdbool.h"
#include "sdkconfig.h"
#ifdef CONFIG_WIFI_STATION_SSID
#define WIFI_STATION_SSID CONFIG_WIFI_STATION_SSID
#else
#define WIFI_STATION_SSID "Test"
#endif
#ifdef  CONFIG_WIFI_STATION_PASSWORD
#define  WIFI_STATION_PASSWORD  CONFIG_WIFI_STATION_PASSWORD
#else
#define WIFI_STATION_PASSWORD "12345678"
#endif

#ifdef CONFIG_WIFI_STATION_SMARTCONIG_TIMEOUT
#define WIFI_STATION_SMARTCONIG_TIMEOUT CONFIG_WIFI_STATION_SMARTCONIG_TIMEOUT
#else
#define WIFI_STATION_SMARTCONIG_TIMEOUT 120
#endif // WIFI_STATION_SMARTCONIG_TIMEOUT

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern EventGroupHandle_t s_wifi_station_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define ESPTOUCH_DONE_BIT  BIT2

void wifi_station_init();

void wifi_station_setconfig(const wifi_config_t *cfg);

wifi_config_t *wifi_station_getconfig();

bool wifi_station_isconnected();

void wifi_station_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // WIFI_STATION_H_INCLUDED
