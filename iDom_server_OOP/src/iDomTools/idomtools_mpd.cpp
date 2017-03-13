#include "idomtools.h"
#include "../blockQueue/blockqueue.h"

void iDomTOOLS::MPD_play()
{
    blockQueue _q;
    _q._add(MPD_COMMAND::PLAY);
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

void iDomTOOLS::MPD_volumeSet(thread_data *my_data, int vol)
{
    my_data->ptr_MPD_info->volume = vol;
    blockQueue _q;
    _q._add(MPD_COMMAND::VOLSET);
}

void iDomTOOLS::MPD_playId(thread_data *my_data, int id)
{
    my_data->ptr_MPD_info->currentSongID = id;
    blockQueue _q;
    _q._add(MPD_COMMAND::PLAY_ID);
}
