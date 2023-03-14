#include "appconfig.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "app.h"
#include "mqtt.h"
#include "onenettokencpp.h"
#include <inttypes.h>
#include <string>

/*
产品ID
*/
#define ONENET_PRODUCT_ID "616jk7jr6e"
/*
产品访问Key,本代码采用产品Key+自动注册的方式
*/
#define ONENET_PRODUCT_ACCESS_KEY "Pdd3QpBN0FFpXNjPFR4DXn2HQ9utro6uQFvdjuLQMFY="
/*
采用MAC字符串作为设备名称
*/
#define ONENET_PRODUCT_DEVICENAME macstr
/*
OneNET Token版本
*/
#define ONENET_TOKEN_VERSION "2018-10-31"
/*
OneNET Hmac method
*/
#define ONENET_TOKEN_METHOD "sha256"


#ifdef CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE

static const char *TAG = "MQTT_OneNETDevice";

static bool mqttc_event_callback(esp_mqtt_event_id_t event_id, esp_mqtt_event_handle_t event); //返回true表示消息已处理，无需其它处理
static void mqttc_event_on_init_config(esp_mqtt_client_config_t *mqtt_cfg);

//OneNETDevice初始化
void OneNETDevice_Init()
{
    mqttc_start(mqttc_event_on_init_config, mqttc_event_callback);
}

static bool mqttc_event_callback(esp_mqtt_event_id_t event_id, esp_mqtt_event_handle_t event) //返回true表示消息已处理，无需其它处理
{
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
    {
        esp_mqtt_client_subscribe(event->client, (std::string("$sys/")+ONENET_PRODUCT_ID+"/"+ONENET_PRODUCT_DEVICENAME+"/#").c_str(), 0);
        ESP_LOGI(TAG,"OneNET Device is online");
        return true;
    }
    break;
    default:
    {

    }
    break;
    }

    return false;
}

static std::string token;
static void mqttc_event_on_init_config(esp_mqtt_client_config_t *mqtt_cfg)
{
    {
        std::string res=OneNETTokenGetDeviceRes(ONENET_PRODUCT_ID,ONENET_PRODUCT_DEVICENAME);
        std::string sign=OneNETTokenGetSign(0x7FFFFFFF,res,ONENET_PRODUCT_ACCESS_KEY,ONENETCPP_DEFAULT_METHOD,ONENETCPP_DEFAULT_VERSION);
        //计算token
        token=OneNETTokenGenerateURLToken(0x7FFFFFFF,res,sign,ONENETCPP_DEFAULT_METHOD,ONENETCPP_DEFAULT_VERSION);
    }
    //版本必须为3.1.1
    mqtt_cfg->protocol_ver = MQTT_PROTOCOL_V_3_1_1;

    //keepalive
    mqtt_cfg->keepalive = 120;

    //clientid、用户名、密码
    mqtt_cfg->client_id = ONENET_PRODUCT_DEVICENAME;
    mqtt_cfg->username = ONENET_PRODUCT_ID;
    mqtt_cfg->password = token.c_str();

}

#endif // CONFIG_MQTT_CLIENT_USE_ONENET_DEVICE
