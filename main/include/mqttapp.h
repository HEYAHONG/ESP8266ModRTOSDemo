#ifndef MQTTAPP_H_INCLUDED
#define MQTTAPP_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
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

#define  MQTT_APP_URI "mqtt://iot.eclipse.org:1883"


void mqtt_app_start();

void mqtt_app_set_uri(char *uri,size_t length);
const char *   mqtt_app_get_uri();

#endif // MQTTAPP_H_INCLUDED
