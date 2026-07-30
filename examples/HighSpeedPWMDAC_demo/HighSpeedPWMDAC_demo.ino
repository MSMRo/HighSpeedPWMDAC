#include <HighSpeedPWMDAC.h>

// Use pin 9 by default (OC1A). The library reserves Timer1.
HighSpeedPWMDAC dac(9);

void setup() {
  Serial.begin(115200);
  dac.begin();
}

void loop() {
  // Example: read A3 and output same voltage on the pseudo-DAC
  float v = analogRead(A3) * (5.0 / 1023.0);
  dac.setVoltage(v);

  Serial.print("Set voltage: ");
  Serial.print(v);
  Serial.println(" V");

  delay(500);
}
