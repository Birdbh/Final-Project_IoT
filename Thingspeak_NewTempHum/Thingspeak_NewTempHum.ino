// Check here for connections: https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Using the new temp sensor
#include <DallasTemperature.h>
#include <OneWire.h>

const int oneWireBus = 4;

OneWire oneWire(oneWireBus);

DallasTemperature sensors(&oneWire);

// Replace with your network details
const char* ssid = "LOL";
const char* password = "diego123";

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

  // Start the DS18B20 sensor
  sensors.begin();


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
            // Read temperature as Celsius (the default)
            sensors.requestTemperatures();
            float t = sensors.getTempCByIndex(0);
            // Check if any reads failed and exit early (to try again).
            if (isnan(t)) {
              Serial.println("Failed to read from sensor!");      
            }
            else{
              // Computes temperature values in Celsius + Fahrenheit and Humidity
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
        //end of sensor readings
              ThingSpeak.setField(3,t);
              //ThingSpeak.setField(2,f)  In case we decide to add Fahrenheit readings as well.
            }
            
      ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
    client.stop();

  // wait and then post again
  delay(postingInterval);
}
