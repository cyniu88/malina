#include <sstream>

#include "menu_maintenance.h"
#include "menu_main.h"
#include "menu_root.h"

MENU_MAINTENANCE::MENU_MAINTENANCE(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) :MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{

}

void MENU_MAINTENANCE::entry()
{
    menuDatabase.pushBack({"  HARD RELOAD",    [=]() {print("iDom RELOAD",""); 
													  my_dataPTR->main_iDomTools->reloadHard_iDomServer();}});
    menuDatabase.pushBack({"RASPBERRY RELOAD", [=]() {print("Raspberry", "RELOAD");
                                                      my_dataPTR->main_iDomTools->raspberryReboot();}});
    print(menuDatabase.getCurrent().name, arrow);
    my_dataPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_MAINTENANCE::exit()
{

}

std::string MENU_MAINTENANCE::getStateName() const
{
    return typeid (this).name();
}

void MENU_MAINTENANCE::keyPadOk()
{
    menuDatabase.getCurrent().function();
}

void MENU_MAINTENANCE::keyPadRes()
{
    changeStateTo<MENU_MAIN>();
}

void MENU_MAINTENANCE::keyPadUp()
{
    menuDatabase.up();
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_MAINTENANCE::keyPadDown()
{
    menuDatabase.down();
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_MAINTENANCE::keyPadLeft()
{
    --counter;
    std::stringstream ss;
    ss << counter << " : " << (char)counter;
    print("dane: ", ss.str());
}

void MENU_MAINTENANCE::keyPadRight()
{
    ++counter;
    std::stringstream ss;
    ss << counter << " : " << (char)counter;
    print("dane: ", ss.str());
}

void MENU_MAINTENANCE::timeout(std::function<void ()> function)
{
    changeStateTo<MENU_ROOT>();
}
