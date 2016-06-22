/////////////////////
// Pin used
////////////////////
//
// D2 HumidityAndTemperature
// D7 RF Transmitter
// D20 (SDA) BMP085
// D21 (SCL) BMP085
//
// A6 LightSensor
// A0 MoistureEC5

// Dependencies
#include <cactus_io_DHT22.h>
#include <Wire.h>
#include <VirtualWire.h>

// Defines
#define DHT22_PIN 2
#define BMP085_ADDRESS 0x77

// Inicializations
DHT22 dht(DHT22_PIN);

////////////////////////////
// Variables and constants
////////////////////////////

// Generals
int led13Pin = 13;

// RF Transmitter
int rfDigInput = 7;

// Moisture sensor
int moisturePin = A0;
int moistureValue = 0;

// Light sensor
int lightSensorPin = A6;
int lightSensorValue = 0;

// BMP085
const unsigned char bmp085_OSS = 0;
int bmp085_ac1;
int bmp085_ac2;
int bmp085_ac3;
unsigned int bmp085_ac4;
unsigned int bmp085_ac5;
unsigned int bmp085_ac6;
int bmp085_b1;
int bmp085_b2;
int bmp085_mb;
int bmp085_mc;
int bmp085_md;
long bmp085_b5;
short bmp085_temperature;
long bmp085_pressure;
const float bmp085_p0 = 101325;
float bmp085_altitude;

// Functions for each device

// Format: sensorNameSetup() and sensorNameLoop()

/////////////////////////////
// RF Transmitter
/////////////////////////////
void transmitterRFSetup() {
	vw_set_ptt_inverted(true);
	vw_set_tx_pin(rfDigInput);
	vw_setup(4000);
}
void transmitterRFLoop() {
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	const char *msg = "Hola mundo!";
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;
	vw_send((uint8_t *)msg, strlen(msg));
	vw_wait_tx();
	delay(500);
	digitalWrite(led13Pin,0);
	delay(500);

  Serial.print("Sent: ");
  Serial.println(*msg);
	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(1000);
}

/////////////////////////////
// MoistureEC5
/////////////////////////////
void moistureEC5Loop()
{
	// Reset analogReference
	analogReference(INTERNAL1V1);
	delay(500);

	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	moistureValue = analogRead(moisturePin);
	Serial.print("MoistureEC5: "); Serial.println(moistureValue, DEC);

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	// Reset analogReference
	analogReference(DEFAULT);
	delay(1500);
}

