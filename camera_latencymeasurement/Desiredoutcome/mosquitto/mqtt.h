#include <mosquittopp.h>
#include <cstdio>

class MqttWrapper : public mosqpp::mosquittopp
{
public:
	MqttWrapper(const char *id, const char *host, int port);

	void on_connect(int rc);
	void on_message(struct mosquitto_message *message);
	void on_subcribe(int mid, int qos_count, const int *granted_qos);
};


