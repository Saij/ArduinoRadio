#ifndef __TUNER_HANDLER_H__
#define __TUNER_HANDLER_H__

#include <Arduino.h>

#define FREQ_LIMIT_LOW 	8750
#define FREQ_LIMIT_HIGH 10800
#define FREQ_SPACING	10

class TunerHandler {
	public:
		static void setup();
		static void update();
		static void setFrequency(uint16_t newFrequency);
		static uint16_t getFrequency();
		static void setVolume(uint16_t newVolume);
		
	private:
		static void _saveRegisters();
		static void _readRegisters();
		static void _waitEnd();

		// memory representation of the registers
		static uint16_t _registers[];
};

#endif // __TUNER_HANDLER_H__