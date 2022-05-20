/**********************************************************************
  Filename    : WiFi Server
  Description : Use ESP32's WiFi server feature to wait for other WiFi devices to connect.
                And communicate with them once a connection has been established.
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include <WiFi.h>
#define PIN_LED    2
#define PIN_BUTTON 13
#define port 80
const char *ssid_Router      = "****";  //input your wifi name
const char *password_Router  = "****";  //input your wifi passwords
WiFiServer  server(port);
String inputString = "";      //a String to hold incoming data
String stringOne;
String stringTwo;

void setup()
{
    //set up wifi server
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
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());      
    //Serial.printf("IP port: %d\n",port);      
    server.begin(port);               
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    // initialize digital pin PIN_LED as an output.
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println(String("Input '1' to turn the LED ON, input '2' to turn the LED OFF"));
  Serial.println(String("Status of the LED will be printed on the client terminal"));
  stringOne = String("1");
  stringTwo = String("2");
}

void loop(){
 WiFiClient client = server.available();            // listen for incoming clients
  if (client) {                                     // if you get a client,
    Serial.println("Client connected.");
    while (client.connected()) {                    // loop while the client's connected
      if (client.available()) {                     // if there's bytes to read from the client,
        Serial.println(client.readStringUntil('\n')); // print it out the serial monitor
        while(client.read()>0);                     // clear the wifi receive area cache
      }
      if(Serial.available()){                       // read input from serial monitor
        char inChar = Serial.read();         // read one character
        inputString += inChar;
        client.print("Input: ");
        client.print(inputString); // print it out the client.
        delay(1000);
        //client.println("inputString: " + inputString); //display input to client terminal (test)
        //change state of LED based on serial monitor inputs
        //and display state to client terminal
        if (inputString.equals(stringOne)) {
        client.println("LED ON");
        digitalWrite(PIN_LED, HIGH);
        } 
        else if (inputString.equals(stringTwo)) {
        client.println("LED OFF");
        digitalWrite(PIN_LED, LOW);
        }
        else {
        client.println("INVALID INPUT, LED STAYING AS IS");
        }
        inputString = "";
        while(Serial.read()>0);                     // clear the wifi receive area cache
      }
    }
    client.stop();                                  // stop the client connecting.
    Serial.println("Client Disconnected.");
  }
 }
