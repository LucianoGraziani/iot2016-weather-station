#include <VirtualWire.h>

int rfDigPin = 7;
int led13Pin = 13;
int relePin = 12;

float actualValue = 0.00;

void receiverRFSetup()
{
	vw_set_ptt_inverted(true);
	vw_set_rx_pin(rfDigPin);
	vw_setup(4000);
	vw_rx_start();
}
void receiverRFLoop()
{
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;
	if (vw_get_message(buf, &buflen))
	{
		// Main process started indicator
		digitalWrite(led13Pin, HIGH);

		// TODO INIT: esto tiene que reemplazarse con el chequeo de los datos y la consecuente activación de algún relé
		Serial.print("Got: ");
		char c;
		for (int i = 0; i < buflen; i++)
		{
			int a = buf[i];
			c = a;
			Serial.print(c);
		}
		if(c == '1') {
			digitalWrite(relePin, HIGH);
		} else {
			digitalWrite(relePin, LOW);
		}
		Serial.println("");
		// END

		delay(1000);
		digitalWrite(led13Pin, LOW);
	}

	delay(1000);
}

void setup()
{
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
	pinMode(relePin, OUTPUT);
	receiverRFSetup();
}

void loop()
{
	receiverRFLoop();
}
