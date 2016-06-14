#include "SettingsHandler.h"
#include "Debug.h"
#include <EEPROMex.h>

#define NUM_REGISTERS 8

#define MAGIC_CODE 0x4152
#define VERSION 0x000B

#define REG_BRIGHTNESS	0x02
#define MASK_BRIGHTNESS 0x00FF

#define REG_SOFTMUTE	0x02
#define MASK_SOFTMUTE	0x0100
#define POS_SOFTMUTE	0x08

#define REG_VOLUME		0x02
#define MASK_VOLUME		0x3E00
#define POS_VOLUME		0x09

#define REG_LAST_FREQ	0x03

#define REG_PRESETS		0x04
#define NUM_PRESETS		4

#define SAVE_TIME_THRESHOLD 3000 // 3 Seconds

uint8_t SettingsHandler::_varSize = ((NUM_REGISTERS << 1) | 1);
uint8_t SettingsHandler::_bufferLen = (EEPROMSizeATmega328 / SettingsHandler::_varSize);
uint16_t SettingsHandler::_statusBufferAddress = (EEPROMSizeATmega328 - SettingsHandler::_bufferLen);

uint16_t SettingsHandler::_settings[NUM_REGISTERS] = {
	MAGIC_CODE,
	VERSION,
	0x307F, // Volume 24, Softmute, Brightess 127
	0x22F6, // Last Frequency 8950
	0x2242, // Preset 1 8770
	0x22F6, // Preset 2 8950
	0x277E, // Preset 3 10110
	0x2A12, // Preset 4 10770
};

unsigned long SettingsHandler::_lastChange = 0;

void SettingsHandler::setup() {
	debugPrintf(F("Initialize Settings"));

	debugPrintf(F("\tVAR_SIZE: %02X"), SettingsHandler::_varSize);
	debugPrintf(F("\tBUFFER_LEN: %02X"), SettingsHandler::_bufferLen);
	debugPrintf(F("\tSTATUS_BUFFER_ADDRESS: %04X"), SettingsHandler::_statusBufferAddress);

	SettingsHandler::_loadSettings();
}

void SettingsHandler::setBrightness(uint8_t newBrightness) {
	if (SettingsHandler::getBrightness() != newBrightness) {
		// Update brightness
		SettingsHandler::_settings[REG_BRIGHTNESS] &= ~MASK_BRIGHTNESS;
		SettingsHandler::_settings[REG_BRIGHTNESS] |= newBrightness;
		SettingsHandler::_lastChange = millis();
	}
}

uint8_t SettingsHandler::getBrightness() {
	return SettingsHandler::_settings[REG_BRIGHTNESS] & MASK_BRIGHTNESS;
}

void SettingsHandler::setSoftmute(uint8_t newSoftmute) {
	newSoftmute = constrain(newSoftmute, 0, 1);
	if (SettingsHandler::getSoftmute() != newSoftmute) {
		// Update softmute
		SettingsHandler::_settings[REG_SOFTMUTE] &= ~MASK_SOFTMUTE;
		SettingsHandler::_settings[REG_SOFTMUTE] |= (newSoftmute << POS_SOFTMUTE);
		SettingsHandler::_lastChange = millis();
	}
}

uint8_t SettingsHandler::getSoftmute() {
	return (SettingsHandler::_settings[REG_SOFTMUTE] & MASK_SOFTMUTE) >> POS_SOFTMUTE;
}

void SettingsHandler::setVolume(uint16_t newVolume) {
	newVolume = constrain(newVolume, 0, 31);
	if (SettingsHandler::getVolume() != newVolume) {
		// Update volume
		SettingsHandler::_settings[REG_VOLUME] &= ~MASK_VOLUME;
		SettingsHandler::_settings[REG_VOLUME] |= (newVolume << POS_VOLUME);
		SettingsHandler::_lastChange = millis();
	}
}

uint16_t SettingsHandler::getVolume() {
	return (SettingsHandler::_settings[REG_VOLUME] & MASK_VOLUME) >> POS_VOLUME;
}

void SettingsHandler::setLastFrequency(uint16_t newFrequency) {
	if (SettingsHandler::_settings[REG_LAST_FREQ] != newFrequency) {
		// Update last frequency
		SettingsHandler::_settings[REG_LAST_FREQ] = newFrequency;
		SettingsHandler::_lastChange = millis();
	}
}

uint16_t SettingsHandler::getLastFrequency() {
	return SettingsHandler::_settings[REG_LAST_FREQ];
}

