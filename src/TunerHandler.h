#ifndef __TUNER_HANDLER_H__
#define __TUNER_HANDLER_H__

#include <Arduino.h>

typedef uint16_t RADIO_FREQ;

class TunerHandler {
	public:
		static void setupTuner();
		static void setFrequency(RADIO_FREQ newFreq);
		
	private:
		TunerHandler() {};
		static void _saveRegisters();
		static void _saveRegister(uint8_t regNr);

		// memory representation of the registers
		static uint16_t _registers[];

		// Frequency limits
		static RADIO_FREQ _freqLow;
  		static RADIO_FREQ _freqHigh;
  		static RADIO_FREQ _freqSteps;
};

#endif // __TUNER_HANDLER_H__
