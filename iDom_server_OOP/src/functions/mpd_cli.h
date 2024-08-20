#pragma once

#include <iostream>
#include <libmpd-1.0/libmpd/debug_printf.h>
#include <libmpd-1.0/libmpd/libmpd.h>

#include "../iDom_server_OOP.h"

void main_mpd_cli(thread_context *context, const std::string &threadName);
void updatePlayList(MpdObj *mi, thread_context *context);
/////////////// LCD

static std::string _msg = "null";


