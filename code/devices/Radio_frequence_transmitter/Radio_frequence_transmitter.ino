#include <VirtualWire.h>

int rfDigInput = 7;
int led13Pin = 13;

void transmitterRFSetup() {
	vw_set_ptt_inverted(true);
	vw_set_tx_pin(rfDigInput);
	// speed of data transfer Kbps
	vw_setup(4000);
}
void transmitterRFLoop() {
	digitalWrite(led13Pin, 1);
	const char *msg = "Hola mundo!";
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;
	vw_send((uint8_t *)msg, strlen(msg));
	// Wait until the whole message is gone
	vw_wait_tx();
	delay(500);
	digitalWrite(led13Pin,0);
	delay(500);
}

void setup() {
	Serial.begin(9600);
	pinMode(led13Pin, OUTPUT);
	transmitterRFSetup();
}

void loop() {
	transmitterRFLoop();
}
