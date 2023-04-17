//#include <ESP32Servo.h>
#include <Servo.h>

class BServo
{
private:
    int _pin;
    Servo _servo;

public:
    BServo(int pin);
    void Forward(int trunsNum);
    void Back(int trunsNum);
    void Reset();
};