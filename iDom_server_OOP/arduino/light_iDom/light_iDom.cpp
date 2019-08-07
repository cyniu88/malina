#include "light_iDom.h"

iDomLIGHT::iDomLIGHT(String name, int pinButton, int pinRelay): m_pinButton(pinButton),
  m_pinRelay(pinRelay),
  m_name(name)
{
  m_debouncer = Bounce();
  m_debouncer.attach(pinButton);
  m_debouncer.interval(5);
}

void iDomLIGHT::lightON() {
  digitalWrite(m_pinRelay, HIGH);
  EEPROM.update(m_pinRelay, HIGH);
  m_state = STATE::ON;
}
void iDomLIGHT::lightOFF() {
  digitalWrite(m_pinRelay, LOW);
  EEPROM.update(m_pinRelay, LOW);
  m_state = STATE::OFF;
}


bool iDomLIGHT::isLightON() {
  return !(m_state != STATE::ON);
}
bool iDomLIGHT::isLightOFF() {
  return !(m_state != STATE::OFF);
}
