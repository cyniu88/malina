#include "idomtools.h"
#include "../blockQueue/blockqueue.h"
#include "../iDom_server_OOP.h"

void iDomTOOLS::MPD_play(thread_data *context)
{
    if (context->idom_all_state.houseState == STATE::UNLOCK)
    {
        blockQueue _q;
        _q._add(MPD_COMMAND::PLAY);
    }
    else
    {
        context->myEventHandler.run("MPD")->addEvent("MPD can not start due to home state: " +
                                                     stateToString(context->idom_all_state.houseState));
    }
}

void iDomTOOLS::MPD_stop()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::STOP);
}

void iDomTOOLS::MPD_next()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::NEXT);
}

void iDomTOOLS::MPD_prev()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::PREV);
}

void iDomTOOLS::MPD_pause()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::PAUSE);
}

void iDomTOOLS::MPD_volumeUp()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::VOLUP);
}

void iDomTOOLS::MPD_volumeDown()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::VOLDOWN);
}

void iDomTOOLS::MPD_volumeSet(thread_data *context, int vol)
{
    context->ptr_MPD_info->volume = vol;
    blockQueue _q;
    _q._add(MPD_COMMAND::VOLSET);
}

void iDomTOOLS::MPD_play(thread_data *context, int id)
{
    if (context->idom_all_state.houseState == STATE::UNLOCK)
    {
        context->ptr_MPD_info->currentSongID = id;
        blockQueue _q;
        _q._add(MPD_COMMAND::PLAY_ID);
    }
    else
    {
        context->myEventHandler.run("MPD")->addEvent("MPD can not start due to home state: " +
                                                     stateToString(context->idom_all_state.houseState));
    }
}

int iDomTOOLS::MPD_getVolume(thread_data *context)
{
    return context->ptr_MPD_info->volume;
}
