/*
 Programmed by Diego Diaz Neergaard 
 Student number: s163261
 Department : DTU Electro
 Date: 20 April 2023
 Version : v1.0
 */
 /*
 Thingspeak implementation by Emre Hayatoglu
 s226716
 */
 

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

#define ONE_WIRE_BUS D5 //GPIO 14. Soil temp sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#define uv_light D4       //GPIO 2
#define photo_resistor A0 //ADC0

#define DHTPIN D6     //GPI0 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Replace with your network details
const char* ssid = "LOL";
const char* password = "diego123";

WiFiClient client;


unsigned long channelID = 2077705; //your channel
const char * myWriteAPIKey = "EK8E6JY8O4DX0T6X"; // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // post data every 10 seconds


int lcdColumns = 20; // set the LCD number of columns and rows
int lcdRows = 4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); // set LCD address, number of columns and rows

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  sensors.begin();
  dht.begin(); // initialize DTH11 
  lcd.clear();
  lcd.init(); // initialize LCD                     
  lcd.backlight(); // turn on LCD backlight  
  Serial.println(F("DHT11 measurements")); // begin measurements
  pinMode(uv_light, OUTPUT);// initialize digital pin D4 for UV lamp. 
}

void loop() {
ThingSpeak.begin(client);
  if (client.connect(server, 80)) {
    
    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();

    Serial.print("RSSI: ");
    Serial.println(rssi); 

////  SOIL TEMP SYSTEM   ////
  sensors.requestTemperatures(); 
  float T1 = sensors.getTempCByIndex(0);

  if (T1 != DEVICE_DISCONNECTED_C){
     Serial.print(T1);
     Serial.println("  ");
     lcd.setCursor(0,2);
     lcd.print("Soil Temp: ");
     lcd.setCursor(12, 2);
     lcd.print(T1); 
     }
     else {
           Serial.print("Sensor is not working");
           lcd.print("XX.XX");
     }

////  UV_LIGHT SYSTEM-ANALOG   ////
  int analogValue = analogRead(photo_resistor);
  ThingSpeak.setField(2,analogValue);

  Serial.print("Plant UV-light =");
  Serial.print(analogValue);  // raw analog reading

  if (analogValue < 390){
      digitalWrite(uv_light, HIGH);  // turn the UV-light on (HIGH is the voltage level)
      lcd.setCursor(0,3);
      lcd.print("Plant UV-light: ON "); // print to the lcd display
      Serial.println(" => Very Dark");
  } else if (analogValue < 550){
             digitalWrite(uv_light, HIGH);  // turn the UV-light on (HIGH is the voltage level)
             lcd.setCursor(0,3);
             lcd.print("Plant UV-light: ON "); // print to the lcd display
             Serial.println("=> Dark");
    } else if (analogValue < 750 ){
              digitalWrite(uv_light, LOW);   // turn the UV-light off by making the voltage LOW
              lcd.setCursor(0,3);
              lcd.print("Plant UV-light: OFF"); // print to the lcd display
              Serial.println("=> Light");
      } else if (analogValue < 950){
                 digitalWrite(uv_light, LOW);   // turn the UV-light off by making the voltage LOW
                 lcd.setCursor(0,3);
                 lcd.print("Plant UV-light: OFF"); // print to the lcd display
                 Serial.println("=> Bright");
         } else {
                Serial.println("=> Very bright");
                digitalWrite(uv_light, LOW);   // turn the UV-light off by making the voltage LOW
                lcd.setCursor(0,3);
                lcd.print("Plant UV-light: OFF"); // print to the lcd display
           }
delay(300);

//////////////// DTH11- AIR-TEMPERATURE & HUMIDITY SYSTEM   ////////////////////////
  delay(500); 

  float h = dht.readHumidity(); // Read humidity
  float t = dht.readTemperature(); // Read temperature as Celsius (the default)
  float f = dht.readTemperature(true); // Read temperature as Fahrenheit (isFahrenheit = true)

  if (isnan(h) || isnan(t) || isnan(f)) {   // Check if any reads failed and exit early (to try again).
     Serial.println(F("Failed to read from DHT sensor!"));
     return;
  }

  float hif = dht.computeHeatIndex(f, h);  // Compute heat index in Fahrenheit (the default)
  float hic = dht.computeHeatIndex(t, h, false);  // Compute heat index in Celsius (isFahreheit = false)

  Serial.print(F("Air humidity: ")); // printing to the monitor
  Serial.print(h);
  Thingspeak.setField(1,h);
  Serial.print(F("%  Air temperature: ")); // printing to the monitor
  Serial.print(t);
  Serial.println(F("°C "));

  lcd.setCursor(0,0);
  lcd.print("Air temp: "); // print ti the lcd display
  lcd.print(t);
  lcd.print((char)223);
  lcd.print("C");
  ThingSpeak.setField(3,t);
  ThingSpeak.writeFields(channelID, myWriteAPIKey);

  lcd.setCursor(0,1);
  lcd.print("Air humidity: "); // print ti the lcd display
  lcd.print(h);
  }
  client.stop();

  // wait and then post again
  delay(postingInterval);
  
}
