/**********************************************************************
  Filename    : WiFi Server
  Description : Use ESP32's WiFi server feature to wait for other WiFi devices to connect.
                And communicate with them once a connection has been established.
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include <WiFi.h>

#define port 80
const char *ssid_Router      = "****";  //input your wifi name
const char *password_Router  = "****";  //input your wifi passwords
WiFiServer  server(port);
const byte ledPins[] = {15, 2, 4};    //define red, green, blue led pins
const byte chns[] = {0, 1, 2};        //define the pwm channels
int red, green, blue;
uint32_t duty_red, duty_green, duty_blue;
int color_count;
String redHex, greenHex, blueHex, fullHex;

void setup()
{
    Serial.begin(115200);
    //set up wifi server
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
    Serial.println("WiFi connected, waiting for client to start");
    //Serial.print("IP address: ");
    //Serial.println(WiFi.localIP());      
    //Serial.printf("IP port: %d\n",port);      
    server.begin(port);               
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    
    //setup the pwm (color) channels,1KHz,8bit
    for (int i = 0; i < 3; i++) {   
    ledcSetup(chns[i], 1000, 8);
    ledcAttachPin(ledPins[i], chns[i]);
  }
  color_count = 0;
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
      if(Serial.available()){                       // if there's bytes to read from the serial monitor,
        client.print(Serial.readStringUntil('\n')); // print it out the client.
        while(Serial.read()>0);                     // clear the wifi receive area cache
      }
      
  //color generation and writing to client terminal
  client.println("color #" + String(color_count));
  red = random(0, 256);
  green = random(0, 256);
  blue = random(0, 256);
  setColor(red, green, blue);
  //read red, green, and blue values of randomly chosen color from ESP32
  duty_red = ledcRead(chns[0]);
  duty_green = ledcRead(chns[1]);
  duty_blue = ledcRead(chns[2]);
  //write RGB values to client terminal
  client.println("current RED value is: " + String(duty_red));
  client.println("current GREEN value is: " + String(duty_green));
  client.println("current BLUE value is: " + String(duty_blue));
  redHex = String(int(duty_red),HEX);
  if(int(duty_red) < 15){
    redHex = String("0") + redHex; //add a 0 before numbers < 15 so all hex values are the same length
  }
  greenHex = String(int(duty_green),HEX);
  if(int(duty_green) < 15){
    greenHex = String("0") + greenHex;
  }
  blueHex = String(int(duty_blue),HEX);
  if(int(duty_blue) < 15){
    blueHex = String("0") + blueHex;
  }
  fullHex = redHex + greenHex + blueHex;
  //write hex value to client terminal
  client.println("current HEX value is: " + String(fullHex));
  delay(7500);
  color_count++;

    }
    client.stop();                                  // stop the client connecting.
    Serial.println("Client Disconnected.");
  }
}

void setColor(byte r, byte g, byte b) {
  ledcWrite(chns[0], 255 - r); //Common anode LED, low level to turn on the led.
  ledcWrite(chns[1], 255 - g);
  ledcWrite(chns[2], 255 - b);
}
