#ifndef __SETTINGS_HANDLER_H__
#define __SETTINGS_HANDLER_H__

#include <Arduino.h>

// Config version code (magic code)
// 0x41 => A (fixed)
// 0x52 => R (fixed)
// 0x00 => 0 (version)
// 0x00 => 1 (version)
#define CONFIG_VERSION 0x41520002

#define NUM_PRESETS 6

typedef struct {
  uint32_t version;
  uint8_t brightness;
  uint16_t presets[NUM_PRESETS];
} Settings;

class SettingsHandler {
  public:
    static void setupSettings();
    static bool loadSettings();
    static void saveSettings();
    static Settings* getSettings();

  private:
    SettingsHandler() {};
    static Settings _settings;
    static int _address;
};

#endif // __SETTINGS_HANDLER_H__
