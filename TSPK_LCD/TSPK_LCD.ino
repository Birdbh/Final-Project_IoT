#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <LiquidCrystal_I2C.h>

// Replace with your network details
const char* ssid = "xxxxx";
const char* password = "xxxxxx";

WiFiClient client;

unsigned long channelID = 2077705; //your channel
const char * myReadAPIKey = "YOACHKQ9U8NDDLV3"; // your WRITE API key
const char* server = "api.thingspeak.com";

//float temperature;
//float humidity;
 
LiquidCrystal_I2C lcd(0x27, 16, 2); // setting up lcd screen
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.clear();
  lcd.backlight();

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

void loop() {
  // put your main code here, to run repeatedly:
  //to serial printer
  //temp_data = thingSpeakRead(channelID,Fields=3,ReadKey=myWriteAPIKey)
  //hum_data = thingSpeakRead(channelID,Fields=1,ReadKey=myWriteAPIKey)

  ThingSpeak.begin(client);
  
  long temp_data = ThingSpeak.readLongField(channelID, 2, myReadAPIKey);
  long hum_data = ThingSpeak.readLongField(channelID, 2, myReadAPIKey);

  Serial.print("Temperature: ");
  Serial.print(temp_data);
  Serial.println(" C");
  
  Serial.print("Humidity: ");
  Serial.print(hum_data);
  Serial.println(" %");
  // to lcd screen
  lcd.setCursor(2,0);
  lcd.print("Temp: ");
  lcd.print(temp_data);
  lcd.print(" C");  
  lcd.setCursor(2,1);
  lcd.print("Hum: ");
  lcd.print(hum_data);
  lcd.print(" %");  
  delay(10000);
}
