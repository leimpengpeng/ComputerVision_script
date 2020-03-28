#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include "mqtt.h"

using namespace std;

int main(int argc, char *argv[]) {
    
    MqttWrapper *mqttclient = new MqttWrapper(NULL, "localhost", 1883);
    
    int counter = 0;
    while(true) {
	stringstream str;
	str << counter;
	counter++;
	mqttclient->publish(NULL, "test", str.str().size(), str.str().c_str());
	
	mqttclient->loop();
	
	sleep(1);
    }
    
    return 0;
}