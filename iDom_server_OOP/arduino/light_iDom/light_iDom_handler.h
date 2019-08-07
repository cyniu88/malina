#ifndef LIGHT_IDOM_HANDLER_H
#define LIGHT_IDOM_HANDLER_H

#include  "light_iDom.h"

constexpr int arrayMaxSize = 3;

class light_iDomHandler {

    iDomLIGHT* m_lightArray[arrayMaxSize];
    int m_lightCount = 0;
  public:
    void insert(iDomLIGHT* it);
    void setupPin();
    void restorePinState();
    void run();
};

#endif
