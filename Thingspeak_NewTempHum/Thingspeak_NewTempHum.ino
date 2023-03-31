// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// For testing, in case you haven't, install the Adafruit BME280 library.
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

//Initialize the sensor:
Adafruit_BME280 bme;

// Replace with your network details
const char* ssid = "";
const char* password = "";

WiFiClient client;


unsigned long channelID = 2077705; //your channel
const char * myWriteAPIKey = "EK8E6JY8O4DX0T6X"; // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // post data every 10 seconds

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  bool status;

  //  Check for connections.
  status = bme.begin(0x76);
  if (!status){
    Serial.println("Couldn't find a BME280 sensor, check your connections");
    while (1);
  }
  
  // Connecting to WiFi network
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
  
}

void loop() {
  ThingSpeak.begin(client);
  if (client.connect(server, 80)) {
    
    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();

    Serial.print("RSSI: ");
    Serial.println(rssi); 


    ThingSpeak.setField(4,rssi);

// **** This part reads only sensors and calculates
            float h = bme.readHumidity();
            // Read temperature as Celsius (the default)
            float t = bme.readTemperature();
            // Read temperature as Fahrenheit
            float f = t * 9 / 5 + 32;
            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t)) {
              Serial.println("Failed to read from sensor!");
              strcpy(celsiusTemp,"Failed");
              strcpy(fahrenheitTemp, "Failed");
              strcpy(humidityTemp, "Failed");         
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F ");
        //end of sensor readings
              ThingSpeak.setField(3,t);
              //ThingSpeak.setField(2,f)  In case we decide to add Fahrenheit readings as well.
              ThingSpeak.setField(1,h);
            }
            
      ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
    client.stop();

  // wait and then post again
  delay(postingInterval);
}
