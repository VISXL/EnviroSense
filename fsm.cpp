#include "fsm.h"
#include "config.h"
#include "sensors.h"

static State currentState = STATE_IDLE;
static unsigned long stateEnteredMs = 0;
static unsigned long safeHeldSince = 0;
static unsigned long failValidSince = 0;

void fsmBegin() {
  currentState = STATE_IDLE;
  stateEnteredMs = millis();
  safeHeldSince = 0; failValidSince = 0;
}

State fsmGetState() { return currentState; }
bool fsmIsBusy() { return currentState == STATE_WARNING || currentState == STATE_ALERT; }

static void transition(State next) {
  if (next == currentState) return;
  Serial.print("[FSM] -> "); Serial.println(next);
  currentState = next;
  stateEnteredMs = millis();
  safeHeldSince = 0;
}

void fsmOnInvalidRead() {
  if (millis() - sensorsLastValidMs() > SENSOR_TIMEOUT_MS) transition(STATE_FAIL);
}

void fsmUpdate(int light) {
  unsigned long now = millis();
  if (currentState == STATE_IDLE) { transition(STATE_MONITOR); return; }
  if (currentState == STATE_FAIL) {
    if (failValidSince == 0) failValidSince = now;
    if (now - failValidSince > FAIL_RECOVERY_MS) { failValidSince = 0; transition(STATE_MONITOR); }
    return;
  }
  failValidSince = 0;
  float avgT = sensorsAvgTemp(), avgH = sensorsAvgHum(), hsi = sensorsHSI();
  if (isnan(avgT) || isnan(avgH)) return;

  float warnT = TEMP_WARN;
  if (light < LDR_DARK) warnT -= DARK_OFFSET;

  bool alertCond = (avgT > TEMP_ALERT) && (avgH > HUM_ALERT);
  bool warnCond  = (avgT > warnT) || (avgH > HUM_WARN) || (hsi > HSI_WARN);
  bool safeCond  = (avgT < TEMP_SAFE) && (avgH < HUM_SAFE);

  if (alertCond) transition(STATE_ALERT);
  else if (warnCond) transition(STATE_WARNING);
  else if (safeCond) {
    if (safeHeldSince == 0) safeHeldSince = now;
    if (now - safeHeldSince >= HYSTERESIS_MS) transition(STATE_MONITOR);
  } else safeHeldSince = 0;
}
