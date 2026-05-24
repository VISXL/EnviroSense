#include "sensors.h"
#include "config.h"
#include <DHT.h>

static DHT dht(DHT_PIN, DHT22);
static float tempBuf[BUFFER_SIZE];
static float humBuf[BUFFER_SIZE];
static int bufIdx = 0;
static int bufCount = 0;
static unsigned long lastValidMs = 0;

void sensorsBegin() {
  dht.begin();
  pinMode(LDR_PIN, INPUT);
  for (int i = 0; i < BUFFER_SIZE; i++) { tempBuf[i] = NAN; humBuf[i] = NAN; }
  bufIdx = 0; bufCount = 0; lastValidMs = 0;
}

bool sensorsRead(float &tempOut, float &humOut, int &lightOut) {
  tempOut = dht.readTemperature();
  humOut  = dht.readHumidity();
  lightOut = analogRead(LDR_PIN);
  if (isnan(tempOut) || isnan(humOut)) return false;
  if (tempOut < -40.0f || tempOut > 80.0f) return false;
  if (humOut < 0.0f || humOut > 100.0f) return false;
  lastValidMs = millis();
  return true;
}

void sensorsPush(float t, float h) {
  tempBuf[bufIdx] = t; humBuf[bufIdx] = h;
  bufIdx = (bufIdx + 1) % BUFFER_SIZE;
  if (bufCount < BUFFER_SIZE) bufCount++;
}

static float avgOf(float *buf) {
  if (bufCount == 0) return NAN;
  float sum = 0.0f; int n = 0;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    if (!isnan(buf[i])) { sum += buf[i]; n++; }
  }
  return n == 0 ? NAN : sum / n;
}

float sensorsAvgTemp() { return avgOf(tempBuf); }
float sensorsAvgHum()  { return avgOf(humBuf); }

float sensorsHSI() {
  float t = sensorsAvgTemp(), h = sensorsAvgHum();
  if (isnan(t) || isnan(h)) return NAN;
  return t + 0.1f * (h - 40.0f);
}

unsigned long sensorsLastValidMs() { return lastValidMs; }
