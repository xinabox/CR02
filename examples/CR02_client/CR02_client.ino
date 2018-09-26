#include <RH_RF95.h> // 

#define LED_BUILTIN 16

#define CR02_FREQUENCY 868.0

uint8_t tempdata[30];

/*
  Slave Select mapped to D13/PD2
  Interrupt mapped to D14/PB2
*/
RH_RF95 CR02;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  if (!CR02.init()){
    Serial.println("init failed");
  }

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // Failure to do that will result in extremely low transmit powers.

  //CR02.setModemConfig(CR02.Bw31_25Cr48Sf512);
  CR02.setFrequency(CR02_FREQUENCY);
  CR02.setTxPower(23, false);
}

void loop()
{
  Serial.println("Sending to CR02_server");
  // Send a message to CR02_server

  sprintf(tempdata, "%s", "Hello Server");

  CR02.send(tempdata, sizeof(tempdata));

  CR02.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (CR02.waitAvailableTimeout(3000))
  {
    // Should be a reply message for us now
    if (CR02.recv(buf, &len))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(CR02.lastRssi(), DEC);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is CR02_server running?");
  }
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
}


