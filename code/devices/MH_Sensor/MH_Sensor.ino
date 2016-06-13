int lightSensorPin = A0; // select the input pin for the potentiometer
int led13Pin = 13; // select the pin for the LED
int lightSensorValue = 0; // variable to store the value coming from the sensor

void lightSensorLoop()
{
	// Reset analogReference
	analogReference(DEFAULT);
	delay(500);
	lightSensorValue = analogRead(lightSensorPin);
	digitalWrite(led13Pin, HIGH);
	delay(lightSensorValue);
	digitalWrite(led13Pin, LOW);
	delay(lightSensorValue);
	Serial.println(lightSensorValue, DEC);
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
