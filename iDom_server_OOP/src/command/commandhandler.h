#ifndef COMMANDHANDLE_H
#define COMMANDHANDLE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "commandClass/commandexit.h"
#include "commandClass/commandtest.h"


class commandHandler
{

    std::map<std::string, std::unique_ptr<command> > commandMap;

public:
    commandHandler();

    std::string run(std::vector <std::string> &v, thread_data *my_data);
};

#endif // COMMANDHANDLE_H
