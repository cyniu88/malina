#include "command_mpd.h"
#include "../../blockQueue/blockqueue.h"

command_mpd::command_mpd(std::string name) :command(name)
{

}

std::string command_mpd::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf;
    blockQueue char_queue;

    if (v[1]=="start")
    {

        char_queue._add('t');
        sleep(1);
        str_buf=my_data->ptr_MPD_info->title;
    }
    else if (v[1]=="stop")
    {
        char_queue._add('P');
        str_buf="stoped!";
    }
    else if (v[1]=="next")
    {
        char_queue._add('D');
        sleep(1);
        str_buf = my_data->ptr_MPD_info->radio + " : "+ my_data->ptr_MPD_info->title;
    }
    else if (v[1]=="prev")
    {
        char_queue._add('U');
        sleep(1);
        str_buf=my_data->ptr_MPD_info->radio+ " : "+ my_data->ptr_MPD_info->title;
    }
    else if (v[1]=="pause")
    {
        char_queue._add('A');
        str_buf="paused!";
    }
    else if (v[1]=="volume")
    {
        if (v[2]=="up"){
            char_queue._add('+');
        }
        else if (v[2]=="down"){
            char_queue._add('-');
        }


        sleep(1);
        str_buf=std::to_string(   my_data->ptr_MPD_info->volume);
    }


    else if (v[1]=="get")
    {
        if(v[2]=="volume"){
            str_buf=std::to_string(   my_data->ptr_MPD_info->volume);
        }
        else if (v[2]=="info")
        {

            str_buf = my_data->ptr_MPD_info->radio + " : "+ my_data->ptr_MPD_info->title;

        }
    }

    else if (v[1]=="list")
    {

        str_buf =  my_data->ptr_MPD_info->songList;
        //my_data->ptr_MPD_info->songList ="";

    }
    else
    {
        str_buf="unknown parameter " + v[1];
    }
    return str_buf;
}
