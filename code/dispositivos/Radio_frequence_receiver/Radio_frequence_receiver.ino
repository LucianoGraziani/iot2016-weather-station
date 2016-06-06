#include <VirtualWire.h>

int digPin = 6;
int ledPin = 13;

void setup()
{
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(digPin);
  vw_setup(4000);  // Bits per sec
  pinMode(ledPin, OUTPUT);
  vw_rx_start();       // Start the receiver PLL running
  Serial.begin(9600);
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    Serial.print("Got: ");
        
    for (int i = 0; i < buflen; i++)
    {
        int a = buf[i]; 
        char c = a;
        Serial.print(c);
    }
    Serial.println("");
  }
}
