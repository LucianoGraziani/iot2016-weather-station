// Dependencies
#include <cactus_io_DHT22.h>

// Defines
#define DHT22_PIN 2

// Inicializations
DHT22 dht(DHT22_PIN);

// Variables and constants
int led13Pin = 13;

// Light sensor
int lightSensorPin = A0;
int lightSensorValue = 0;

// Functions for each device

// Format: sensorNameSetup() and sensorNameLoop()
void humidityAndTemperatureSetup()
{
	dht.begin();
	Serial.println("DHT sensor setup finished.");
}
/**
 * Uses D2 for output data
 */
void humidityAndTemperatureLoop()
{
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	dht.readHumidity();
	dht.readTemperature();

	if (isnan(dht.humidity) || isnan(dht.temperature_C))
	{
		Serial.println("DHT sensor read failure!");
		return;
	}

	// TODO INIT: esto tiene que reemplazarse con el envío de los datos a internet
	Serial.println("DHT22 Humidity - Temperature Sensor");
	Serial.println("RH\t\tTemp (C)\tTemp (F)\tHeat Index (C)\tHeat Index (F)");
	Serial.print(dht.humidity); Serial.print(" %\t\t");
	Serial.print(dht.temperature_C); Serial.print(" *C\t");
	Serial.print(dht.temperature_F); Serial.print(" *F\t");
	Serial.print(dht.computeHeatIndex_C()); Serial.print(" *C\t");
	Serial.print(dht.computeHeatIndex_F()); Serial.println(" *F");
	// END

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(2000);
}

/**
 * Uses A0 output for data
 */
void lightSensorLoop()
{
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	// Process
	lightSensorValue = analogRead(lightSensorPin);
	Serial.print("Light sensor: (0-1024 RAW ANALOG)"); Serial.println(lightSensorValue, DEC);

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(2000);
}
/******************************
 * Main functions
 */
void setup()
{
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
	humidityAndTemperatureSetup();
}

void loop()
{
	humidityAndTemperatureLoop();
	lightSensorLoop();
}
