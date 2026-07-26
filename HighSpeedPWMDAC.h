#ifndef HIGHSPEEDPWM_DAC_H
#define HIGHSPEEDPWM_DAC_H

#include <Arduino.h>

class HighSpeedPWMDAC {
public:
  // Default pin is 9 (OC1A) because this library uses Timer1.
  explicit HighSpeedPWMDAC(uint8_t pin = 9);

  // Reserve Timer1 and configure the pin. Call in setup().
  void begin();

  // Set output by voltage (0.0 .. vcc). Values outside range are clamped.
  void setVoltage(float volts);

  // Set raw PWM value (0..1023).
  void setRaw(uint16_t value);

  // Release Timer1 (disconnect OC1A). Use with care — affects Timer1 globally.
  void release();

  // Getters
  uint8_t getPin() const { return _pin; }
  float getVcc() const { return _vcc; }

  static const uint16_t kMaxValue = 1023;

private:
  uint8_t _pin;
  float _vcc;
};

#endif // HIGHSPEEDPWM_DAC_H
