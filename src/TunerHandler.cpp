#include "TunerHandler.h"
#include "Debug.h"
#include "SettingsHandler.h"
#include <Arduino.h>
#include <Wire.h>

#define SI4703_ADR 0x10

#define PIN_RESET 6
#define PIN_SDIO  A4
#define PIN_SCLK  A5
#define PIN_MUTE   0

#define REG_POWERCFG				0x02
#define REG_POWERCFG_DSMUTE			0x8000
#define REG_POWERCFG_DMUTE			0x4000
#define REG_POWERCFG_MONO			0x2000
#define REG_POWERCFG_RDSM			0x0800
#define REG_POWERCFG_SKMODE			0x0400
#define REG_POWERCFG_SEEKUP			0x0200
#define REG_POWERCFG_SEEK			0x0100
#define REG_POWERCFG_DISABLE		0x0040
#define REG_POWERCFG_ENABLE			0x0001

#define REG_CHANNEL					0x03
#define REG_CHANNEL_TUNE			0x8000
#define REG_CHANNEL_CHANNEL_MASK	0x03FF

#define REG_SYSCFG1					0x04
#define REG_SYSCFG1_RDSIEN 			0x8000
#define REG_SYSCFG1_STCIEN 			0x4000
#define REG_SYSCFG1_RDS 			0x1000
#define REG_SYSCFG1_DE				0x0800
#define REG_SYSCFG1_AGCD			0x0400

#define REG_SYSCFG2					0x05
#define REG_SYSCFG2_SPACE_200		0x0000
#define REG_SYSCFG2_SPACE_100		0x0010
#define REG_SYSCFG2_SPACE_50		0x0020
#define REG_SYSCFG2_VOL_MASK		0x000F
#define REG_SYSCFG2_SEEKTH_MASK		0xFF00

#define REG_SYSCFG3					0x06
#define REG_SYSCFG3_VOLEXT			0x0100
#define REG_SYSCFG3_SKSNR_MASK		0x00F0
#define REG_SYSCFG3_SKCNT_MASK		0x000F

#define REG_STATUSRSSI				0x0A
#define REG_STATUSRSSI_STC			0x4000

#define REG_READCHAN				0x0B
#define REG_READCHAN_READCHAN_MASK	0x03FF

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

	// To get the Si4703 inito 2-wire mode, SEN needs to be high and SDIO needs to be low after a reset
	// The breakout board has SEN pulled high, but also has SDIO pulled high. Therefore, after a normal power up
	// The Si4703 will be in an unknown state. RST must be controlled

	pinMode(PIN_MUTE, OUTPUT);
	digitalWrite(PIN_MUTE, LOW);

	pinMode(PIN_RESET, OUTPUT);
	pinMode(PIN_SDIO, OUTPUT);
	digitalWrite(PIN_SDIO, LOW);
	digitalWrite(PIN_RESET, LOW);
	delay(1);

	digitalWrite(PIN_RESET, HIGH);
	delay(1);

	// Now that the unit is reset and I2C inteface mode, we need to begin I2C
	Wire.begin();

	// Setting oscillator
  	TunerHandler::_readRegisters();
	TunerHandler::_registers[0x07] = 0x8100; //Enable the oscillator, from AN230 page 12, rev 0.9
  	TunerHandler::_saveRegisters();
	delay(500); //Wait for clock to settle - from AN230 page 12
	TunerHandler::_readRegisters();

	// Enable the IC and disable mute
  	TunerHandler::_registers[REG_POWERCFG] = REG_POWERCFG_DMUTE | REG_POWERCFG_ENABLE;
	TunerHandler::_registers[REG_SYSCFG1] |= REG_SYSCFG1_RDS; // Enable RDS

	// 100kHz Europe setup
  	TunerHandler::_registers[REG_SYSCFG1] |= REG_SYSCFG1_DE; 
  	TunerHandler::_registers[REG_SYSCFG2] |= REG_SYSCFG2_SPACE_100;

  	TunerHandler::_registers[REG_SYSCFG2] &= ~(REG_SYSCFG2_VOL_MASK | REG_SYSCFG2_SEEKTH_MASK);
  	TunerHandler::_registers[REG_SYSCFG2] |= 0x1001; // Set volume to low and medium Seek RSSI Threshold

  	TunerHandler::_registers[REG_SYSCFG3] &= ~(REG_SYSCFG3_SKSNR_MASK | REG_SYSCFG3_SKCNT_MASK);
  	TunerHandler::_registers[REG_SYSCFG3] |= 0x0033; // Medium Seek SNR Threshold and medium Seek Impulse Detection Threshold

	TunerHandler::_saveRegisters();
	delay(110); // Max powerup time

	TunerHandler::_readRegisters();
	TunerHandler::setFrequency(SettingsHandler::getLastFrequency());
	TunerHandler::setVolume(SettingsHandler::getVolume());
}

