#include <WiFiProv.h>

#include <esp_crt_bundle.h>
#include <ssl_client.h>
#include <WiFiClientSecure.h>

#include <PubSubClient.h>

#include <MQTT.h>
#include <MQTTClient.h>

#include <ETH.h>

/**********************************************************************
  Filename    : WiFi Server
  Description : Use ESP32's WiFi server feature to wait for other WiFi devices to connect.
                And communicate with them once a connection has been established.
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include <WiFi.h>

#define port 80
const char *ssid_Router      = "*****";  //input your wifi name
const char *password_Router  = "*****";  //input your wifi passwords
WiFiServer  server(port);
int count;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
void callback(char* topic, byte* payload, unsigned int length); 

void setup()
{
    Serial.begin(115200);
    Serial.printf("\nConnecting to ");
    Serial.println(ssid_Router);
    WiFi.disconnect();
    WiFi.begin(ssid_Router, password_Router);
    delay(1000);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());			
    Serial.printf("IP port: %d\n",port);			
    server.begin(port);								
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    count = 0;
    mqttClient.setCallback(callback);
    mqttClient.setServer("broker.mqttdashboard.com", 1883);
    mqttClient.connect("JC3wUTQxFE");
    if (mqttClient.connect("JC3wUTQxFE")) {
    // connection succeeded
    Serial.println("Connection to MQTT client successful");
    }
    else {
    // connection failed
    Serial.println("Connection failed ");
    Serial.println(mqttClient.state());
    }
}

void loop(){
  mqttClient.publish("testtopic/1", "This is a test message");
  mqttClient.endPublish();
  Serial.println("published"); 
  delay(10000);
  mqttClient.subscribe("testtopic/1");
  Serial.println("subscribed");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
