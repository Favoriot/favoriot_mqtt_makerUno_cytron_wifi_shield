#include "WiFiEsp.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2,3); //RX,TX
#endif

#define sensorPin A0

#define accessToken "rw-apikey/acesstoken"; //replace with your access token

const char* ssid = "";  // **replace the SSID
const char* pass = "";  // **replace the password
int status = WL_IDLE_STATUS;

const char* mqttServer = "mqtt.favoriot.com";
const int mqttPort = 1883;

const char* username = accessToken; 
const char* password = accessToken; 

int qos = 0;

int celcius = 0;

unsigned long lastSend;

WiFiEspClient client;
PubSubClient mqttClient(client);

void reconnect() {
  while (!mqttClient.connected()) {
     Serial.print("re-try Connecting to Favoriot platform ...");
     if(mqttClient.connect("f0f6b33a-f6b7-4aaf-8bb1-a6d4541a6691",username,password)){ // set any device id or use device id in favoriot platform
        Serial.println("Connected");
        String tempToken = accessToken;
        String topicSubscribe = tempToken + "/v2/streams/status";
        char tempSubscribe[topicSubscribe.length()+1];
        topicSubscribe.toCharArray(tempSubscribe,topicSubscribe.length()+1);
        mqttClient.subscribe(tempSubscribe);
      }
      else{
        Serial.print(mqttClient.state());
        Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
  
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Response:[");
    for (int i = 0; i < length; i++){
       Serial.print((char)payload[i]); 
     }
    Serial.print("] ");
    Serial.println();
 }
 
void setup() {
    Serial.begin(115200);  // start serial for terminal  
    Serial1.begin(9600);   // start esp8266 serial 
    
    pinMode(sensorPin,INPUT); 
  
    WiFi.init(&Serial1);

   if(WiFi.status() == WL_NO_SHIELD){
      Serial.println("Wifi shield no present");
      while(true);
   }
   // attempt to connect wifi network
   while(status != WL_CONNECTED){
     Serial.print("Attempting to connect to WPA SSID: ");
     Serial.println(ssid);
     status = WiFi.begin(ssid,pass); // connect to WPA/WPA2 network
   }
   
   Serial.println("Connected to the network");
   delay(1500);
   mqttClient.setServer(mqttServer,mqttPort);
   mqttClient.setCallback(callback);
}


void loop() {
    if (!mqttClient.connected()) {
      reconnect();
     } 
    if (millis() - lastSend > 30000 ) { // Update and send only after 30 seconds
     celcius = analogRead(sensorPin) * 0.488;  //read the temperature and convert to celcius
     dataStream(celcius);  //send data to FAVORIOT platform
     lastSend = millis(); 
    }

    mqttClient.loop();
    
}


void dataStream(int celcius){ 
  String json = "{";
    json += "\"device_developer_id\":"; 
    json += "\"deviceDefault@favoriot\""; // replace with your device developer id
    json +=",";
    json +="\"data\":";
    json += "{";
    json += "\"Temperature\":";
    json += "\""+String(celcius)+"\"";
    json += "}}";

    Serial.print("Sending: ");
    Serial.println(json);
    char tempObj[json.length()+1];
    json.toCharArray(tempObj,json.length()+1);
    String tempToken = accessToken;
    String topicPublish = tempToken+"/v2/streams";
    char tempPublish[topicPublish.length()+1];
    topicPublish.toCharArray(tempPublish,topicPublish.length()+1);
    mqttClient.publish(tempPublish,tempObj);

}
