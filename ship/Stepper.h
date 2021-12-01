#ifndef _Stepper_H__
#define _Stepper_H__

#include <Arduino.h>
#include <cstddef>

class Stepper
{
private:
    size_t _l1;
    size_t _l2;
    size_t _l3;
    size_t _l4;
    int _delayMillSecond;

public:
    Stepper(size_t l1, size_t l2, size_t l3, size_t l4, int speed);
    void Forward(int setpNum);
    void Back(int setpNum);
    void Reset();
    ~Stepper();
};

#endif