#ifndef EasySwitch_sensors_h
#define EasySwitch_senosrs_h
#include <EEPROM.h>
#include "Arduino.h"
#include <avr/pgmspace.h> 
#include <Wire.h>    
#include "Adafruit_MCP23017.h"
const PROGMEM uint16_t sensorId_t[] = {
101,101,101,101,101,101,121,121,116,116,126,126,126,126,105,107,109,110,111,111,113,102,117,117,117,123,125,125,128,128,130,130,130,888,999,100,103,104,106,108,112,114,115,118,119,120,122,124,127,129,131,};

const PROGMEM uint16_t relay_t[] = {
128,128,128,128,128,128,148,148,143,143,153,153,153,153,132,134,136,137,138,138,140,129,144,144,144,150,152,152,155,155,157,157,157,243,38,127,130,131,133,135,139,141,142,145,146,147,149,149,154,156,258,};

const PROGMEM uint8_t relayOptions_t[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

const PROGMEM uint8_t relayStartup_t[] = {
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,};

const PROGMEM uint8_t relayType_t[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

const PROGMEM int button_t[] = {
29,28,36,37,27,26,24,30,25,31,22,23,33,32,45,20,47,46,42,43,40,41,53,52,44,21,49,48,50,51,35,19,34,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};

const PROGMEM uint16_t button_type_t[] = {
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,};

const PROGMEM int button_next_t[] = {
29,28,36,37,27,26,24,30,25,31,22,23,33,32,45,20,47,46,42,43,40,41,53,52,44,21,49,48,50,51,35,19,34,17,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};

const PROGMEM uint16_t button_click_t[] = {
3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};

const PROGMEM byte one_button_or_more_t[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};

const char s_0[] PROGMEM = "salon_kinkiet";
const char s_1[] PROGMEM = "salon_kinkiet";
const char s_2[] PROGMEM = "salon_kinkiet";
const char s_3[] PROGMEM = "salon_kinkiet";
const char s_4[] PROGMEM = "salon_kinkiet";
const char s_5[] PROGMEM = "salon_kinkiet";
const char s_6[] PROGMEM = "salon_nad_stolem";
const char s_7[] PROGMEM = "salon_nad_stolem";
const char s_8[] PROGMEM = "kuchnia_bar";
const char s_9[] PROGMEM = "kuchnia_bar";
const char s_10[] PROGMEM = "kuchnia_kinkiet";
const char s_11[] PROGMEM = "kuchnia_kinkiet";
const char s_12[] PROGMEM = "kuchnia_kinkiet";
const char s_13[] PROGMEM = "kuchnia_kinkiet";
const char s_14[] PROGMEM = "spizarka";
const char s_15[] PROGMEM = "wejscie";
const char s_16[] PROGMEM = "pokoj_W";
const char s_17[] PROGMEM = "pokoj_W";
const char s_18[] PROGMEM = "sypialnia";
const char s_19[] PROGMEM = "sypialnia";
const char s_20[] PROGMEM = "lazienka";
const char s_21[] PROGMEM = "lazienka";
const char s_22[] PROGMEM = "korytarz";
const char s_23[] PROGMEM = "korytarz";
const char s_24[] PROGMEM = "korytarz";
const char s_25[] PROGMEM = "Kanciapa";
const char s_26[] PROGMEM = "pokoj_E";
const char s_27[] PROGMEM = "pokoj_E";
const char s_28[] PROGMEM = "pokoj_N";
const char s_29[] PROGMEM = "pokoj_N";
const char s_30[] PROGMEM = "wiatrolap";
const char s_31[] PROGMEM = "wiatrolap";
const char s_32[] PROGMEM = "wiatrolap";
const char s_33[] PROGMEM = "dzwonek";
const char s_34[] PROGMEM = "gniazdko_taras";
const char s_35[] PROGMEM = "";
const char s_36[] PROGMEM = "";
const char s_37[] PROGMEM = "";
const char s_38[] PROGMEM = "";
const char s_39[] PROGMEM = "";
const char s_40[] PROGMEM = "";
const char s_41[] PROGMEM = "";
const char s_42[] PROGMEM = "";
const char s_43[] PROGMEM = "";
const char s_44[] PROGMEM = "";
const char s_45[] PROGMEM = "";
const char s_46[] PROGMEM = "";
const char s_47[] PROGMEM = "";
const char s_48[] PROGMEM = "";
const char s_49[] PROGMEM = "";
const char s_50[] PROGMEM = "";

const char *const relayDescription[] PROGMEM = {
  s_0,  s_1,  s_2,  s_3,  s_4,  s_5,  s_6,  s_7,  s_8,  s_9,  s_10,  s_11,  s_12,  s_13,  s_14,  s_15,  s_16,  s_17,  s_18,  s_19,  s_20,  s_21,  s_22,  s_23,  s_24,  s_25,  s_26,  s_27,  s_28,  s_29,  s_30,  s_31,  s_32,  s_33,  s_34,  s_35,  s_36,  s_37,  s_38,  s_39,  s_40,  s_41,  s_42,  s_43,  s_44,  s_45,  s_46,  s_47,  s_48,  s_49,  s_50,};

class EasySwitch_sensors {
public:
	EasySwitch_sensors();
	void begin();
	void sendPresentation(String version , String name);
	void sendState(int id, bool state, bool ack);
	void sendPresentationId();
	void sendAllState();
	void saveState(int relay, byte state);
	void checkEEprom();
	void EEpromSetAllZero();
	int readState(int relay); 
	void setAllPinMode();
	void checking();
	void createButton(int pin, int id);
	void Click(int i, int button, byte click);
	void buttonType3(int i, int button);
	void buttonType2(int i, int button);
	uint16_t check();
	uint8_t getState();
	void SetOutputArdu(int pin, bool state);
	void SetOutputMCP(int pin, bool state);
	void MCP_begin();
	int GetMcpEx(int in); 
	int GetMcpPin(int ex,int in); 
	bool getTrigger(int i , int relay, bool relayState);
	void loadStateAndSetOutput();
	void serialIn();
	void parseIn();
	void parseAll();
private:
	unsigned long _lastTime; 
	unsigned long _lastTime1s; 
	uint8_t _buttonTimeStart = 12; 
	uint8_t _buttonOneDebounce = 5;
	uint8_t _buttonDoubleStart = 9;
	uint8_t _buttonDoubleStop = 2;
	uint8_t _buttonDoubleDebounce = 3;
	uint8_t _buttonLongDebounce = 10;
	String _serialIn; 
	uint8_t _serialCount;
	unsigned char _frame_in[50];
	uint16_t _node_id;
	uint16_t _child_sensor_id;
	uint16_t _command;
	uint8_t _ack;
	uint16_t _type;
	uint16_t _payload;
	char _charBuf[50]; 
	uint8_t expanderAddresses[8];
	const int numberOfExpanders = sizeof(expanderAddresses);
	Adafruit_MCP23017 expander[8];
byte _eepromerrase =135;
uint8_t _buttonTime[51] ;
uint8_t _buttonState[51];
uint16_t _numberOfRelayButtons = 51;
bool _oneClickPress[51];
String g_sketch_name = "EasySwitch";
};
#endif
