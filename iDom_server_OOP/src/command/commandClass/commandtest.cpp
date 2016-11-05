#include "commandtest.h"

commandTEST::commandTEST(std::string name):command(name)
{

}

std::string commandTEST::execute(std::vector<std::string> &v, thread_data *my_data)
{
    return v[0]+" "+v[1]+" tak dla testu =)";
}
