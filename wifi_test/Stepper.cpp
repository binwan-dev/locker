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
}

void Stepper::Reset()
{
    digitalWrite(this->_l1, 0);
    digitalWrite(this->_l2, 0);
    digitalWrite(this->_l3, 0);
    digitalWrite(this->_l4, 0);
}

void Stepper::Forward(int setpNum)
{
    for (int i = 0; i < setpNum; i++)
    {
        digitalWrite(this->_l1, 1);
        digitalWrite(this->_l2, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l2, 1);
        digitalWrite(this->_l3, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l3, 1);
        digitalWrite(this->_l4, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l4, 1);
        digitalWrite(this->_l1, 1);
        Reset();
        delay(this->_delayMillSecond);
    }
}

void Stepper::Back(int setpNum)
{
    for (int i = 0; i < setpNum; i++)
    {
        digitalWrite(this->_l1, 1);
        digitalWrite(this->_l4, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l4, 1);
        digitalWrite(this->_l3, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l3, 1);
        digitalWrite(this->_l2, 1);
        Reset();
        delay(this->_delayMillSecond);

        digitalWrite(this->_l2, 1);
        digitalWrite(this->_l1, 1);
        Reset();
        delay(this->_delayMillSecond);
    }
}