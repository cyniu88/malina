#include "idomtools.h"
#include "../blockQueue/blockqueue.h"

void iDomTOOLS::MPD_play()
{
    blockQueue _q;
    _q._add('t');
}

void iDomTOOLS::MPD_stop()
{
    blockQueue _q;
    _q._add('P');
}

void iDomTOOLS::MPD_next()
{
    blockQueue _q;
    _q._add('D');
}

void iDomTOOLS::MPD_prev()
{
    blockQueue _q;
    _q._add('U');
}

void iDomTOOLS::MPD_pause()
{
    blockQueue _q;
    _q._add('A');
}

void iDomTOOLS::MPD_volumeUp()
{
    blockQueue _q;
    _q._add('+');
}

void iDomTOOLS::MPD_volumeDown()
{
    blockQueue _q;
    _q._add('-');
}

void iDomTOOLS::MPD_volumeSet(thread_data *my_data, int vol)
{
    my_data->ptr_MPD_info->volume = vol;
    blockQueue _q;
    _q._add('%');
}

void iDomTOOLS::MPD_playId(thread_data *my_data, int id)
{
    my_data->ptr_MPD_info->currentSongID = id;
    blockQueue _q;
    _q._add('I');
}
