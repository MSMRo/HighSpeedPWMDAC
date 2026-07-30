#include <Arduino.h>
#include <avr/pgmspace.h>

#include <HighSpeedPWMDAC.h>
#include "signal.h"

// PWM DAC en el pin 9
HighSpeedPWMDAC dac(9);

// Periodo de muestreo: 1 kHz
const uint32_t SAMPLE_PERIOD_US = 1000;

uint32_t lastSample = 0;
uint16_t indexSignal = 0;

void setup()
{
    dac.begin();
}

void loop()
{
    uint32_t now = micros();

    if (now - lastSample >= SAMPLE_PERIOD_US)
    {
        lastSample += SAMPLE_PERIOD_US;

        // Leer muestra desde PROGMEM
        uint8_t sample = pgm_read_byte(&ecgSignal[indexSignal]);

        // Convertir 0-255 a 0-5V
        float voltage = (sample / 255.0f) * 5.0f;

        // Salida DAC PWM
        dac.setVoltage(voltage);

        // Avanzar al siguiente dato
        indexSignal++;

        if (indexSignal >= SIGNAL_LENGTH)
            indexSignal = 0;
    }
}