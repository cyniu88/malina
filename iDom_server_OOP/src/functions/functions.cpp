
#include "functions.h"
#include "../../libs/config_parser/parser.hpp"

bool useful_F::go_while = true;
bool useful_F::workServer = true;


////////////////////////////////////////// not used now
/*
void ChangeEndianness(int32_t * value,int MAX_MSG_LEN_INT)
{
    if (__BYTE_ORDER == __BIG_ENDIAN)
    {

    }
    else if( __BYTE_ORDER == __LITTLE_ENDIAN)
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



//void useful_F::clearThreadArray(thread_data* my_data)
//{
//    for (int i = 0; i< iDomConst::MAX_CONNECTION;++i)
//    {
//        if (my_data->main_THREAD_arr->at(i).thread_ID == std::this_thread::get_id())
//        {
//            //my_data->main_THREAD_arr[i].thread.detach();
//            my_data->main_THREAD_arr->at(i).thread_name   = "  -empty-  ";
//            my_data->main_THREAD_arr->at(i).thread_socket = 0;
//            my_data->main_THREAD_arr->at(i).thread_ID     = std::thread::id();
//            break;
//        }
//    }
//}

void useful_F::sleep(int sec)
{
    std::this_thread::sleep_for( std::chrono::seconds(sec) );
}

int useful_F::runLinuxCommand(const std::string &cmd)
{
    return system(cmd.c_str());
}
