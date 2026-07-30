#include <Arduino.h>
#include "HighSpeedPWMDAC.h"
#include <math.h>

HighSpeedPWMDAC dac(9);

// Two sine frequencies to sum
const float f1 = 10.0f;   // 10 Hz
const float f2 = 100.0f;  // 100 Hz

float phase1 = 0.0f;
float phase2 = 0.0f;

unsigned long lastMicros = 0;

#define TWO_PI 6.28318530717958647692f

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("HighSpeedPWMDAC Sum of Two Sines Example");

  dac.begin();
  lastMicros = micros();
}

void loop() {
  unsigned long now = micros();
  unsigned long dt = now - lastMicros;
  if (dt == 0) return;

  float dtSec = (float)dt / 1000000.0f;

  phase1 += TWO_PI * f1 * dtSec;
  phase2 += TWO_PI * f2 * dtSec;

  // wrap phases
  if (phase1 >= TWO_PI) phase1 = fmodf(phase1, TWO_PI);
  if (phase2 >= TWO_PI) phase2 = fmodf(phase2, TWO_PI);

  // sum the two sines and normalize to 0..1
  float s1 = sinf(phase1);
  float s2 = sinf(phase2);
  float sum = (s1 + s2) * 0.5f; // range [-1,1]
  float value01 = (sum + 1.0f) * 0.5f; // map to [0,1]

  float volts = value01 * dac.getVcc();
  dac.setVoltage(volts);

  lastMicros = now;
}
