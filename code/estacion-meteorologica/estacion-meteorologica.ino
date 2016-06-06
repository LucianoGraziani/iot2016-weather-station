// Dependencies
#include <cactus_io_DHT22.h>

// Defines
#define DHT22_PIN 2

// Inicializations
DHT22 dht(DHT22_PIN);

// Variables and constants

// Functions for each device

// Format: sensorNameSetup() and sensorNameLoop()
void humidityAndTemperatureSetup()
{
	dht.begin();
	Serial.println("DHT sensor setup finished.");
}

void humidityAndTemperatureLoop()
{
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

		delay(3000);
}

/******************************
 * Main functions
 */
void setup()
{
	Serial.begin(9600);
	humidityAndTemperatureSetup();
}

void loop()
{
	humidityAndTemperatureLoop();
}
