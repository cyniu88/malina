#include "menu_buderus.h"
#include "menu_root.h"
#include "menu_main.h"

MENU_BUDERUS::MENU_BUDERUS(thread_context *context, LCD_c *lcdPTR, MENU_STATE_MACHINE *msm, STATE lcdLED) : MENU_STATE_BASE(context, lcdPTR, msm, lcdLED)
{
}

MENU_BUDERUS::MENU_BUDERUS(const MENU_BUDERUS &base) : MENU_STATE_BASE(base)
{
}

MENU_BUDERUS::MENU_BUDERUS(MENU_BUDERUS &&base) : MENU_STATE_BASE(std::move(base))
{
}

MENU_BUDERUS &MENU_BUDERUS::operator=(const MENU_BUDERUS &base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

MENU_BUDERUS &MENU_BUDERUS::operator=(MENU_BUDERUS &&base)
{
    if (&base not_eq this)
    {
        contextPTR = base.contextPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return *this;
}

void MENU_BUDERUS::entry()
{
    menuDatabase.pushBack({"RUN CIRCLE PUMP", [=, this]()
                           {contextPTR->ptr_buderus->runCirclePompForWhile();  changeStateTo<MENU_ROOT>(); return "done"; }});
    menuDatabase.pushBack({"TEMP INSIDE", [=, this]()
                           { return to_string_with_precision(contextPTR->ptr_buderus->getInsideTemp()) + celsiusDegrees; }});
    menuDatabase.pushBack({"TEMP OUTDOOR", [=, this]()
                           { return to_string_with_precision(contextPTR->ptr_buderus->getOutdoorTemp()) + celsiusDegrees; }});
    menuDatabase.pushBack({"   EXIT", [=, this]()
                           { changeStateTo<MENU_ROOT>(); return "done"; }});
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_BUDERUS::exit()
{
}

void MENU_BUDERUS::keyPadOk()
{
    auto str = menuDatabase.getCurrent().function();
    lcdPTR->printString("", str);
}

void MENU_BUDERUS::keyPadUp()
{
    menuDatabase.up();
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_BUDERUS::keyPadDown()
{
    menuDatabase.down();
    print(menuDatabase.getCurrent().name, arrow);
}

void MENU_BUDERUS::keyPadRes()
{
    changeStateTo<MENU_MAIN>();
}

std::string MENU_BUDERUS::getStateName() const
{
    return typeid(this).name();
}
