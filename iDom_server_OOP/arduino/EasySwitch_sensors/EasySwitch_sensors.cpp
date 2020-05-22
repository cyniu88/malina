/*
2020.05.14 12:18:59
*/
#include "EasySwitch_sensors.h"
EasySwitch_sensors::EasySwitch_sensors()
{
}
void EasySwitch_sensors::begin(){
  checkEEprom();
  EEpromSetAllZero();
	Serial.begin(115200);
	MCP_begin();
	setAllPinMode(); 
  	sendPresentation("1.0", "EasySwitch");
  	sendPresentationId();
    delay(2000);
    sendAllState();
}
void EasySwitch_sensors::MCP_begin(){
	 for (int i = 0; i < numberOfExpanders; i++) {
	 	expanderAddresses[i] = i;
    	expander[i].begin(expanderAddresses[i]);
  	}
}
void EasySwitch_sensors::sendPresentation(String version , String name){
	Serial.println("0;255;3;0;14;Gateway startup complete.");
 	Serial.println("0;255;0;0;17;2.3.0");
 	Serial.print("0;255;3;0;11;");
	Serial.println(g_sketch_name);
 	Serial.print("0;255;3;0;12;");
 	Serial.println(version);
}
void EasySwitch_sensors::sendState(int id, bool state, bool ack, int buttonPin ){
	Serial.print("0;");
	Serial.print(id);
	Serial.print(';');
	Serial.print(buttonPin);
	Serial.print(";1;");
    Serial.print(ack);
    Serial.print(";2;");
	Serial.print(state);
  Serial.print('\n');
}
void EasySwitch_sensors::sendAllState(){
  int id;
  int relay;
  int state;
  for(byte i=0; i < _numberOfRelayButtons; i++){
    relay = pgm_read_word_near(relay_t + i);
    state =  readState(relay);
    id = pgm_read_word_near(sensorId_t + i);
    Serial.print("0;");
    Serial.print(id);
    Serial.print(";1;0;2;");
    Serial.println(state);
    delay(10);
  }
}
void EasySwitch_sensors::sendPresentationId(){
		int id1;
	for(byte i=0; i < _numberOfRelayButtons; i++){
		strcpy_P(_charBuf, (char *)pgm_read_word(&(relayDescription[i])));  
		id1 = pgm_read_word_near(sensorId_t + i);
		Serial.print("0;");
		Serial.print(id1);
		Serial.print(";0;0;3;");
		Serial.println(_charBuf);
	}
}
void EasySwitch_sensors::checkEEprom(){
  byte i;
  for(i=0;i<255; i++){
    if(readState(i) > 1 ){
     saveState(i, 0); 
    }
  }
}
void EasySwitch_sensors::EEpromSetAllZero(){
  byte i;
  if (_eepromerrase > 0 ){
    if(EEPROM.read(10) == _eepromerrase){
    }
    else{
      EEPROM.write(10,_eepromerrase);
      for(i=0;i<255; i++){
        if(readState(i) > 0 ){
          saveState(i, 0); 
        }
      }
    }  
  }
}
void EasySwitch_sensors::saveState(int relay, byte state){
	EEPROM.write((relay+100), state);
}
int EasySwitch_sensors::readState(int relay){
	return EEPROM.read(relay+100);
}
void EasySwitch_sensors::setAllPinMode(){
	int relay;
	int ex;
	int button;
	byte pin;
	bool relayState;
	bool trigger;
	byte state; 
	for(byte i=0; i < _numberOfRelayButtons; i++){
  		button = pgm_read_word_near(button_t + i);
  		if((button < 126) && (button >  -1 )) {
  			pinMode(button, INPUT_PULLUP);
  		}
  		if(button > 126) {
  			ex = GetMcpEx(button);
  			pin = GetMcpPin(ex,button);	
  			expander[ex].pinMode(pin, INPUT);
  			expander[ex].pullUp(pin, HIGH); 	
  		}
  	}
  	for(byte i=0; i < _numberOfRelayButtons; i++){
  		relay = pgm_read_word_near(relay_t + i);
  		state = pgm_read_byte_near(relayStartup_t + i);
  		if(state == 2){
  			relayState =  readState(relay);
  			trigger = getTrigger(i , relay, relayState);
  		}
  		if(state == 0){
  			trigger = getTrigger(i , relay, 0);
        saveState(relay,0);
  		}
  		if(state == 1){
  			trigger = getTrigger(i , relay, 1);
        saveState(relay, 1);
  		}
  		if((relay < 126) && (relay >  -1 )){
  			pinMode(relay, OUTPUT);	
  			SetOutputArdu(relay, trigger);	
  		}
  		if(relay > 126) {
  			ex = GetMcpEx(relay);
  			pin = GetMcpPin(ex,relay);	
  			expander[ex].pinMode((pin), OUTPUT);
  			SetOutputMCP(relay, trigger);
  		}
  	}
}
void EasySwitch_sensors::checking()
{
  	serialIn(); 
  	int button;
    int button_type;
	for(byte i=0; i < _numberOfRelayButtons; i++){
  		button = pgm_read_word_near(button_t + i);
      button_type = pgm_read_word_near(button_type_t + i);
      if(button_type == 3){
        buttonType3(i,button); 
      }
      if(button_type == 2){
        buttonType2(i,button); 
      }
/*
  		 Serial.print(":pin:");
  		 Serial.print(button);	
  		 Serial.print(":time:");
  		 Serial.print(_buttonTime[i]);	
  		 Serial.print(":state:");
  		 Serial.println(_buttonState[i]);	
*/
  }
  unsigned long now = millis(); 
  if(now - _lastTime > 50 ){
  	 _lastTime = now;
  	for(byte i=0; i < _numberOfRelayButtons; i++){
  		button = pgm_read_word_near(button_t + i);
  		if(_buttonTime[i] > 0){
  			_buttonTime[i] = _buttonTime[i]-1;	
  		}
  	}
  }
/*
  if(now - _lastTime1s > 1000 ){
  	 _lastTime1s = now;
  	for(byte i=0; i < _numberOfRelayButtons; i++){
  		button = pgm_read_word_near(button_t + i);
  		Serial.print("zero:");
  		Serial.println(button);	
  	}
  }
*/
}
void EasySwitch_sensors::buttonType2(int i, int button){
}
void EasySwitch_sensors::buttonType3(int i, int button){
  int pin;
  int ex;
      if((_buttonTime[i] == 0) && (_buttonState[i] == 0)) {
        if((button < 126) && (button >  -1 )){
          if(digitalRead(button) == LOW){
            if(pgm_read_byte_near(one_button_or_more_t + i) == 0 ){
              _buttonTime[i] = _buttonOneDebounce;
              _buttonState[i] =5;
            }
            else{
              _buttonTime[i] = _buttonTimeStart;
              _buttonState[i] =1; 
            }
          }
          else{
          	if (_oneClickPress[i] == 1){
          		_oneClickPress[i] = 0;
          	}
          }
        }
        if (button > 126){
          ex = GetMcpEx(button);
          pin = GetMcpPin(ex,button); 
          if(expander[ex].digitalRead(pin) == LOW){
            if(pgm_read_byte_near(one_button_or_more_t + i) == 0 ){
              _buttonTime[i] = _buttonOneDebounce;
              _buttonState[i] =5;
            }
            else{
              _buttonTime[i] = _buttonTimeStart;
              _buttonState[i] =1; 
            }
          }
          else{
          	if (_oneClickPress[i] == 1){
          		_oneClickPress[i] = 0;
          	}
          }
        }
      } 
      if((_buttonTime[i] < (_buttonDoubleStart)) && (_buttonTime[i] > _buttonDoubleStop) && (_buttonState[i] == 1)){
        if((button < 126) && (button >  -1 )){
          if(digitalRead(button) == LOW){
            _buttonState[i] = 2;
          }
        }
        if (button > 126){
          ex = GetMcpEx(button);
          pin = GetMcpPin(ex,button); 
          if (expander[ex].digitalRead(pin) == LOW){
              _buttonState[i] = 2;
          }
        }       
      } 
      if((_buttonTime[i] == 0) && (_buttonState[i] == 2)){
        if((button < 126) && (button >  -1 )){
          if(digitalRead(button) == LOW){
            _buttonState[i] = 3;
          }
        }
        if (button > 126){
          ex = GetMcpEx(button);
          pin = GetMcpPin(ex,button); 
          if (expander[ex].digitalRead(pin) == LOW){
            _buttonState[i] = 3;
          }
        }
      }
      if((_buttonTime[i] == 0) && (_buttonState[i] == 5)){
        _buttonState[i] = 4;
        if (_oneClickPress[i] == 0){
        	_oneClickPress[i] = 1;
        	Click(i,button,1);
        }    
      }
      if((_buttonTime[i] == 0) && (_buttonState[i] == 1)){
        _buttonState[i] = 4;
        Click(i,button,1);
      }
      if((_buttonTime[i] == 0) && (_buttonState[i] == 2)){
        _buttonState[i] = 4;
        _buttonTime[i] = _buttonDoubleDebounce;
        Click(i,button,2);
      }
      if((_buttonTime[i] == 0) && (_buttonState[i] == 3)){
        _buttonState[i] = 4;
        _buttonTime[i] = _buttonLongDebounce;
        Click(i,button,3);
      }
      if((_buttonTime[i] == 0) && (_buttonState[i] == 4)){
        _buttonState[i] = 0;
        _buttonTime[i] = 0;
      }
}
void EasySwitch_sensors::Click(int i, int button, byte click){
	byte button_click;
	int button_next;
	int relay;
	bool relayState;
	bool trigger;
	int id ;
	for(byte j=0; j < _numberOfRelayButtons; j++){
		button_click = pgm_read_word_near(button_click_t + j);
		button_next = pgm_read_word_near(button_next_t + j);
		if((button_next == button) and (button_click == click)){
			relay = pgm_read_word_near(relay_t + j);
  			relayState =  readState(relay);
  			relayState = !relayState;
  			trigger = getTrigger(j , relay, relayState);
  			if((relay < 126) && (relay >  -1 )){
  				SetOutputArdu(relay,trigger);
  			}
  			if(relay > 126){
   				SetOutputMCP(relay,trigger);
  			}
  			saveState(relay, relayState);
  			id = pgm_read_word_near(sensorId_t + j);
  			sendState(id, relayState,0, button);
		} 
	}
}
bool EasySwitch_sensors::getTrigger(int i , int relay, bool relayState){
	bool trigger;
	byte state = pgm_read_word_near(relayOptions_t + i);
	if (state == 0)  {
  		if(relayState == 0){
  			trigger = 1;
  		}
  		else{
  			trigger = 0;
  		}
  	}
  	else{
  		if(relayState == 0){
  			trigger = 0;
  		}
  		else{
  			trigger = 1;
  		}
  	}
  	return trigger;
}
void EasySwitch_sensors::loadStateAndSetOutput(){
	int relay;
	bool relayState;
	bool trigger;
	for(byte i=0; i < _numberOfRelayButtons; i++){
  		relay = pgm_read_word_near(relay_t + i);
  		relayState =  readState(relay);
  		trigger = getTrigger(i , relay, relayState);
  		if((relay < 126) && (relay >  -1 )){
  			SetOutputArdu(relay, trigger);	 			
  		}
  		if(relay > 126) {
  			SetOutputMCP(relay, trigger);
  		}
  	}
}
void EasySwitch_sensors::SetOutputArdu(int pin, bool state){
	digitalWrite(pin,state);
}
void EasySwitch_sensors::SetOutputMCP(int pin, bool state){
	int ex = GetMcpEx(pin);
  	int pin1 = GetMcpPin(ex,pin);	
  	expander[ex].digitalWrite(pin1,state);
}
int EasySwitch_sensors::GetMcpPin(int ex,int in){
  	int pin = ex*16;
  	pin = (in-127)-pin;
  	return pin;
}
int EasySwitch_sensors::GetMcpEx(int in){
	int ex = (in-127);
	if(ex > 0){
		ex = ex/16;
	}
  	return ex;
}
void EasySwitch_sensors::serialIn(){
	if (Serial.available() > 0) {
	    int inChar = Serial.read();	
	    if (isDigit(inChar)) {
	      _serialIn += (char)inChar;
	    }
	    if (inChar == 59){ 
	    	parseIn();
	    }  
	    if (inChar == '\n') {
	    	parseIn();
	    	parseAll();
	    	_serialIn = "";
	    	_serialCount = 0;
	    }
  	}
}
void EasySwitch_sensors::parseIn(){
	switch (_serialCount) {
		case 0:
	    	_node_id =_serialIn.toInt();
	    	break;
	  	case 1:
	  		_child_sensor_id =_serialIn.toInt();
	    	break;
	    case 2:
	    	_command = _serialIn.toInt();
	    	break;
	    case 3:
	    	_ack = _serialIn.toInt();
	    	break;
	    case 4:
	    	_type = _serialIn.toInt(); 
	    	break;
	    case 5:
	    	_payload = _serialIn.toInt(); 
	    	break;
	}
	_serialCount += 1;
	_serialIn = "";
}
void EasySwitch_sensors::parseAll(){
	int id;
	int relay;
	int trigger;
	if(_command == 1){
		for(byte i=0; i < _numberOfRelayButtons; i++){
			id = pgm_read_word_near(sensorId_t + i); 
			if(id == _child_sensor_id){
				relay = pgm_read_word_near(relay_t + i);
		  		trigger = getTrigger(i , relay, _payload);
		  		if((relay < 126) && (relay >  -1 )){
		  			SetOutputArdu(relay, trigger);	 			
		  		}
		  		if(relay > 126) {
		  			SetOutputMCP(relay, trigger);
		  		}
		  		saveState(relay, _payload);
		  			sendState(_child_sensor_id, trigger,1);
		  	}				
		}
	}
	else if (_command == 88){

sendAllState();
}
else if (_command == 89){

sendPresentationId();
}


	_node_id = 0;
	_child_sensor_id = 0;
	_command = 0;
	_ack = 0;
	_type = 0;
	_payload = 0;
}