/////////////////////////////
// BMP085
/////////////////////////////
void bmp085Setup()
{
	Wire.begin();
	bmp085Calibration();
}
void bmp085Loop()
{
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	bmp085_temperature = bmp085GetTemperature(bmp085ReadUT());
	bmp085_pressure = bmp085GetPressure(bmp085ReadUP());
	bmp085_altitude = (float) 44330 * (1 - pow(((float) bmp085_pressure/bmp085_p0), 0.190295));

	// TODO INIT: esto tiene que reemplazarse con el envío de los datos a internet
	Serial.print("Temperature: ");
	Serial.print(bmp085_temperature, DEC);
	Serial.println(" *0.1 deg C");
	Serial.print("Pressure: ");
	Serial.print(bmp085_pressure, DEC);
	Serial.println(" Pa");
	Serial.print("Altitude: ");
	Serial.print(bmp085_altitude, 2);
	Serial.println(" m");
	Serial.println();
	// END

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(2000);
}
void bmp085Calibration()
{
	bmp085_ac1 = bmp085ReadInt(0xAA);
	bmp085_ac2 = bmp085ReadInt(0xAC);
	bmp085_ac3 = bmp085ReadInt(0xAE);
	bmp085_ac4 = bmp085ReadInt(0xB0);
	bmp085_ac5 = bmp085ReadInt(0xB2);
	bmp085_ac6 = bmp085ReadInt(0xB4);
	bmp085_b1 = bmp085ReadInt(0xB6);
	bmp085_b2 = bmp085ReadInt(0xB8);
	bmp085_mb = bmp085ReadInt(0xBA);
	bmp085_mc = bmp085ReadInt(0xBC);
	bmp085_md = bmp085ReadInt(0xBE);
}
short bmp085GetTemperature(unsigned int ut)
{
	long x1, x2;
	x1 = (((long)ut - (long)bmp085_ac6) * (long)bmp085_ac5) >> 15;
	x2 = ((long)bmp085_mc << 11)/(x1 + bmp085_md);
	bmp085_b5 = x1 + x2;

	return ((bmp085_b5 + 8)>>4);
}
long bmp085GetPressure(unsigned long up)
{
	long x1, x2, x3, b3, b6, p;
	unsigned long b4, b7;
	b6 = bmp085_b5 - 4000;
	x1 = (bmp085_b2 * (b6 * b6)>>12)>>11;
	x2 = (bmp085_ac2 * b6)>>11;
	x3 = x1 + x2;
	b3 = (((((long)bmp085_ac1)*4 + x3)<<bmp085_OSS) + 2)>>2;
	x1 = (bmp085_ac3 * b6)>>13;
	x2 = (bmp085_b1 * ((b6 * b6)>>12))>>16;
	x3 = ((x1 + x2) + 2)>>2;
	b4 = (bmp085_ac4 * (unsigned long)(x3 + 32768))>>15;
	b7 = ((unsigned long)(up - b3) * (50000>>bmp085_OSS));
	if (b7 < 0x80000000)
		p = (b7<<1)/b4;
	else
		p = (b7/b4)<<1;

	x1 = (p>>8) * (p>>8);
	x1 = (x1 * 3038)>>16;
	x2 = (-7357 * p)>>16;
	p += (x1 + x2 + 3791)>>4;

	return p;
}
char bmp085Read(unsigned char address)
{
	unsigned char data;
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(address);
	Wire.endTransmission();
	Wire.requestFrom(BMP085_ADDRESS, 1);
	while(!Wire.available());
	return Wire.read();
}
int bmp085ReadInt(unsigned char address)
{
	unsigned char msb, lsb;
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(address);
	Wire.endTransmission();
	Wire.requestFrom(BMP085_ADDRESS, 2);
	while(Wire.available()<2);
	msb = Wire.read();
	lsb = Wire.read();
	return (int) msb<<8 | lsb;
}
unsigned int bmp085ReadUT()
{
	unsigned int ut;
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(0xF4);
	Wire.write(0x2E);
	Wire.endTransmission();
	delay(5);
	ut = bmp085ReadInt(0xF6);
	return ut;
}
unsigned long bmp085ReadUP()
{
	unsigned char msb, lsb, xlsb;
	unsigned long up = 0;
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(0xF4);
	Wire.write(0x34 + (bmp085_OSS<<6));
	Wire.endTransmission();
	delay(2 + (3<<bmp085_OSS));
	Wire.beginTransmission(BMP085_ADDRESS);
	Wire.write(0xF6);
	Wire.endTransmission();
	Wire.requestFrom(BMP085_ADDRESS, 3);
	while(Wire.available() < 3);
	msb = Wire.read();
	lsb = Wire.read();
	xlsb = Wire.read();
	up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-bmp085_OSS);
	return up;
}

/////////////////////////////
// Humidity and temperature
/////////////////////////////
void humidityAndTemperatureSetup()
{
	dht.begin();
	Serial.println("DHT sensor setup finished.");
}
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

/////////////////////////////
// Light level Sensor
/////////////////////////////
void lightSensorLoop()
{
	// Process started indicator
	digitalWrite(led13Pin, HIGH);

	// TODO INIT: esto tiene que reemplazarse con el envío de los datos a internet
	lightSensorValue = analogRead(lightSensorPin);
	Serial.print("Light sensor: (0-1024 RAW ANALOG)"); Serial.println(lightSensorValue, DEC);
	// END

	// Process finished indicator
	digitalWrite(led13Pin, LOW);

	delay(2000);
}

/////////////////////////////
// Main Functions
/////////////////////////////
void setup()
{
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
	humidityAndTemperatureSetup();
	bmp085Setup();
	transmitterRFSetup();
}

void loop()
{
	moistureEC5Loop();
	humidityAndTemperatureLoop();
	lightSensorLoop();
	bmp085Loop();
	transmitterRFLoop();
}
