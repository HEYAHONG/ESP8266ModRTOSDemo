#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "lwip/apps/sntp.h"

#define INIT_MAX_WAIT_HIT 4

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void system_init();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INIT_H_INCLUDED
