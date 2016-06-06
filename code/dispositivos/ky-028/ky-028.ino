#include <math.h>

int bigNTCsensor = A0;
int digitalSensor = 7;

int digVal = 0;
int readVal = 0;

void setup(void) {
  pinMode(digitalSensor, INPUT);
  Serial.begin(9600);
}

void loop(void) {
  float celsius, fahrenheit;
  
  readVal = analogRead(bigNTCsensor);
  digVal = digitalRead(digitalSensor);
  double temp = Thermistor(readVal);
  Serial.print("Digital output: ");
  Serial.println(digVal);
  Serial.print("Temperature big sensor is = ");
  Serial.print(temp);
  Serial.println(" Celsius, ");

  delay(5000);
}

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
   //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return Temp;
}
