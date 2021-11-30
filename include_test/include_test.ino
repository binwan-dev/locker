#include "Person.h"

void setup()
{
    Serial.println("setup");
}

void loop()
{
    Person a;
    a.Hello();
    Serial.println("loop...");
}