#ifndef MPD_CLI_H
#define MPD_CLI_H

#include <iostream>
#include <libmpd-1.0/libmpd/debug_printf.h>
#include <libmpd-1.0/libmpd/libmpd.h>

#include "../iDom_server_OOP.h"

void main_mpd_cli(thread_data *my_data, const std::string &threadName);
void updatePlayList(MpdObj *mi, thread_data *my_data);
/////////////// LCD

static std::string _msg = "null";

///////////////////////////////////////////

#endif // MPD_CLI_H
