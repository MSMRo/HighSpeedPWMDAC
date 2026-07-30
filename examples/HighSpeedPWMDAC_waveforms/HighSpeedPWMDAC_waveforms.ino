#include <Arduino.h>
#include "HighSpeedPWMDAC.h"
#include "SignalGenerator.h"

// Output on pin 9 (default)
HighSpeedPWMDAC dac(9);
SignalGenerator gen(dac, 1.0f, 2000); // default 1 Hz, 2kHz sample rate

void setup() {
  Serial.begin(115200);
  while (!Serial) ;
  Serial.println("HighSpeedPWMDAC Waveforms Example");

  gen.begin();
  gen.setFrequency(5.0f); // 5 Hz example
}

void loop() {
  // Cycle through waveforms every 5 seconds
  static unsigned long lastSwitch = 0;
  static int idx = 0;
  unsigned long now = millis();
  if (now - lastSwitch > 5000) {
    idx = (idx + 1) % 4;
    lastSwitch = now;
    switch (idx) {
      case 0: gen.setWaveform(SignalGenerator::SINE); Serial.println("SINE"); break;
      case 1: gen.setWaveform(SignalGenerator::SQUARE); Serial.println("SQUARE"); break;
      case 2: gen.setWaveform(SignalGenerator::TRIANGLE); Serial.println("TRIANGLE"); break;
      case 3: gen.setWaveform(SignalGenerator::RAMP); Serial.println("RAMP"); break;
    }
  }

  gen.update();
}
