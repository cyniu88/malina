#include "light_iDom_handler.h"

void light_iDomHandler::setupPin() {
  for (int i = 0; i < m_lightCount; ++i) {
    pinMode(m_lightArray[i]->m_pinRelay, OUTPUT);
    pinMode(m_lightArray[i]->m_pinButton, INPUT_PULLUP);
    Serial.print("setup pin ");
    Serial.println(i);
  }
}


void light_iDomHandler::restorePinState() {
  byte value;
  for (int i = 0; i < m_lightCount; ++i) {
    //pinMode(m_lightArray[i]->m_pinRelay, OUTPUT);
    value = EEPROM.read(m_lightArray[i]->m_pinRelay);
    if (value == HIGH) {
      m_lightArray[i]->lightON();
    }
    else
      m_lightArray[i]->lightOFF();
  }
}

void light_iDomHandler::run() {
  for (int i = 0; i < m_lightCount; ++i) {
    if (m_lightArray[i]->m_debouncer.update()) {
      //Serial.print(".");
      // Get the update value.
      int value = m_lightArray[i]->m_debouncer.read();
      // Send in the new value.
      if (value == LOW && m_lightArray[i]->isLightON()) {
        m_lightArray[i]->lightON();
        continue;
      }
      if (value == LOW && m_lightArray[i]->isLightOFF() ) {
        m_lightArray[i]->lightOFF();
        continue;
      }
    }
  }

}

void light_iDomHandler::insert(iDomLIGHT* it) {
  m_lightArray[m_lightCount] = it;
  ++m_lightCount;
}


void light_iDomHandler::printAllNameAndState() {
  for (int i = 0; i < m_lightCount; ++i) {
    Serial.print("light status name ");
    Serial.print(m_lightArray[i]->m_name);
    Serial.print(" status ");
    Serial.print(m_lightArray[i]->stateToString(m_lightArray[i]->m_state));
    Serial.print(" ID ");
    Serial.print(i);
    Serial.println(";");
  }
}
