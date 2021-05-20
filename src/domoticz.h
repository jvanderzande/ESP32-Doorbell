#include <Arduino.h>

void Button_Check();
void Button_Pressed(const char* State);
void LedToggle();
void SetLedtoDefault(bool warn);

bool Domoticz_JSON_Switch(const char *Idx, const char* State);
bool Domoticz_MQTT_Switch(const char *Idx, const char* State);

void Motion_Check();
void Motion_Active(const char* State);

void Mqtt_begin();
bool Mqtt_Connect();
bool Mqtt_Loop();
void Mqtt_messageReceived(String &topic, String &payload);

String process_messageReceived(String payload);

extern uint Flash_done;
extern long Flash_timer;