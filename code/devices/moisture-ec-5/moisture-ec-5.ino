int moisturePin = A0;
int moistureValue = 0; // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
	// Reset analogReference
	analogReference(INTERNAL1V1);
	delay(500);

	moistureValue = analogRead(moisturePin);
	Serial.print("MoistureEC5: "); Serial.println(moistureValue, DEC);

	// Reset analogReference
	analogReference(DEFAULT);
	delay(500);
}
