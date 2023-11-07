
#include "functions.h"

bool useful_F::go_while = true;
bool useful_F::workServer = true;

using namespace std::chrono_literals;

////////////////////////////////////////// not used now
/*
void ChangeEndianness(int32_t * value,int MAX_MSG_LEN_INT)
{
    if (__BYTE_ORDER == __BIG_ENDIAN)
    {

    }
    else if(__BYTE_ORDER == __LITTLE_ENDIAN)
    { //cout << "little-endian" << endl
        for (int i =0; i < MAX_MSG_LEN_INT; ++i )
        {
            int32_t result = 0;
            result |= (value[i] & 0x000000FF) << 24;
            result |= (value[i] & 0x0000FF00) << 8;
            result |= (value[i] & 0x00FF0000) >> 8;
            result |= (value[i] & 0xFF000000) >> 24;

            value[i] = result;
        }
    }
}// ChangeEndianness

void binary(int val)
{    for (int i = 31; i >= 0; i--)
    {
        if ((i + 1) % 8 == 0)
            std::cout << ' ';
        std::cout << ((val >> i) % 2);
    }
     std::cout << std::endl;
}
*/

// void useful_F::clearThreadArray(thread_data* context)
//{
//     for (int i = 0; i< iDomConst::MAX_CONNECTION;++i)
//     {
//         if (context->main_THREAD_arr->at(i).thread_ID == std::this_thread::get_id())
//         {
//             //context->main_THREAD_arr[i].thread.detach();
//             context->main_THREAD_arr->at(i).thread_name   = "  -empty-  ";
//             context->main_THREAD_arr->at(i).thread_socket = 0;
//             context->main_THREAD_arr->at(i).thread_ID     = std::thread::id();
//             break;
//         }
//     }
// }

void useful_F::sleep(std::chrono::seconds sec)
{
    std::this_thread::sleep_for(sec);
}

int useful_F::runLinuxCommand(const std::string &cmd)
{
    return system(cmd.c_str());
}
