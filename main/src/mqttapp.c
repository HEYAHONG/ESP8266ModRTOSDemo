#include "mqttapp.h"


static const char *TAG ="MQTTApp";

static esp_mqtt_client_config_t mqtt_cfg={0};
static esp_mqtt_client_handle_t client=0;

static char default_mqtt_uri[64]=MQTT_APP_URI;

static void save_mqtt_app_config()
{
    FILE* f = fopen("/spiffs/mqtturi.bin", "wb");
    if (f == NULL)
    {
        ESP_LOGE(TAG, "Failed to open mqtturi.bin for writing");
        return;
    }

    fwrite(default_mqtt_uri,sizeof(default_mqtt_uri),1,f);

    ESP_LOGI(TAG, "save file mqtturi.bin");

    fclose(f);
}

static void load_mqtt_app_config()
{
     //以下均需要spiffs支持
    struct stat st;
    if (stat("/spiffs/mqtturi.bin", &st) == 0)
    {
        if(st.st_size != sizeof(default_mqtt_uri))
        {
            //文件大小与结构结构体大小不一致
            save_mqtt_app_config();
        }
        else
        {
            FILE* f = fopen("/spiffs/mqtturi.bin", "rb");
            if (f == NULL)
            {
                ESP_LOGE(TAG, "Failed to open mqtturi.bin for read");
                return;
            }

            fread(default_mqtt_uri,sizeof(default_mqtt_uri),1,f);

            ESP_LOGI(TAG, "load file mqtturi.bin");


            fclose(f);
        }
    }
    else
    {
        //保存文件
        save_mqtt_app_config();
    }

    mqtt_cfg.uri=default_mqtt_uri;
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_app_set_uri(char *uri,size_t length)
{
    memset(default_mqtt_uri,0,sizeof(default_mqtt_uri));
    memcpy(default_mqtt_uri,uri,length>sizeof(default_mqtt_uri)?sizeof(default_mqtt_uri):length);
    save_mqtt_app_config();
    if(client!=0)
        esp_mqtt_set_config(client,&mqtt_cfg);
}

const char *  mqtt_app_get_uri()
{
    load_mqtt_app_config();
    return (char *)default_mqtt_uri;
}

void mqtt_app_start(void)
{
    load_mqtt_app_config();
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


