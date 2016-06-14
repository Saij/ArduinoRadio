#ifndef __TUNER_HANDLER_H__
#define __TUNER_HANDLER_H__

#include <Arduino.h>

#define FREQ_LIMIT_LOW 	8700
#define FREQ_LIMIT_HIGH 10800
#define FREQ_SPACING	10

class TunerHandler {
	public:
		static void setup();
		static void setFrequency(uint16_t newFreq);
		static void setVolume(uint8_t newVolume);
		
	private:
		static void _saveRegisters();
		static void _readRegisters();

		// memory representation of the registers
		static uint16_t _registers[];
};

#endif // __TUNER_HANDLER_H__
