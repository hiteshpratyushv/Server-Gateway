/*
 * mqtt_task.c
 *
 *  Created on: 09.03.2017
 *      Author: pcbreflux
 */

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
//#include "freertos/heap_regions.h"

#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
//#include "esp_heap_alloc_caps.h"

#include "nvs_flash.h"

#include "MQTTClient.h"

#define MQTT_TAG "MQTT"

/* Constants that aren't configurable in menuconfig */
#define MQTT_SERVER "192.168.43.112" // "test.mosquitto.org"//"192.168.1.36" //"m20.cloudmqtt.com"
#define MQTT_USER ""
#define MQTT_PASS ""
#define MQTT_PORT 1883
#define MQTT_WEBSOCKET 0  // 0=no 1=yes
#define MQTT_BUF_SIZE 512

static unsigned char mqtt_sendBuf[MQTT_BUF_SIZE];
static unsigned char mqtt_readBuf[MQTT_BUF_SIZE];

extern EventGroupHandle_t wifi_event_group;

const int WIFI_CONNECTED_BIT = BIT0;
const int MQTT_CONNECTED_BIT = BIT1;
const int SUB_START = BIT12;

int ret;
static Network network;
static MQTTClient client;
static MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

void mqtt_connect()
{
	NetworkInit(&network);
	network.websocket = MQTT_WEBSOCKET;

    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
	ESP_LOGI(MQTT_TAG, "Connected to AP");


    ESP_LOGI(MQTT_TAG, "Start MQTT Task ...");

//	ESP_LOGI(MQTT_TAG,"free DRAM %u IRAM %u",esp_get_free_heap_size(),xPortGetFreeHeapSizeTagged(MALLOC_CAP_32BIT));
	ESP_LOGI(MQTT_TAG,"NetworkConnect %s:%d ...",MQTT_SERVER,MQTT_PORT);
	NetworkConnect(&network, MQTT_SERVER, MQTT_PORT);
	ESP_LOGI(MQTT_TAG,"MQTTClientInit  ...");
	MQTTClientInit(&client, &network,
		2000,            // command_timeout_ms
		mqtt_sendBuf,         //sendbuf,
		MQTT_BUF_SIZE, //sendbuf_size,
		mqtt_readBuf,         //readbuf,
		MQTT_BUF_SIZE  //readbuf_size
	);

	char buf[30];
	MQTTString clientId = MQTTString_initializer;
#if defined(MBEDTLS_MQTT_DEBUG)
	sprintf(buf, "ESP32MQTT");
#else
	sprintf(buf, "ESP32MQTT%08X",esp_random());
#endif
	ESP_LOGI(MQTT_TAG,"MQTTClientInit  %s",buf);
	clientId.cstring = buf;

	MQTTString username = MQTTString_initializer;
	username.cstring = MQTT_USER;

	MQTTString password = MQTTString_initializer;
	password.cstring = MQTT_PASS;

	data.clientID          = clientId;
	data.willFlag          = 0;
	data.MQTTVersion       = 4; // 3 = 3.1 4 = 3.1.1
	data.keepAliveInterval = 0;
	data.cleansession      = 1;
	data.username          = username;
	data.password          = password;

	ESP_LOGI(MQTT_TAG,"MQTTConnect  ...");
	ret = MQTTConnect(&client, &data);
	if (ret != SUCCESS) {
		ESP_LOGI(MQTT_TAG, "MQTTConnect not SUCCESS: %d", ret);
	}
	ESP_LOGI(MQTT_TAG,"MQTTConnected");
	xEventGroupSetBits(wifi_event_group, MQTT_CONNECTED_BIT);
	vTaskDelete(NULL);
}

void mqtt_disconnect()
{
	xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
	MQTTDisconnect(&client);
	NetworkDisconnect(&network);
	vTaskDelete(NULL);
	xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
}

void mqtt_publish(void *pvParameters)
{
	char *param=(char *)pvParameters;
	ESP_LOGI(MQTT_TAG, "param: [%s]", param);
	ESP_LOGI(MQTT_TAG, "MQTTPublish  ...");
	MQTTMessage message;

	message.qos = QOS1;
	message.retained = false;
	message.dup = false;
	message.payload = (void*)param;
	message.payloadlen = strlen(param)+1;

	ret = MQTTPublish(&client, "test/topic", &message);
	if (ret != SUCCESS) {
		ESP_LOGI(MQTT_TAG, "MQTTPublish not SUCCESS: %d", ret);
	}
	ESP_LOGI(MQTT_TAG,"MQTTPublish Done");
	vTaskDelete(NULL);
}

static void mqtt_message_handler(MessageData *md) {
	ESP_LOGI(MQTT_TAG, "Topic received!: %.*s %.*s", md->topicName->lenstring.len, md->topicName->lenstring.data, md->message->payloadlen, (char*)md->message->payload);
}

void mqtt_subscribe()
{
   	xEventGroupWaitBits(wifi_event_group, MQTT_CONNECTED_BIT, false, true, portMAX_DELAY);
	ESP_LOGI(MQTT_TAG, "MQTTSubscribe  ...");
	ret = MQTTSubscribe(&client, "test/topic", QOS1, mqtt_message_handler);
	if (ret != SUCCESS) {
		ESP_LOGI(MQTT_TAG, "MQTTSubscribe: %d", ret);
	}
	xEventGroupSetBits(wifi_event_group, SUB_START);
	vTaskDelete(NULL);
}

 void mqtt_unsubscribe()
 {
 	xEventGroupClearBits(wifi_event_group,MQTT_CONNECTED_BIT);
 	xEventGroupClearBits(wifi_event_group,SUB_START);
 	ESP_LOGI(MQTT_TAG, "MQTTUnsubscribe  ...");
		ret = MQTTUnsubscribe(&client, "test/topic");
		if (ret != SUCCESS) {
			ESP_LOGI(MQTT_TAG, "MQTTUnsubscribe: %d", ret);
		}
	xEventGroupSetBits(wifi_event_group,MQTT_CONNECTED_BIT);
	vTaskDelete(NULL);
 }

 void mqtt_yield()
 {
 	while(1) {
 		 	xEventGroupWaitBits(wifi_event_group, SUB_START, false, true, portMAX_DELAY);
 			ESP_LOGI(MQTT_TAG,"MQTTYield ...");
			ret = MQTTYield(&client, (data.keepAliveInterval)*1000);
			if (ret != SUCCESS) {
				ESP_LOGI(MQTT_TAG, "MQTTYield: %d", ret);
			}
		}
	vTaskDelete(NULL);
 }