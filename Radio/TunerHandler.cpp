#include "TunerHandler.h"
#include "Debug.h"
#include <Arduino.h>
#include <Wire.h>

// I2C-Address RDA Chip for sequential  Access
#define I2C_SEQ_ADDRESS  0x10

// I2C-Address RDA Chip for Index Access
#define I2C_INDX_ADDRESS  0x11

// Chip ID Register
#define RADIO_REG_CHIPID  0x00

// Control Register
#define RADIO_REG_CTRL    	  0x02
#define RADIO_REG_CTRL_OUTPUT 0x8000
#define RADIO_REG_CTRL_UNMUTE 0x4000
#define RADIO_REG_CTRL_MONO   0x2000
#define RADIO_REG_CTRL_BASS   0x1000
#define RADIO_REG_CTRL_SEEKUP 0x0200
#define RADIO_REG_CTRL_SEEK   0x0100
#define RADIO_REG_CTRL_RDS    0x0008
#define RADIO_REG_CTRL_NEW    0x0004
#define RADIO_REG_CTRL_RESET  0x0002
#define RADIO_REG_CTRL_ENABLE 0x0001

// Channel Register
#define RADIO_REG_CHAN    			0x03
#define RADIO_REG_CHAN_SPACE     	0x0003
#define RADIO_REG_CHAN_SPACE_100 	0x0000
#define RADIO_REG_CHAN_BAND     	0x000C
#define RADIO_REG_CHAN_BAND_FM      0x0000
#define RADIO_REG_CHAN_BAND_FMWORLD 0x0008
#define RADIO_REG_CHAN_TUNE   		0x0010

// Register 0x04
#define RADIO_REG_R4    		0x04
#define RADIO_REG_R4_EM50   	0x0800
#define RADIO_REG_R4_SOFTMUTE   0x0200
#define RADIO_REG_R4_AFC   		0x0100

// Volume Register
#define RADIO_REG_VOL     			0x05
#define RADIO_REG_VOL_INT			0x8000

uint16_t TunerHandler::_registers[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

// Set default band and frequency limits
RADIO_FREQ TunerHandler::_freqLow = 8700;
RADIO_FREQ TunerHandler::_freqHigh = 10800;
RADIO_FREQ TunerHandler::_freqSteps = 10;

void _write16(uint16_t val) {
	Wire.write(val >> 8); 
	Wire.write(val & 0xFF);
}

void TunerHandler::setupTuner() {
	debugPrintf(F("Initialize Tuner"));

	Wire.begin();

    // initialize all registers
    TunerHandler::_registers[RADIO_REG_CHIPID] = 0x5804;
    TunerHandler::_registers[RADIO_REG_CTRL] = (RADIO_REG_CTRL_RESET | RADIO_REG_CTRL_ENABLE);
    TunerHandler::_registers[RADIO_REG_CHAN] = (RADIO_REG_CHAN_BAND_FM | RADIO_REG_CHAN_SPACE_100);
    TunerHandler::_registers[RADIO_REG_R4] = RADIO_REG_R4_EM50 | RADIO_REG_R4_SOFTMUTE;
    TunerHandler::_registers[RADIO_REG_VOL] = (RADIO_REG_VOL_INT | 0x0800 | 0x000F); // INT Mode, Seek threshold default, Volume 100%

    TunerHandler::_saveRegisters();
    TunerHandler::_registers[RADIO_REG_CTRL] = RADIO_REG_CTRL_ENABLE;
    TunerHandler::_saveRegister(RADIO_REG_CTRL);
}

// Save writable registers back to the chip
// The registers 02 through 06, containing the configuration
// using the sequential write access mode.
void TunerHandler::_saveRegisters() {
	debugPrintf(F("Saving Tuner registers"));
  	
  	Wire.beginTransmission(I2C_SEQ_ADDRESS);
  	
  	for (int i = 2; i <= 6; i++) {
  		debugPrintf(F("Register 0x%02X: 0x%04X"), i, TunerHandler::_registers[i]);
  		_write16(TunerHandler::_registers[i]);
  	}

	Wire.endTransmission();
}

// Save one register back to the chip
void TunerHandler::_saveRegister(uint8_t regNr) {
	debugPrintf(F("Saving Tuner register 0x%02X: 0x%04X"), regNr, TunerHandler::_registers[regNr]);

  	Wire.beginTransmission(I2C_INDX_ADDRESS);
  	Wire.write(regNr);
  	_write16(TunerHandler::_registers[regNr]);
  	Wire.endTransmission();
}

void TunerHandler::setFrequency(RADIO_FREQ newFreq) {
	
}