void TunerHandler::update() {
	TunerHandler::_readRegisters();
}

void TunerHandler::_saveRegisters() {
	debugPrintf(F("TunerHandler::_saveRegisters"));

	Wire.beginTransmission(SI4703_ADR);
	
	// A write command automatically begins with register 0x02 so no need to send a write-to address
	// First we send the 0x02 to 0x07 control registers
	// In general, we should not write to registers 0x08 and 0x09
  	for (uint8_t regSpot = 0x02; regSpot < 0x08; regSpot++) {
  		_write16(TunerHandler::_registers[regSpot]);
	}

  	Wire.endTransmission();
}

void TunerHandler::_readRegisters() {
	//debugPrintf(F("TunerHandler::_readRegisters"));

	// Si4703 begins reading from register upper register of 0x0A and reads to 0x0F, then loops to 0x00.
	// We want to read the entire register set from 0x0A to 0x09 = 32 bytes.
	Wire.requestFrom(SI4703_ADR, 32);

	// Wait for 16 words/32 bytes to come back from slave I2C device
	// @TODO implement timeout
  	while (Wire.available() < 32); 

  	// Remember, register 0x0A comes in first so we have to shuffle the array around a bit
  	for (uint8_t x = 0x0A;; x++) {
    	// Loop back to zero
    	if (x == 0x10) {
    		x = 0;
    	}

    	TunerHandler::_registers[x] = _read16();

    	if (x == 0x09) {
    		break;
    	}
    }
}

uint16_t TunerHandler::getFrequency() {
	//debugPrintf(F("TunerHandler::_getFrequency"));
	uint16_t channel = TunerHandler::_registers[REG_READCHAN] & REG_READCHAN_READCHAN_MASK;
	return (channel * FREQ_SPACING) + FREQ_LIMIT_LOW;
} 

void TunerHandler::setFrequency(uint16_t newFrequency) {
	newFrequency = constrain(newFrequency, FREQ_LIMIT_LOW, FREQ_LIMIT_HIGH);
	debugPrintf(F("TunerHandler::setFrequency: %d"), newFrequency);
	
	uint16_t channel = (newFrequency - FREQ_LIMIT_LOW) / FREQ_SPACING;
	TunerHandler::_registers[REG_CHANNEL] &= ~REG_CHANNEL_CHANNEL_MASK;
	TunerHandler::_registers[REG_CHANNEL] |= REG_CHANNEL_TUNE | channel;

	TunerHandler::_saveRegisters();
	TunerHandler::_waitEnd();
}

void TunerHandler::_waitEnd() {
	debugPrintf(F("TunerHandler::_waitEnd"));

	// wait until STC gets high
	do {
		TunerHandler::_readRegisters();
	} while ((TunerHandler::_registers[REG_STATUSRSSI] & REG_STATUSRSSI_STC) == 0);
  
  	// end the seek mode
  	TunerHandler::_registers[REG_POWERCFG] &= ~REG_POWERCFG_SEEK;
  	TunerHandler::_registers[REG_CHANNEL] &= ~REG_CHANNEL_TUNE;
	TunerHandler::_saveRegisters();

	// wait until STC gets down again
	do {
		TunerHandler::_readRegisters();
	} while ((TunerHandler::_registers[REG_STATUSRSSI] & REG_STATUSRSSI_STC) != 0);
}

void TunerHandler::setVolume(uint16_t newVolume) {
	debugPrintf(F("TunerHandler::setVolume: %d"), newVolume);
	
	newVolume = constrain(newVolume, 0, 30);

	if (newVolume > 15) {
		// Normal volume
		TunerHandler::_registers[REG_SYSCFG3] &= ~REG_SYSCFG3_VOLEXT;
		newVolume -= 15;
		debugPrintf(F("Normal Volume: %d"), newVolume);
	} else {
		// Extended volume
		TunerHandler::_registers[REG_SYSCFG3] |= REG_SYSCFG3_VOLEXT;
		debugPrintf(F("Extended Volume: %d"), newVolume);
	}
	TunerHandler::_registers[REG_SYSCFG2] &= ~REG_SYSCFG2_VOL_MASK;
	TunerHandler::_registers[REG_SYSCFG2] |= newVolume;

	if (newVolume > 0) {
		digitalWrite(PIN_MUTE, HIGH);
	} else {
		digitalWrite(PIN_MUTE, LOW);
	}

	TunerHandler::_saveRegisters();
}