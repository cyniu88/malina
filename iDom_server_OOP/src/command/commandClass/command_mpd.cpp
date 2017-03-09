#include "command_mpd.h"
#include "../../blockQueue/blockqueue.h"

command_mpd::command_mpd(std::string name) :command(name)
{

}

std::string command_mpd::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string str_buf;

    if (v[1]=="start")
    {
        if (v.size()>2){
            if (std::stoi(v[2] ) > 0){
                iDomTOOLS::MPD_playId(my_data,std::stoi(v[2]));
                sleep(1);
                str_buf=my_data->ptr_MPD_info->songList[std::stoi(v[2])-1];
            }
        }
        else {
            iDomTOOLS::MPD_play();
            sleep(1);
            str_buf=my_data->ptr_MPD_info->title;
        }
    }
    else if (v[1]=="stop")
    {
        iDomTOOLS::MPD_stop();
        str_buf="stoped!";
    }
    else if (v[1]=="next")
    {
        iDomTOOLS::MPD_next();
        sleep(1);
        str_buf = my_data->ptr_MPD_info->radio + " : "+ my_data->ptr_MPD_info->title;
    }
    else if (v[1]=="prev")
    {
        iDomTOOLS::MPD_prev();
        sleep(1);
        str_buf=my_data->ptr_MPD_info->radio+ " : "+ my_data->ptr_MPD_info->title;
    }
    else if (v[1]=="pause")
    {
        iDomTOOLS::MPD_pause();
        str_buf="paused!";
    }
    else if (v[1]=="volume")
    {
        if (v[2]=="up"){
            iDomTOOLS::MPD_volumeUp();
        }
        else if (v[2]=="down"){
            iDomTOOLS::MPD_volumeDown();
        }
        else {
            int vol = std::stoi(v[2]);
            if (vol >0 && vol <100){
                iDomTOOLS::MPD_volumeSet(my_data,vol);
            }
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
        for (auto i : my_data->ptr_MPD_info->songList)
            str_buf += i+"\n";
    }
    else
    {
        str_buf="unknown parameter " + v[1];
    }
    return str_buf;
}

std::string command_mpd::help()
{
    std::string help = "MPD - for control music player:";
    help.append("\n\n");
    help.append("parameter:");
    help.append("\n");
    help.append("\tstart - play music");
    help.append("\n");
    help.append("\tstop  - stop music");
    help.append("\n");
    help.append("\tpause - pause music");
    help.append("\n");
    help.append("\tnext  - next song");
    help.append("\n");
    help.append("\tprev  - previous song");
    help.append("\n");
    help.append("\tlist  - show playlist");
    help.append("\n");
    help.append("\tget volume - get volume %");
    help.append("\n");
    help.append("\tget info - get info about current song");
    help.append("\n");
    help.append("\tvolume up/down - increase/decrease volume 1%");
    help.append("\n");
    return help;
}
