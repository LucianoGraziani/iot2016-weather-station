#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#define ID  "5772e2ee7625421453a672ba"  // Put here your Ubidots variable ID
#define TOKEN  "YXCQHvPZJZsmGTt07FZDZNnGJRzCIM"  // Put here your Ubidots TOKEN
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
//IPAddress ip(192, 168, 0, 177);

Ubidots client(TOKEN);
int lightSensorPin = A7; // select the input pin for the potentiometer
int led13Pin = 13; // select the pin for the LED
int lightSensorValue = 0; // variable to store the value coming from the sensor

int lightSensorLoop()
{
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	// TODO INIT: esto tiene que reemplazarse con el envío de los datos a internet
	lightSensorValue = analogRead(lightSensorPin);
	// We'll have a few threshholds, qualitatively determined
  Serial.print(lightSensorValue);
	if (lightSensorValue < 10) {
		Serial.println(" - Very Bright");
	} else if (lightSensorValue < 200) {
		Serial.println(" - Bright");
	} else if (lightSensorValue < 500) {
		Serial.println(" - Light");
	} else if (lightSensorValue < 800) {
		Serial.println(" - Dim");
	} else {
		Serial.println(" - Dark");
	}
	// END

	delay(1000);

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(1000);

  return lightSensorValue;
}

void setup()
{
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop()
{  
  client.add(ID, lightSensorLoop());
  client.sendAll();
}
