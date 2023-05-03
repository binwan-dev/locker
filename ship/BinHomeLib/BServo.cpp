#include "BServo.h"

BServo::BServo(int pin)
{
    _pin = pin;
    _servo.attach(pin);
}
void BServo::Forward(int trunsNum)
{
    for (int i = 0; i < trunsNum; i++)
    {
        _servo.write(180);
        delay(720);
    }
}
void BServo::Back(int trunsNum)
{
    for (int i = 0; i < trunsNum; i++)
    {
        _servo.write(11);
        delay(720);
    }
}
void BServo::Reset()
{
    _servo.write(90);
    delay(500);
}