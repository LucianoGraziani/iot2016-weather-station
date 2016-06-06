#include <VirtualWire.h>

int digInput = 7;
int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(digInput);
  vw_setup(4000);// speed of data transfer Kbps
  Serial.begin(9600);
}

void loop() {
  digitalWrite(ledPin,1);
  const char *msg = "Hola mundo!";
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(ledPin,0);
  delay(1000);
}
