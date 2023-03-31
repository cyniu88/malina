#include <iostream>
#include "menu_main.h"
#include "menu_light.h"
#include "menu_root.h"
#include "menu_kodi.h"
#include "menu_buderus.h"
#include "menu_info.h"
#include "menu_maintenance.h"

MENU_MAIN::MENU_MAIN(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(my_data, lcdPTR, msm, lcdLED)
{
    //  std::cout << "MENU_MAIN::MENU_MAIN()" << std::endl;
}

MENU_MAIN::MENU_MAIN(const MENU_MAIN &base) : MENU_STATE_BASE(base)
{
    // std::cout << "MENU_MAIN::MENU_MAIN() kopiujacy" << std::endl;
}

MENU_MAIN::MENU_MAIN(MENU_MAIN &&base) : MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_MAIN::MENU_MAIN() przenoszacy" << std::endl;
}

MENU_MAIN &MENU_MAIN::operator=(const MENU_MAIN &base)
{
    // std::cout << "MENU_MAIN::operator = kopiujacy" << std::endl;
    if (&base not_eq this)
    {
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        menuDatabase = base.menuDatabase;
    }
    return *this;
}

MENU_MAIN &MENU_MAIN::operator=(MENU_MAIN &&base)
{
    std::cout << "MENU_MAIN::operator = przenoszacy" << std::endl;
    if (&base not_eq this)
    {
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        menuDatabase = std::move(base.menuDatabase);
    }
    return *this;
}

MENU_MAIN::~MENU_MAIN()
{
    // std::cout << "MENU_MAIN::~MENU_MAIN()" << std::endl;
}

void MENU_MAIN::entry()
{
    //  std::cout << "MENU_MAIN::entry()" << std::endl;
    menuDatabase.pushBack({"   MUSIC", [=]()
                           { my_dataPTR->main_iDomTools->MPD_play(my_dataPTR);
                                                changeStateTo<MENU_ROOT>(); }});
    menuDatabase.pushBack({"   LIGHT", [=]()
                           { changeStateTo<MENU_LIGHT>(); }});
    menuDatabase.pushBack({"   KODI", [=]()
                           { changeStateTo<MENU_KODI>(); }});
    menuDatabase.pushBack({"   INFO", [=]()
                           { changeStateTo<MENU_INFO>(); }});
    menuDatabase.pushBack({"   BUDERUS", [=]()
                           { changeStateTo<MENU_BUDERUS>(); }});
    menuDatabase.pushBack({" MAINTENANCE", [=]()
                           { changeStateTo<MENU_MAINTENANCE>(); }});
    menuDatabase.pushBack({"   EXIT", [=]()
                           { changeStateTo<MENU_ROOT>(); }});
    print(menuDatabase.getCurrent().name, arrow);
    my_dataPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_MAIN::exit()
{
    //  std::cout << "MENU_MAIN::exit()" << std::endl;
}

std::string MENU_MAIN::getStateName() const
{
    return typeid(this).name();
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

void MENU_MAIN::timeout(std::function<void()> function)
{
    changeStateTo<MENU_ROOT>();
}
