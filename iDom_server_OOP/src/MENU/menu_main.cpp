#include <iostream>
#include "menu_main.h"
#include "menu_light.h"
#include "menu_root.h"
#include "menu_music.h"

MENU_MAIN::MENU_MAIN(thread_data* my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm): MENU_STATE_BASE (my_data, lcdPTR, msm)
{
  //  std::cout << "MENU_MAIN::MENU_MAIN()" << std::endl;
}

MENU_MAIN::MENU_MAIN(const MENU_MAIN &base): MENU_STATE_BASE(base)
{
   // std::cout << "MENU_MAIN::MENU_MAIN() kopiujacy" << std::endl;
}

MENU_MAIN::MENU_MAIN(MENU_MAIN &&base): MENU_STATE_BASE(std::move(base))
{
  //  std::cout << "MENU_MAIN::MENU_MAIN() przenoszacy" << std::endl;
}

MENU_MAIN &MENU_MAIN::operator=(const MENU_MAIN &base)
{
   // std::cout << "MENU_MAIN::operator = kopiujacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
         menuDatabase = base.menuDatabase;
     }
     return * this;
}

MENU_MAIN &MENU_MAIN::operator=(MENU_MAIN &&base)
{
    std::cout << "MENU_MAIN::operator = przenoszacy" << std::endl;
     if(&base != this){
         my_dataPTR = base.my_dataPTR;
         lcdPTR = base.lcdPTR;
         stateMachinePTR = base.stateMachinePTR;
         menuDatabase = std::move(base.menuDatabase);
     }
     return * this;
}

MENU_MAIN::~MENU_MAIN()
{
   // std::cout << "MENU_MAIN::~MENU_MAIN()" << std::endl;
}

void MENU_MAIN::entry()
{
  //  std::cout << "MENU_MAIN::entry()" << std::endl;
    menuDatabase.pushBack({"   MUSIC",  [=]() { changeStateTo<MENU_MUSIC>();}});
    menuDatabase.pushBack({"   LIGHT",  [=]() { changeStateTo<MENU_LIGHT>();}});
    menuDatabase.pushBack({"   KODI",   [=]() { my_dataPTR->main_iDomTools->startKodi_Thread();
                                                changeStateTo<MENU_ROOT>();}});
    menuDatabase.pushBack({"   BUDERUS",[=]() { changeStateTo<MENU_ROOT>();}});
    menuDatabase.pushBack({"   EXIT",   [=]() { changeStateTo<MENU_ROOT>();}});
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_MAIN::exit()
{
  //  std::cout << "MENU_MAIN::exit()" << std::endl;
}

std::string MENU_MAIN::getStateName()
{
    return typeid (this).name();
}

void MENU_MAIN::keyPadOk()
{
    menuDatabase.getCurrent().function();
}

void MENU_MAIN::keyPadRes()
{
    changeStateTo<MENU_ROOT>();
}

void MENU_MAIN::keyPadUp()
{
    menuDatabase.up();
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_MAIN::keyPadDown()
{
    menuDatabase.down();
    print(menuDatabase.getCurrent().name, arrow);
}

