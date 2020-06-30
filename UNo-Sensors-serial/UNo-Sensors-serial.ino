#include <ArduinoJson.h>


#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4); // Change to (0x27,16,2) for 16x2 LCD.
#define DHTPINGROW 12     // Digital pin connected to the DHT sensor 
#define DHTPINBLM 13     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define DHTTYPEBLM    DHT11     // DHT 22 (AM2302)

DHT dht(DHTPINGROW, DHTTYPE);
DHT dht1(DHTPINBLM, DHTTYPEBLM); 

const int OUTFILTER = 3; //OUTFILTER
const int INPUTAIR = 4; //GROW
const int INPUTAIR2 = 5; //BLOOM
const int WATERPUMP = 6; //PUMP
const int WATERPUMP2 = 8; //PUMP

String message = "";
bool messageReady = false;
bool mannualmode =false;

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  dht.begin();
  dht1.begin();
  pinMode(OUTFILTER, OUTPUT);
  pinMode(INPUTAIR, OUTPUT);
  pinMode(INPUTAIR2, OUTPUT);
  pinMode(WATERPUMP, OUTPUT);
  digitalWrite(OUTFILTER, LOW);
  digitalWrite(INPUTAIR, LOW);
  digitalWrite(INPUTAIR2, LOW);
  digitalWrite(WATERPUMP, LOW);
  digitalWrite(WATERPUMP2, LOW);
  lcd.init();
  lcd.backlight();
 
}


void checkLogicblm(int tempblm , int humidityblm){
         if(tempblm > 30 ){
          digitalWrite(INPUTAIR2, HIGH);
          }
          if(tempblm < 25 ){
          digitalWrite(INPUTAIR2, LOW);
          }
         }

         

void checkLogicgrow(int temp , int humidity){
  if(temp > 28 ){
    digitalWrite(INPUTAIR, HIGH);
      }
  if(temp < 25 ){
    digitalWrite(INPUTAIR, LOW);
  }
  }

  

String fixonoff (bool val) {
  if(val){
    return "ON";
    }
    if(!val){
      return "OFF";
      }  
  }

    

  int printerHum(int hum, int humblm) {
     lcd.setCursor(13, 0); // Set the cursor on the first column and first row.
     lcd.print("H"); // Print the string "Hello World!"
     lcd.setCursor(14, 0); // Set the cursor on the first column and first row.
     lcd.print(hum); // Print the string "Hello World!"
     lcd.setCursor(19, 0); // Set the cursor on the first column and first row.
     lcd.print("%"); // Print the string "Hello World!"
     lcd.setCursor(13, 1); // Set the cursor on the first column and first row.
     lcd.print("H"); // Print the string "Hello World!"
     lcd.setCursor(14, 1); // Set the cursor on the first column and first row.
     lcd.print(humblm); // Print the string "Hello World!"
     lcd.setCursor(18, 1); // Set the cursor on the first column and first row.
     lcd.print("%"); // Print the string "Hello World!"
    
    }

    int printerTemp(int temp, int tempblm){
       String flt = fixonoff(digitalRead(3));
              String air1 = fixonoff(digitalRead(4));
                     String air2 = fixonoff(digitalRead(5));
      String Pump = fixonoff(digitalRead(6));
         String Pump2 = fixonoff(digitalRead(8));
 
      lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
      lcd.print("Grow "); // Print the string "Hello World!"
      lcd.setCursor(6, 0); // Set the cursor on the first column and first row.
      lcd.print("C"); // Print the string "Hello World!"
      lcd.setCursor(7, 0); // Set the cursor on the first column and first row.
      lcd.print(temp); // Print the string "Hello World!
      lcd.setCursor(0, 1); // Set the cursor on the first column and first row.
      lcd.print("Bloom"); // Print the string "Hello World!"
      lcd.setCursor(6, 1); // Set the cursor on the first column and first row.
      lcd.print("C"); // Print the string "Hello World!"
      lcd.setCursor(7, 1); // Set the cursor on the first column and first row.
      lcd.print(tempblm); // Print the string "Hello World!"
      lcd.setCursor(0, 2); // Set the cursor on the first column and first row.
      lcd.print("OUT="); // Print the string "Hello World!"
      lcd.setCursor(6, 2); // Set the cursor on the first column and first row.
      lcd.print(flt); // Print the string "Hello World!"
      lcd.setCursor(11, 2); // Set the cursor on the first column and first row.
      lcd.print("AIR="); // Print the string "Hello World!"
      lcd.setCursor(17, 2); // Set the cursor on the first column and first row.
      lcd.print(air1); // Print the string "Hello World!"
      lcd.setCursor(0, 3); // Set the cursor on the first column and first row.
      lcd.print("Pump="); // Print the string "Hello World!"
      lcd.setCursor(7, 3); // Set the cursor on the first column and first row.
      lcd.print(Pump2); // Print the string "Hello World!"
     }

    
void loop() {

  delay(1000);
 bool val = digitalRead(11);
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h11 = dht.readHumidity();
  float t11 = dht.readTemperature();

  
if(!val) {
         digitalWrite(WATERPUMP2, HIGH);
  }
if(val) {
           digitalWrite(WATERPUMP2, LOW);
  }


  

        while(Serial.available()) {
    message = Serial.readString();
    messageReady = true;
  }

    if(messageReady) {
    DynamicJsonDocument doc(512); // ArduinoJson version 6+
    DeserializationError error = deserializeJson(doc,message);
    if(error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      messageReady = false;
      return;
    }
    if(doc["type"] == "getTemps") {
      doc["type"] = "rsp";
      // Get data from analog sensors
      doc["gtemp"] = (t1);
      doc["ghum"] = (h1);
      doc["btemp"] = (t11);
      doc["bhum"] = (h11);
      serializeJson(doc,Serial);
    }

      String flt = fixonoff(digitalRead(3));
      String air1 = fixonoff(digitalRead(4));
      String air2 = fixonoff(digitalRead(5));
      String Pump = fixonoff(digitalRead(6));

      if(doc["type"] == "getStatus") {
      doc["type"] = "statusresponse";
      // Get data from analog sensors
      doc["Filter"] = (flt);
      doc["GrowAir"] = (air1);
      doc["BloomAir"] = (air2);
      doc["Pump"] = (Pump);
      serializeJson(doc,Serial);
    }
        if(doc["type"] == "oPump") {
          digitalWrite(WATERPUMP, HIGH);
          doc["type"] = "pumpresponse";
          doc["pumpison"] = true;
          serializeJson(doc,Serial);
    }
            if(doc["type"] == "cPump") {
          digitalWrite(WATERPUMP, LOW);
      doc["type"] = "pumpresponse";
      doc["pumpison"] = false;
      serializeJson(doc,Serial);
    }
    messageReady = false;
  }
  
  printerTemp(t11,t1);
  printerHum(h11,h1);
  checkLogicblm(t11,h11);
  checkLogicgrow(t1,h1);


}
