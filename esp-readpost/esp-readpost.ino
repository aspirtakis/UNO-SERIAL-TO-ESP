#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
SoftwareSerial s(0,2);
int data;
const char* ssid = "OPTILOOPATTIC";
const char* password = "!mijehof205!";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://18.222.8.67:8888/api/v2/mongodb/_table/Sensors";
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;
const char* tempgrow;

void setup() {
  Serial.begin(9600);
    s.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  s.begin(9600);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

}

void sendData() {
  
  
   DynamicJsonDocument doc2(512);
   DynamicJsonDocument doc(512);
     doc["type"] = "getTemps";
     serializeJson(doc,s);
        bool messageReady = false;
        String message;
      
           
           
   while(messageReady == false) { // blocking but that's ok
    if(s.available()) {
      message = s.readString();
      messageReady = true;

    }
  }

       DeserializationError error = deserializeJson(doc,message);
      if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

      auto Type = doc["type"].as<char*>();
      auto MetaData = doc["gtemp"].as<int>();;
      auto TempG = doc["gtemp"].as<int>();;
      auto TempB = doc["btemp"].as<int>();;
      auto HumG = doc["ghum"].as<int>();;
      auto HumB = doc["bhum"].as<int>();;
      
   String request = (String)"{\"resource\":[{\"id\":50,\"MetaData\":"+MetaData+(String)",\"TempGrow\":"+TempG+(String)",\"TempBloom\":"+TempB+(String)",\"HumidityG\":"+HumG+(String)",\"HumidityB\":"+HumB+(String)"}],\"ids\":[0],\"filter\":\"string\",\"params\":[\"string\"]}";

   //  Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-DreamFactory-API-Key", "9fa0af1c2aef033e9683b9570c27c4f281808584c04d752bd089f92c14640660");
      int httpResponseCode = http.POST(request);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
    delay(1000);
  }

void loop() {
  sendData();


  }
    



 
