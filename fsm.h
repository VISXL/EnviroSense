#ifndef FSM_H
#define FSM_H
#include <Arduino.h>

enum State { STATE_IDLE, STATE_MONITOR, STATE_WARNING, STATE_ALERT, STATE_FAIL };

void fsmBegin();
void fsmUpdate(int lightReading);
void fsmOnInvalidRead();
State fsmGetState();
bool fsmIsBusy();

#endif
