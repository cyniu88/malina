#include <iostream>

#include "menu_light.h"
#include "menu_main.h"
#include "menu_volume.h"
#include "menu_root.h"

MENU_LIGHT::MENU_LIGHT(thread_data *my_data, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED):MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{
   // std::cout << "MENU_LIGHT::MENU_LIGHT()" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(const MENU_LIGHT &base): MENU_STATE_BASE(base)
{
   // std::cout << "MENU_LIGHT::MENU_LIGHT() kopiujacy" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(MENU_LIGHT &&base):MENU_STATE_BASE(std::move(base)),  lightDatabase (std::move(base.lightDatabase))
{
   // std::cout << "MENU_LIGHT::MENU_LIGHT() przenoszacy" << std::endl;
}

MENU_LIGHT &MENU_LIGHT::operator=(const MENU_LIGHT &base)
{
   // std::cout << "MENU_LIGHT::operator = kopiujacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_LIGHT &MENU_LIGHT::operator=(MENU_LIGHT &&base)
{
    std::cout << "MENU_LIGHT:operator = przenoszacy" << std::endl;
    if(&base != this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        lightDatabase = std::move(base.lightDatabase);
    }
    return * this;
}

MENU_LIGHT::~MENU_LIGHT()
{
   // std::cout << "MENU_LIGHT::~MENU_LIGHT()" << std::endl;
}

void MENU_LIGHT::entry()
{
   // std::cout << "MENU_LIGHT::entry()" << std::endl;
    auto jj = my_dataPTR->main_house_lighting_handler->getAllInfoJSON();

    for(const auto& data : jj){
        auto name = data.at("room").get<std::string>();

        BULB bulb(data.at("bubl name").get<std::string>(),data.at("bulb ID").get<int>());
        lightDatabase.databaseMap[name].pushBack({bulb,0});
    }
    lightDatabase.databaseMap["all"].pushBack({BULB("all",0),0});

    for(auto& mm : lightDatabase.databaseMap){
        mm.second.pushBack({BULB("all",0),0});
    }

    lightDatabase.begin();
    auto p = lightDatabase.getCurrent();
    print(p->first , p->second.getCurrent().name.name);
}

void MENU_LIGHT::exit()
{
   // std::cout << "MENU_LIGHT::exit()" << std::endl;
}

std::string MENU_LIGHT::getStateName()
{
    return typeid (this).name();
}

void MENU_LIGHT::keyPadRes()
{
    changeStateTo<MENU_MAIN>();
}

void MENU_LIGHT::keyPadUp()
{
    lightDatabase.up();
    auto p = lightDatabase.getCurrent();
    print(p->first, p->second.getCurrent().name.name);
}

void MENU_LIGHT::keyPadDown()
{
    lightDatabase.down();
    auto p = lightDatabase.getCurrent();
    print(p->first, p->second.getCurrent().name.name);
}

void MENU_LIGHT::keyPadLeft()
{
    auto p = lightDatabase.getCurrent();
    p->second.down();
    print(p->first, p->second.getCurrent().name.name);
}

void MENU_LIGHT::keyPadRight()
{
    auto p = lightDatabase.getCurrent();
    p->second.up();
    print(p->first, p->second.getCurrent().name.name);
}

void MENU_LIGHT::keyPadOk()
{
    auto p = lightDatabase.getCurrent();
    int id = p->second.getCurrent().name.id;
    if(p->first == "all"){
        my_dataPTR->main_house_lighting_handler->turnOnAllBulb();
        return;
    }
    else if(id == 0){
        my_dataPTR->main_house_lighting_handler->turnOnAllInRoom(p->first);
        return;
    }
    my_dataPTR->main_house_lighting_handler->turnOnBulb(id);
}

void MENU_LIGHT::keyPadPower()
{
    auto p = lightDatabase.getCurrent();
    int id = p->second.getCurrent().name.id;
    if(p->first == "all"){
        my_dataPTR->main_house_lighting_handler->turnOffAllBulb();
        return;
    }
    else if(id == 0){
        my_dataPTR->main_house_lighting_handler->turnOffAllInRoom(p->first);
        return;
    }
    my_dataPTR->main_house_lighting_handler->turnOffBulb(id);
}

