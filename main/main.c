

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "init.h"

static const char *TAG = "esp8266 main";

void app_main(void)
{

    system_init();

}
