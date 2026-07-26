#include "HighSpeedPWMDAC.h"

HighSpeedPWMDAC::HighSpeedPWMDAC(uint8_t pin)
  : _pin(pin), _vcc(5.0f) {
}

void HighSpeedPWMDAC::begin() {
  pinMode(_pin, OUTPUT);

  // Configure Timer1 as Fast PWM 10-bit, non-inverting output on OC1A (Pin 9)
  // WGM13:0 = 0b0011 => 10-bit Fast PWM (WGM12:WGM10 = 0b011)
  // COM1A1 = 1 non-inverting
  TCCR1A = _BV(COM1A1) | _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10); // No prescaler

  // Initialize output compare so output starts at 0
  OCR1A = 0;
}

void HighSpeedPWMDAC::setVoltage(float volts) {
  // Clamp to 0..Vcc
  if (volts < 0.0f) volts = 0.0f;
  if (volts > _vcc) volts = _vcc;

  // Map to 10-bit value and write to OCR1A
  uint16_t val = (uint16_t)((volts * (float)kMaxValue) / _vcc + 0.5f);
  OCR1A = val;
}

void HighSpeedPWMDAC::setRaw(uint16_t value) {
  if (value > kMaxValue) value = kMaxValue;
  OCR1A = value;
}

void HighSpeedPWMDAC::release() {
  // Disconnect OC1A and stop Timer1. This will affect any code that expects
  // Timer1 (e.g., tone(), servo libraries that use Timer1, and other PWM on pins 9/10).
  TCCR1A = 0;
  TCCR1B = 0;
}
