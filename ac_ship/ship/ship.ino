#include <IRremote.hpp>
#define IR_RECEIVE_PIN          5
void setup()
{
  Serial.begin(9600);
  IrSender.begin(IR_RECEIVE_PIN);
  IrSender.enableIROut(38);
}

void loop()
{
  
  IrSender.sendSamsung48(0xB24D, 0x8FD, 0xF70802FDB24D);
  Serial.println("send");
  delay(10000);
}