#ifndef MPD_CLI_H
#define MPD_CLI_H

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libmpd-1.0/libmpd/libmpd.h>
#include <libmpd-1.0/libmpd/debug_printf.h>
#include <wiringPi.h>
#include "../blockQueue/blockqueue.h"
#include "../wiadomosc/wiadomosc.h"
#include "../functions/functions.h"
#include "../logger/logger.hpp"
#include "master_LCD.h"
//#include "../iDom_server.h"
extern bool go_while;
extern Logger log_file_mutex;
void  *main_mpd_cli(void *data );

/////////////// LCD

#include <wiringPi.h>

#include <lcd.h>

#include <pcf8574.h>

using namespace std;
static int screen;
static std::string wiad = " KINIA";

//define new pins
#define AF_BASE 100

#define AF_RS   (AF_BASE + 0)
#define AF_RW   (AF_BASE + 1)
#define AF_E    (AF_BASE + 2)

#define AF_BL   (AF_BASE + 3)

#define AF_D1   (AF_BASE + 4)
#define AF_D2   (AF_BASE + 5)
#define AF_D3   (AF_BASE + 6)
#define AF_D4   (AF_BASE + 7)
///////////////////////////////////////////


#endif // MPD_CLI_H
