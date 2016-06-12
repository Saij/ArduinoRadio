#include "SettingsHandler.h"
#include "Debug.h"
#include <EEPROMex.h>

#define BASE_ADDRESS 32

Settings SettingsHandler::_settings = {
  CONFIG_VERSION,

  // Brightness
  127, 

  // Presets
  {
    8770,
    8950,
    9410,
    9970,
    10110,
    10770
  }
};

int SettingsHandler::_address = BASE_ADDRESS;

void SettingsHandler::setupSettings() {
  debugPrintf(F("Initialize Settings"));
  
  EEPROM.setMemPool(BASE_ADDRESS, EEPROMSizeUno);
  SettingsHandler::_address  = EEPROM.getAddress(sizeof(Settings)); // Size of config object

  if (!SettingsHandler::loadSettings()) {
    debugPrintf(F("Saving default values"));
    SettingsHandler::saveSettings();
  }
}

bool SettingsHandler::loadSettings() {
  Settings loadedSettings;
  debugPrintf(F("Loading settings"));

  EEPROM.readBlock(SettingsHandler::_address, loadedSettings);
  if (loadedSettings.version == CONFIG_VERSION) {
    debugPrintf(F("Settings loaded"));
    SettingsHandler::_settings = loadedSettings;
    return true;
  }

  debugPrintf(F("Settings could not be loaded"));
  return false;
}

void SettingsHandler::saveSettings() {
  debugPrintf(F("Saving settings"));
  EEPROM.writeBlock(SettingsHandler::_address, SettingsHandler::_settings);
}

Settings* SettingsHandler::getSettings() {
  return &SettingsHandler::_settings;
}

