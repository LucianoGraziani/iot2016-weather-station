int sensorPin = A0;
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  delay(1000);
  Serial.println (sensorValue, DEC);
}
