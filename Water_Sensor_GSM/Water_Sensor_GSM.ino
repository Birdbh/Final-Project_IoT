#define waterpin D7
#define wateranalog A0
#include <SoftwareSerial.h>


SoftwareSerial SIM900(7, 8); 

void setup() {
	// Set D7 as an OUTPUT
	pinMode(waterpin, OUTPUT);
	
	// Set to LOW so no power flows through the sensor
	digitalWrite(waterpin, LOW);
	
	SIM900.begin(19200);
    delay(20000);  
}

void loop() {
	if (readSensor() == 0){
        GSM()
    }
	
	delay(1000);
}

//This is a function used to get the reading
int readSensor() {
	digitalWrite(waterpin, HIGH);
	delay(20);
	water_level = analogRead(sensorPin);
	digitalWrite(waterpin, LOW);
	return water_level;
}

void GSM() {
    //Turn on GSM shield
    SIM900.print("AT+CMGF=1\r"); 
  ` delay(100);

    //Add phone number to send text to with international format
    SIM900.println("AT+CMGS=\"PHONENUMBERHERE\""); 
    delay(100);

    SIM900.println("You plant needs water!!!"); 
    delay(100);

    // End AT command with a ^Z, ASCII code 26
    SIM900.println((char)26); 
    delay(100);
    SIM900.println();
    // Give module time to send SMS
    delay(5000); 

}