void SettingsHandler::setPresetFrequency(uint8_t preset, uint16_t newFrequency) {
	preset = constrain(preset, 1, NUM_PRESETS) - 1;
	if (SettingsHandler::_settings[REG_PRESETS + preset] != newFrequency) {
		// Update preset frequency
		SettingsHandler::_settings[REG_PRESETS + preset] = newFrequency;
		SettingsHandler::_lastChange = millis();
	}
}

uint16_t SettingsHandler::getPresetFrequency(uint8_t preset) {
	preset = constrain(preset, 1, NUM_PRESETS) - 1;
	return SettingsHandler::_settings[REG_PRESETS + preset];
}

void SettingsHandler::update() {
	if (SettingsHandler::_lastChange > 0 && (millis() - SettingsHandler::_lastChange) > SAVE_TIME_THRESHOLD) {
		SettingsHandler::_saveSettings();
		SettingsHandler::_lastChange = 0;
	}
}

uint16_t SettingsHandler::_findNextWriteIndex() {	
	debugPrintf(F("_findNextWriteIndex"));

	uint16_t i;
	for (i = SettingsHandler::_statusBufferAddress; i < (SettingsHandler::_bufferLen + SettingsHandler::_statusBufferAddress); i++) {
    	uint16_t prevIndex;
    
    	// Wrap around case
    	if (i == SettingsHandler::_statusBufferAddress) {
      		prevIndex = SettingsHandler::_statusBufferAddress + SettingsHandler::_bufferLen - 1;
    	} else {
      		prevIndex = i - 1;
      	}

    	uint8_t prevElem = EEPROM.readByte(prevIndex);
    	uint8_t currElem = EEPROM.readByte(i);

    	// Must truncate the addition because the index tracking relies of wrap around
    	if (((prevElem + 1) & 0xFF) != currElem) {
			debugPrintf(F("\tprevIndex: %04X\n\tprevElem: %04X\n\tcurrIndex: %04X\n\tcurrElem: %04X"), prevIndex, prevElem, i, currElem);
      		      
      		return i - SettingsHandler::_statusBufferAddress;
    	}
	}

	return 0;
}

void SettingsHandler::_loadSettings() {
	debugPrintf(F("_loadSettings"));

	uint16_t writeOffset = SettingsHandler::_findNextWriteIndex() * SettingsHandler::_varSize;
	uint16_t readAddress;

	debugPrintf(F("\tWrite Offset: %04X"), writeOffset);

	readAddress = writeOffset - SettingsHandler::_varSize;
	if (writeOffset == 0) {
		readAddress = SettingsHandler::_statusBufferAddress - SettingsHandler::_varSize;
	}

	debugPrintf(F("\tRead Address: %04X"), readAddress);

	uint16_t magicCode = EEPROM.readInt(readAddress);
	uint16_t version = EEPROM.readInt(readAddress + 0x2);

	debugPrintf(F("\tMagic Code: %04X\n\tVersion: %04X"), magicCode, version);

	if (magicCode == SettingsHandler::_settings[0x0] && version == SettingsHandler::_settings[0x1]) {
		debugPrintf(F("\tLoading settings"));
		EEPROM.readBlock<uint16_t>(readAddress, SettingsHandler::_settings, NUM_REGISTERS);
	}
}

void SettingsHandler::_saveSettings() {
	debugPrintf(F("_saveSettings"));

	uint16_t writeOffset = SettingsHandler::_findNextWriteIndex();
	debugPrintf(F("\tWrite Offset: %04X"), writeOffset);

	EEPROM.updateBlock<uint16_t>(writeOffset * SettingsHandler::_varSize, SettingsHandler::_settings, NUM_REGISTERS);

	// Update status buffer 
    uint16_t currIndex = SettingsHandler::_statusBufferAddress + writeOffset;
    uint16_t prevIndex;

    // Wrap around case
    if (currIndex == SettingsHandler::_statusBufferAddress) {
		prevIndex = SettingsHandler::_statusBufferAddress + SettingsHandler::_bufferLen - 1;
    } else {
		prevIndex = currIndex - 1;
    }

    uint16_t sbVal = EEPROM.readByte(prevIndex) + 1;
    debugPrintf(F("\tCurr Index: %04X\n\tPrev Index: %04X\n\tNew Val: %04X"), currIndex, prevIndex, sbVal);

    EEPROM.updateByte(currIndex, sbVal);
}