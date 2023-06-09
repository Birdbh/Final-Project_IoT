// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Replace with your network details
const char* ssid = "Bird";
const char* password = "test123456";

WiFiClient client;

unsigned long channelID = 2077705; //your channel
const char * myWriteAPIKey = "EK8E6JY8O4DX0T6X"; // your WRITE API key
const char* server = "api.thingspeak.com";

const int postingInterval = 20 * 1000; // post data every 20 seconds

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  
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

// runs over and over again
void loop() {
  ThingSpeak.begin(client);
  if (client.connect(server, 80)) {
    
    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();

    Serial.print("RSSI: ");
    Serial.println(rssi); 

    int lightValue = analogRead(A0);
    if (isnan(lightValue)) {
      Serial.println("Failed to read from light sensor!");    
    }
    else{
      Serial.print(lightValue);
      //end of sensor readings
      ThingSpeak.setField(2,lightValue);
    }
            
      ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
    client.stop();

  // wait and then post again
  delay(postingInterval);
}
