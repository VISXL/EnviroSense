#include "outputs.h"
#include "config.h"
#include <Arduino.h>

static unsigned long lastBlinkMs = 0;
static bool blinkOn = false;

void outputsBegin() {
  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Force LED green immediately on startup
  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, HIGH);
  digitalWrite(LED_B_PIN, LOW);
  noTone(BUZZER_PIN);
}

static void setRGB(bool r, bool g, bool b) {
  digitalWrite(LED_R_PIN, r ? HIGH : LOW);
  digitalWrite(LED_G_PIN, g ? HIGH : LOW);
  digitalWrite(LED_B_PIN, b ? HIGH : LOW);
}

static bool blinkAt(unsigned long periodMs) {
  unsigned long now = millis();
  if (now - lastBlinkMs >= periodMs) { lastBlinkMs = now; blinkOn = !blinkOn; }
  return blinkOn;
}

void outputsTick(State s) {
  switch (s) {
    case STATE_IDLE:    setRGB(false,false,true); noTone(BUZZER_PIN); break;
    case STATE_MONITOR: setRGB(false,true,false); noTone(BUZZER_PIN); break;
    case STATE_WARNING: {
      bool on = blinkAt(500);
      if (on) { setRGB(true,true,false); tone(BUZZER_PIN,1000); }
      else    { setRGB(false,false,false); noTone(BUZZER_PIN); }
      break;
    }
    case STATE_ALERT: setRGB(true,false,false); tone(BUZZER_PIN,2000); break;
    case STATE_FAIL: {
      bool on = blinkAt(150);
      if (on) { setRGB(true,false,false); tone(BUZZER_PIN,500); }
      else    { setRGB(false,false,false); noTone(BUZZER_PIN); }
      break;
    }
  }
}
