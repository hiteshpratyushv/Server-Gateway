# Server Gateway



Install:

   Change your Settings in mqtt_task.c:

	#define MQTT_SERVER "abc.cloudmqtt.com"

	#define MQTT_USER "user"

	#define MQTT_PASS "pass"
	#define MQTT_PORT 12345
	#define MQTT_WEBSOCKET 0  // 0=no 1=yes
   Create the servercertifikate in cert.c
	openssl s_client -showcerts -connect abc.cloudmqtt.com:12345 </dev/null

   make menuconfig
	config your CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD
   make -j8 all
   make flash monitor


Setup Tool chain for compiling

http://esp-idf.readthedocs.io/en/latest/get-started/

Configure Raspberry Pi as MQTT Broker

http://rockingdlabs.dunmire.org/exercises-experiments/ssl-client-certs-to-secure-mqtt

