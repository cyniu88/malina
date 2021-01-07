#include "light_iDom.h"

iDomLIGHT::iDomLIGHT(String name, int pinButton, int pinRelay): m_pinButton(pinButton),
  m_pinRelay(pinRelay),
  m_name(name),
  m_debouncer(Bounce())
{
  m_debouncer.attach(pinButton);
  m_debouncer.interval(5);
}

void iDomLIGHT::lightON() {
  digitalWrite(m_pinRelay, HIGH);
  EEPROM.update(m_pinRelay, HIGH);
  Serial.print("light on name ");
  Serial.print(m_name);
  Serial.println(";");
}
void iDomLIGHT::lightOFF() {
  digitalWrite(m_pinRelay, LOW);
  EEPROM.update(m_pinRelay, LOW);
  Serial.print("light off name ");
  Serial.print(m_name);
  Serial.println(";");
}


bool iDomLIGHT::isLightON() {
  return (getState() != STATE::ON);
}
bool iDomLIGHT::isLightOFF() {
  return (getState() != STATE::OFF);
}

STATE iDomLIGHT::getState() {
  int value = EEPROM.read(m_pinRelay);
  if (value == HIGH) {
    return STATE::ON;
  }
  else
    return STATE::OFF;
}

String iDomLIGHT::stateToString(STATE s) {
  switch (s)
  {
    case STATE::ON:
      return "ON";
    case STATE::OFF:
      return "OFF";
    case STATE::UNKNOWN:
      return "UNKNOWN";
  }
}
