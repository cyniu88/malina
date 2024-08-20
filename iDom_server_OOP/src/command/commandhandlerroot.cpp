#include "commandhandlerroot.h"
#include "commandClass/command_cmd.h"
#include "commandClass/command_event.h"
#include "commandClass/command_433mhz.h"
#include "commandClass/command_mpd.h"
#include "commandClass/commandrs232.h"
#include "commandClass/command_idom.h"
#include "commandClass/command_program.h"
#include "commandClass/command_ardu.h"
#include "commandClass/command_state.h"
#include "commandClass/command_sleep.h"
#include "commandClass/command_gateway.h"
#include "commandClass/command_buderus.h"
#include "commandClass/command_light.h"

commandHandlerRoot::commandHandlerRoot(thread_context *context) : commandHandler(context)
{
    std::unique_ptr<command> gateway(new command_gateway("gateway"));
    commandMap.insert(std::make_pair(gateway->getCommandName(), std::move(gateway)));

    std::unique_ptr<command> cmd(new command_cmd("cmd"));
    commandMap.insert(std::make_pair(cmd->getCommandName(), std::move(cmd)));

    std::unique_ptr<command> event(new command_event("event"));
    commandMap.insert(std::make_pair(event->getCommandName(), std::move(event)));

    std::unique_ptr<command> r_433MHz(new command_433MHz("433MHz"));
    commandMap.insert(std::make_pair(r_433MHz->getCommandName(), std::move(r_433MHz)));

    std::unique_ptr<command> program(new command_program("program"));
    commandMap.insert(std::make_pair(program->getCommandName(), std::move(program)));

    std::unique_ptr<command> MPD(new command_mpd("MPD"));
    commandMap.insert(std::make_pair(MPD->getCommandName(), std::move(MPD)));

    std::unique_ptr<command> RS232(new commandRS232("RS232"));
    commandMap.insert(std::make_pair(RS232->getCommandName(), std::move(RS232)));

    std::unique_ptr<command> iDom(new command_iDom("iDom"));
    commandMap.insert(std::make_pair(iDom->getCommandName(), std::move(iDom)));

    std::unique_ptr<command> state(new command_state("state"));
    commandMap.insert(std::make_pair(state->getCommandName(), std::move(state)));

    std::unique_ptr<command> ardu(new command_ardu("ardu", context));
    commandMap.insert(std::make_pair(ardu->getCommandName(), std::move(ardu)));

    std::unique_ptr<command> sleep(new command_sleep("sleep"));
    commandMap.insert(std::make_pair(sleep->getCommandName(), std::move(sleep)));

    std::unique_ptr<command> buderus(new command_buderus("buderus"));
    commandMap.insert(std::make_pair(buderus->getCommandName(), std::move(buderus)));

    std::unique_ptr<command> light(new command_light("light"));
    commandMap.insert(std::make_pair(light->getCommandName(), std::move(light)));
}

