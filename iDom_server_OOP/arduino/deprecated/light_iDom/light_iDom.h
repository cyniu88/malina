#ifndef LIGHT_IDOM_H
#define LIGHT_IDOM_H

#include <Bounce2.h>
#include <EEPROM.h>


enum class STATE {
  ON = 1,
  OFF = 0,
  UNKNOWN = 3
};

class iDomLIGHT {
  public:
    int m_pinButton = 0;
    int m_pinRelay = 0;
    String m_name = "NULL";
    Bounce m_debouncer;
    iDomLIGHT(String name, int pinButton, int pinRelay);
    void lightON();
    void lightOFF();
    bool isLightON();
    bool isLightOFF();
    STATE getState();
    String stateToString(STATE s);
};

#endif
