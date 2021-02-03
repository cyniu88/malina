#include <iostream>
#include "menu_kodi.h"
#include "menu_root.h"

MENU_KODI::MENU_KODI(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED): MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{
   // std::cout << "MENU_KODI::MENU_KODI()" << std::endl;
}

MENU_KODI::MENU_KODI(const MENU_KODI &base):MENU_STATE_BASE(base)
{
   // std::cout << "MENU_KODI::MENU_KODI() kopiujacy" << std::endl;
}

MENU_KODI::MENU_KODI(MENU_KODI &&base):MENU_STATE_BASE(std::move(base))
{
  //  std::cout << "MENU_KODI::MENU_KODI() przenoszacy" << std::endl;
}

MENU_KODI &MENU_KODI::operator=(const MENU_KODI &base)
{
  //  std::cout << "MENU_KODI::operator = kopiujacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
     }
     return * this;
}

MENU_KODI &MENU_KODI::operator=(MENU_KODI &&base)
{
  //  std::cout << "MENU_KODI::operator = przenoszacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
     }
     return * this;
}

MENU_KODI::~MENU_KODI()
{

}

void MENU_KODI::entry()
{
    my_dataPTR->main_iDomTools->startKodi_Thread();
}

void MENU_KODI::exit()
{

}

std::string MENU_KODI::getStateName()
{
    return typeid (this).name();
}

void MENU_KODI::timeout(std::function<void ()> function)
{
    changeStateTo<MENU_ROOT>();
}
