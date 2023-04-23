/*
 Programmed by Diego Diaz Neergaard 
 Student number: s163261
 Department : DTU Electro
 Date: 20 April 2023
 Version : v1.0
 */
 /*
 ThingSpeak implementation by Emre Hayatoglu
 */
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'
#define soilPower D5 // GPIO 14 for the soil moisture sensor
#define sensorPin A0 // analog pin for the soil moisture data
#define waterpump D6 // GPIO 0 for the relay signal to the waterpump

#define ledRed D7 // for the RGB led 
#define ledGreen D3 // for the RGB led 
#define ledBlue D4 // for the RGB led 

int lcdColumns = 16; // set the LCD number of columns and rows
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); // set LCD address, number of columns and rows

// Replace with your network details
const char* ssid = "LOL";
const char* password = "diego123";

WiFiClient client;

unsigned long channelID = 2077705; //your channel
const char * myWriteAPIKey = "EK8E6JY8O4DX0T6X"; // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // post data every 1 second

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
  
  lcd.clear();
  lcd.init(); // initialize LCD                     
  lcd.backlight(); // turn on LCD backlight  

  pinMode(ledRed, OUTPUT); 
	pinMode(ledGreen, OUTPUT);
	pinMode(ledBlue, OUTPUT);

  digitalWrite(ledRed, LOW);
	digitalWrite(ledGreen, LOW);
	digitalWrite(ledBlue, LOW);

	pinMode(soilPower, OUTPUT); // initialize digital pin D5 for waterpump
	pinMode(waterpump, OUTPUT);// initialize digital pin D3 for waterpump
	digitalWrite(soilPower, LOW); // Initially keep the sensor OFF
}

void loop() {
	//get the reading from the function below and print it
  ThingSpeak.begin(client);
  if (client.connect(server, 80)) {
    
    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();

    Serial.print("RSSI: ");
    Serial.println(rssi); 
	  int moisture = readSensor();
  
	Serial.print("Soil analog value: ");
	Serial.println(moisture);
  ThingSpeak.setField(4,moisture);

	// Determine status of our soil
	if (moisture < soilWet) {
     Serial.println("Status: Soil is too wet");
     lcd.setCursor(0,0);
     lcd.print("Soil is too wet");
     digitalWrite(ledRed, LOW);
	   digitalWrite(ledGreen, LOW);
	   digitalWrite(ledBlue, HIGH);
     digitalWrite(waterpump, LOW);

	} else if (moisture >= soilWet && moisture < soilDry) {
		        Serial.println("Status: Soil moisture is perfect");
            lcd.setCursor(0,0);
            lcd.print("Soil is perfect");
            digitalWrite(waterpump, LOW);
            digitalWrite(ledRed, LOW);
	          digitalWrite(ledGreen, HIGH);
	          digitalWrite(ledBlue, LOW);
	 }        else { 
                 Serial.println("Status: Soil is too dry - time to water!");
	               digitalWrite(waterpump, HIGH);
                 lcd.setCursor(0,0);
                 lcd.print("Time to water !");
                 digitalWrite(ledRed, HIGH);
	               digitalWrite(ledGreen, LOW);
	               digitalWrite(ledBlue, LOW);
                 delay(1000);
                }
  ThingSpeak.writeFields(channelID, myWriteAPIKey);
  delay(800);	// Take a reading every second for testing
  }
  client.stop();
  delay(postingInterval);
}
//  This function returns the analog soil moisture measurement
  int readSensor() {
	digitalWrite(soilPower, HIGH);	// Turn the sensor ON
	delay(10);							// Allow power to settle
	int val = analogRead(sensorPin);	// Read the analog value form sensor
	digitalWrite(soilPower, LOW);		// Turn the sensor OFF
	return val;							// Return analog moisture value
}