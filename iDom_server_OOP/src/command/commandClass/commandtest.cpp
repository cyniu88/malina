#include "commandtest.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string msg="test - for test";
    msg.push_back((char)0);
    msg.append("dwa");
    auto l = msg.length();
    auto s = msg.size();

    msg.append(std::to_string(l));
    msg.append("  s: ");
    msg.append(std::to_string(s));
    return msg;
}

std::string commandTEST::help()
{

    return "test - for test\n";
}
