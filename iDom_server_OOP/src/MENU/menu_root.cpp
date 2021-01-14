#include "menu_root.h"
#include "menu_main.h"

MENU_ROOT::MENU_ROOT(std::shared_ptr<LCD_c> _lcdPtr)
{
  lcd = _lcdPtr;
}

void MENU_ROOT::entry()
{
    lcd->noBacklight();
}

void MENU_ROOT::exit()
{

}

void MENU_ROOT::printStateName()
{
    std::cout << "klasa state: " << typeid (this).name() << std::endl;
}

void MENU_ROOT::keyPadMenu()
{
    changeTo<MENU_MAIN>();
}
