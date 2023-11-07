#include "menu_info.h"
#include "menu_root.h"
#include "menu_main.h"

#include "../iDomTools/idomtools_interface.h"

MENU_INFO::MENU_INFO(thread_data *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED)
{
}

MENU_INFO::MENU_INFO(const MENU_INFO &base) : MENU_STATE_BASE(base)
{
}

MENU_INFO::MENU_INFO(MENU_INFO &&base) : MENU_STATE_BASE(std::move(base))
{
}

MENU_INFO &MENU_INFO::operator=(const MENU_INFO &base)
{ //  std::cout << "MENU_VOLUME::operator = kopiujacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

MENU_INFO &MENU_INFO::operator=(MENU_INFO &&base)
{ //  std::cout << "MENU_VOLUME::operator = kopiujacy" << std::endl;
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

void MENU_INFO::entry()
{
    m_infoDatabase.insert(std::pair<std::string, std::string>("smog", contextPTR->main_iDomTools->getSmog()));
    m_infoDatabase.insert(std::pair<std::string, std::string>("temperatura na piecu",
                                                              to_string_with_precision(contextPTR->ptr_buderus->getCurFlowTemp()) + celsiusDegrees));
    m_infoDatabase.insert(std::pair<std::string, std::string>("dlugosc dnia", contextPTR->main_iDomTools->getDayLenght()));
    m_infoDatabase.insert(std::pair<std::string, std::string>("wschod slonca", contextPTR->main_iDomTools->getSunrise()));
    m_infoDatabase.insert(std::pair<std::string, std::string>("zachod slonca", contextPTR->main_iDomTools->getSunset()));
    m_infoDatabase.begin();
    auto p = m_infoDatabase.getCurrent();
    print(p->first, p->second);
}

void MENU_INFO::exit()
{
}

std::string MENU_INFO::getStateName() const
{
    return typeid(this).name();
}

void MENU_INFO::keyPadUp()
{
    m_infoDatabase.up();
    auto p = m_infoDatabase.getCurrent();
    print(p->first, p->second);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_INFO::keyPadDown()
{
    m_infoDatabase.down();
    auto p = m_infoDatabase.getCurrent();
    print(p->first, p->second);
    contextPTR->main_Rs232->print("TIMEOUT:30000;");
}

void MENU_INFO::keyPadRes()
{
    changeStateTo<MENU_MAIN>();
}

void MENU_INFO::timeout(std::function<void()> function)
{
    changeStateTo<MENU_ROOT>();
}
