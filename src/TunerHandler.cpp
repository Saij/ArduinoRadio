#include "TunerHandler.h"
#include "Debug.h"
#include "SettingsHandler.h"
#include <Arduino.h>
#include <Wire.h>

uint16_t TunerHandler::_registers[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

void _write16(uint16_t val) {
	Wire.write(val >> 8); 
	Wire.write(val & 0xFF);
}

uint16_t _read16() {
	uint8_t hiByte = Wire.read();
	uint8_t loByte = Wire.read();
	return 256 * hiByte + loByte;
}

void TunerHandler::setup() {
	debugPrintf(F("Initialize Tuner"));
}

void TunerHandler::_saveRegisters() {
	debugPrintf(F("Saving Tuner registers"));
}

void TunerHandler::_readRegisters() {
	debugPrintf(F("Reading Tuner registers"));
}

void TunerHandler::setFrequency(uint16_t newFreq) {
	newFreq = constrain(newFreq, FREQ_LIMIT_LOW, FREQ_LIMIT_HIGH);
	debugPrintf(F("Setting frequency to %d"), newFreq);
}

void TunerHandler::setVolume(uint8_t newVolume) {
	newVolume = constrain(newVolume, 0, 15);
	debugPrintf(F("Setting Tuner Volume: %02X"), newVolume);
}