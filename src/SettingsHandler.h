#ifndef __SETTINGS_HANDLER_H__
#define __SETTINGS_HANDLER_H__

#include <Arduino.h>

class SettingsHandler {
	public:
		static void setup();
		static void update();

		static void setBrightness(uint8_t newBrightness);
		static uint8_t getBrightness();

	private:
		static uint16_t _findNextWriteIndex();
		static void _loadSettings();
		static void _saveSettings();

		static uint8_t _varSize;
		static uint8_t _bufferLen;
		static uint16_t _statusBufferAddress;
		static uint16_t _settings[];
		static unsigned long _lastChange;
};

#endif // __SETTINGS_HANDLER_H__