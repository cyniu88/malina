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
#include "../blockQueue/blockqueue.h"   //brak
//#include "../wiadomosc/wiadomosc..h"
//#include "../functions/functions..h"
//#include "../logger/logger..hpp"
#include "../c_irda_logic/c_irda_logic.h"
#include "../LCD_c/lcd_c.h"     //brak
//#include "../parser/parser..hpp"
#include "../c_connection/c_connection.h"
#include"../CRON/cron.hpp"
//extern bool go_while;
//extern Logger log_file_mutex;
void  main_mpd_cli(thread_data *my_data );
void  updatePlayList(MpdObj *mi, thread_data *my_data);
/////////////// LCD

static std::string _msg="null";

///////////////////////////////////////////


#endif // MPD_CLI_H
