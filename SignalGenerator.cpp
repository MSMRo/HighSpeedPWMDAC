#include "SignalGenerator.h"
#include <math.h>

#define TWO_PI 6.28318530717958647692f

SignalGenerator::SignalGenerator(HighSpeedPWMDAC &dac, float frequency, unsigned long sampleRate)
  : _dac(dac), _frequency(frequency), _sampleRate(sampleRate), _enabled(true), _phase(0.0f), _lastMicros(0), _wave(SINE) {
}

void SignalGenerator::begin() {
  _dac.begin();
  _lastMicros = micros();
}

void SignalGenerator::setFrequency(float freqHz) {
  if (freqHz < 0) freqHz = 0;
  _frequency = freqHz;
}

void SignalGenerator::setWaveform(Wave w) {
  _wave = w;
}

void SignalGenerator::setSampleRate(unsigned long sr) {
  if (sr < 10) sr = 10;
  _sampleRate = sr;
}

void SignalGenerator::setEnabled(bool en) {
  _enabled = en;
}

void SignalGenerator::update() {
  if (!_enabled) return;

  unsigned long now = micros();
  unsigned long dt = now - _lastMicros;
  if (dt == 0) return;

  // advance phase by frequency * dt (seconds) * TWO_PI
  float delta = (float)dt * (_frequency * TWO_PI) / 1000000.0f;
  _phase += delta;
  // keep phase in [0, TWO_PI)
  if (_phase >= TWO_PI) {
    // reduce by multiples of TWO_PI to avoid large numbers
    _phase = fmodf(_phase, TWO_PI);
  }

  _lastMicros = now;

  outputSample(_phase);
}

void SignalGenerator::outputSample(float phase) {
  float vcc = _dac.getVcc();
  float value = 0.0f; // 0..1

  switch (_wave) {
    case SINE: {
      value = (sinf(phase) + 1.0f) * 0.5f;
      break;
    }
    case SQUARE: {
      value = (sinf(phase) >= 0.0f) ? 1.0f : 0.0f;
      break;
    }
    case TRIANGLE: {
      // triangle from 0..1
      float t = phase / TWO_PI; // 0..1
      value = 1.0f - fabsf(2.0f * t - 1.0f);
      break;
    }
    case RAMP: {
      float t = phase / TWO_PI; // 0..1
      value = t;
      break;
    }
    default:
      value = 0.0f;
  }

  float volts = value * vcc;
  _dac.setVoltage(volts);
}
