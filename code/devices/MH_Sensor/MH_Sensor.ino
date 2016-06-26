int lightSensorPin = A7; // select the input pin for the potentiometer
int led13Pin = 13; // select the pin for the LED
int lightSensorValue = 0; // variable to store the value coming from the sensor

void lightSensorLoop()
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
}

void setup()
{
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
}

void loop()
{
	lightSensorLoop();
}
