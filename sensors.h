#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>

void sensorsBegin();
bool sensorsRead(float &tempOut, float &humOut, int &lightOut);
void sensorsPush(float t, float h);
float sensorsAvgTemp();
float sensorsAvgHum();
float sensorsHSI();
unsigned long sensorsLastValidMs();

#endif
