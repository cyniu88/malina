#include "menu_light.h"
#include "menu_main.h"
#include "menu_root.h"

MENU_LIGHT::MENU_LIGHT(thread_data *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED)
{
    // std::cout << "MENU_LIGHT::MENU_LIGHT()" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(const MENU_LIGHT &base) : MENU_STATE_BASE(base)
{
    // std::cout << "MENU_LIGHT::MENU_LIGHT() kopiujacy" << std::endl;
}

MENU_LIGHT::MENU_LIGHT(MENU_LIGHT &&base) : MENU_STATE_BASE(std::move(base)), m_lightDatabase(std::move(base.m_lightDatabase))
{
    // std::cout << "MENU_LIGHT::MENU_LIGHT() przenoszacy" << std::endl;
}

MENU_LIGHT &MENU_LIGHT::operator=(const MENU_LIGHT &base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

MENU_LIGHT &MENU_LIGHT::operator=(MENU_LIGHT &&base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
        m_lightDatabase = std::move(base.m_lightDatabase);
    }
    return *this;
}

void MENU_LIGHT::entry()
{
    // std::cout << "MENU_LIGHT::entry()" << std::endl;
    auto jj = contextPTR->main_house_room_handler->getAllInfoJSON();
    // std::cout << jj.dump(4) << std::endl;
    for (const auto &data : jj)
    {
        auto name = data.at("room").get<std::string>();

        BULB bulb(data.at("bulb name").get<std::string>(), data.at("bulb ID").get<int>());
        m_lightDatabase.databaseMap[name].pushBack({bulb, 0});
    }
    m_lightDatabase.databaseMap["all"].pushBack({BULB("all", 0), 0});

    for (auto &mm : m_lightDatabase.databaseMap)
    {
        mm.second.pushBack({BULB("all", 0), 0});
    }

    m_lightDatabase.begin();
    auto p = m_lightDatabase.getCurrent();
    print(p->first, p->second.getCurrent().name.name);
}

void MENU_LIGHT::exit()
{
    // std::cout << "MENU_LIGHT::exit()" << std::endl;
}

std::string MENU_LIGHT::getStateName() const
{
    return typeid(this).name();
}

void MENU_LIGHT::keyPadRes()
{
    changeStateTo<MENU_MAIN>();
}

void MENU_LIGHT::keyPadUp()
{
    m_lightDatabase.up();
    auto p = m_lightDatabase.getCurrent();
    print(p->first, p->second.getCurrent().name.name);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_LIGHT::keyPadDown()
{
    m_lightDatabase.down();
    auto p = m_lightDatabase.getCurrent();
    print(p->first, p->second.getCurrent().name.name);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_LIGHT::keyPadLeft()
{
    auto p = m_lightDatabase.getCurrent();
    p->second.down();
    print(p->first, p->second.getCurrent().name.name);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_LIGHT::keyPadRight()
{
    auto p = m_lightDatabase.getCurrent();
    p->second.up();
    print(p->first, p->second.getCurrent().name.name);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_LIGHT::keyPadOk()
{
    auto p = m_lightDatabase.getCurrent();
    int id = p->second.getCurrent().name.id;
    if (p->first == "all")
    {
        contextPTR->main_house_room_handler->turnOnAllBulb();
        return;
    }
    else if (id == 0)
    {
        contextPTR->main_house_room_handler->turnOnAllInRoom(p->first);
        return;
    }
    contextPTR->main_house_room_handler->turnOnBulb(id);
}

void MENU_LIGHT::keyPadPower()
{
    auto p = m_lightDatabase.getCurrent();
    int id = p->second.getCurrent().name.id;
    if (p->first == "all")
    {
        contextPTR->main_house_room_handler->turnOffAllBulb();
        return;
    }
    else if (id == 0)
    {
        contextPTR->main_house_room_handler->turnOffAllInRoom(p->first);
        return;
    }
    contextPTR->main_house_room_handler->turnOffBulb(id);
}

void MENU_LIGHT::timeout(std::function<void()> function)
{
    changeStateTo<MENU_ROOT>();
}
