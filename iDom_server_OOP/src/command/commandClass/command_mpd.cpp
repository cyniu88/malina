#include "command_mpd.h"
#include "../../functions/functions.h"
#include "../../iDomTools/idomtools_interface.h"

command_mpd::command_mpd(const std::string &name) :command(name)
{
}

std::string command_mpd::execute(std::vector<std::string> &v, thread_context *context)
{
    std::string str_buf;

    if (v[1] == "start")
    {
        if (v.size()>2)
        {
            int id = std::stoi(v[2]);
            if (id > 0)
            {
                context->main_iDomTools->MPD_play(context,id);
                useful_F::sleep(1s);
                str_buf = context->ptr_MPD_info->songList[id-1];
            }
        }
        else
        {
            context->main_iDomTools->MPD_play(context);
            useful_F::sleep(1s);
            str_buf = context->ptr_MPD_info->title;
        }
        context->main_iDomTools->saveState_iDom(context->serverStarted);
    }
    else if (v[1] == "stop")
    {
        context->main_iDomTools->MPD_stop();
        str_buf = "stoped!";
        context->main_iDomTools->saveState_iDom(context->serverStarted);
    }
    else if (v[1] == "next")
    {
        context->main_iDomTools->MPD_next();
        useful_F::sleep(1s);
        str_buf = context->ptr_MPD_info->radio + " : "+ context->ptr_MPD_info->title;
    }
    else if (v[1] == "prev")
    {
        context->main_iDomTools->MPD_prev();
        useful_F::sleep(1s);
        str_buf = context->ptr_MPD_info->radio+ " : "+ context->ptr_MPD_info->title;
    }
    else if (v[1] == "pause")
    {
        context->main_iDomTools->MPD_pause();
        str_buf = "paused!";
    }
    else if (v[1] == "volume")
    {
        if (v[2] == "up")
        {
            context->main_iDomTools->MPD_volumeUp();
        }
        else if (v[2] == "down")
        {
            context->main_iDomTools->MPD_volumeDown();
        }
        else
        {
            int vol = std::stoi(v[2]);
            if (vol >0 && vol <100)
            {
                context->main_iDomTools->MPD_volumeSet(context,vol);
            }
        }
        //sleep(1);
        str_buf = std::to_string(context->ptr_MPD_info->volume);
    }
    else if (v[1] == "get")
    {
        if(v[2] == "volume")
        {
            str_buf = std::to_string(context->ptr_MPD_info->volume);
        }
        else if (v[2] == "info")
        {
            str_buf = context->ptr_MPD_info->radio + " : " + context->ptr_MPD_info->title;
        }
    }
    else if (v[1] == "list")
    {
        for (const auto& i : context->ptr_MPD_info->songList)
            str_buf.append(i + "\n");
    }
    else
    {
        str_buf = "unknown parameter " + v[1];
    }
    return str_buf;
}

std::string command_mpd::help() const
{
    std::stringstream help;
    help << "MPD - for control music player:" << std::endl;
    help << "parameter:" << std::endl;
    help << "\tstart - play music" << std::endl;
    help << "\tstop  - stop music" << std::endl;
    help << "\tpause - pause music" << std::endl;
    help << "\tnext  - next song" << std::endl;
    help << "\tprev  - previous song" << std::endl;
    help << "\tlist  - show playlist" << std::endl;
    help << "\tget volume - get volume %" << std::endl;
    help << "\tget info - get info about current song" << std::endl;
    help << "\tvolume up/down - increase/decrease volume 1%" << std::endl;
    return help.str();
}
