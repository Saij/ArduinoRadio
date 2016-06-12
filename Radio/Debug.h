#ifdef DEBUG

void DEBUG_SETUP() {
  Serial.begin(38400);
  Serial.println(F("Radio - Debug"));
  Serial.println(F("-------------"));
}

#define DEBUG_PRINT(str)    Serial.print(str);
#define DEBUG_PRINTLN(str)  Serial.println(str);

#else

#define DEBUG_SETUP()
#define DEBUG_PRINT(str)
#define DEBUG_PRINTLN(str)

#endif // DEBUG
