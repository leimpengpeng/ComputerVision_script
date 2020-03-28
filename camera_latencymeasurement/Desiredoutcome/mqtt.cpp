#include "mqtt.h"

MqttWrapper::MqttWrapper(const char *id, const char *host, int port) : mosquittopp(id)
{
	mosqpp::lib_init();			// Initialize libmosquitto
	int keepalive = 0; // seconds
	connect(host, port, keepalive);		// Connect to MQTT Broker
};

void MqttWrapper::on_connect(int rc)
{
	printf("Connected with code %d. \n", rc);

	if (rc == 0)
	{
		subscribe(NULL, "command/IGot");
	}
}

void MqttWrapper::on_message(struct mosquitto_message *message)
{
	
}

void MqttWrapper::on_subcribe(int mid, int qos_count, const int* granted_qos)
{
	printf("Subscription succeeded. \n");
}


