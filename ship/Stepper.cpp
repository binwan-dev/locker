#include "Stepper.h"
#include <Arduino.h>

Stepper::Stepper(size_t l1, size_t l2, size_t l3, size_t l4, int delayMillSecond)
{
    pinMode(l1, OUTPUT);
    pinMode(l2, OUTPUT);
    pinMode(l3, OUTPUT);
    pinMode(l4, OUTPUT);

    this->_l1 = l1;
    this->_l2 = l2;
    this->_l3 = l3;
    this->_l4 = l4;
    this->_delayMillSecond = delayMillSecond;

    Reset();
}

void Stepper::Reset()
{
    digitalWrite(this->_l1, LOW);
    digitalWrite(this->_l2, LOW);
    digitalWrite(this->_l3, LOW);
    digitalWrite(this->_l4, LOW);
}

void Stepper::Forward(int setpNum)
{
    for (int i = 0; i < setpNum; i++)
    {
        Reset();
        digitalWrite(this->_l1, HIGH);
        digitalWrite(this->_l2, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l2, HIGH);
        digitalWrite(this->_l3, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l3, HIGH);
        digitalWrite(this->_l4, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l4, HIGH);
        digitalWrite(this->_l1, HIGH);
        delay(this->_delayMillSecond);
    }
}

void Stepper::Back(int setpNum)
{
    for (int i = 0; i < setpNum; i++)
    {
        Reset();
        digitalWrite(this->_l1, HIGH);
        digitalWrite(this->_l4, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l4, HIGH);
        digitalWrite(this->_l3, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l3, HIGH);
        digitalWrite(this->_l2, HIGH);
        delay(this->_delayMillSecond);

        Reset();
        digitalWrite(this->_l2, HIGH);
        digitalWrite(this->_l1, HIGH);
        delay(this->_delayMillSecond);
    }
}