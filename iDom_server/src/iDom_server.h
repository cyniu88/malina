#ifndef GLOBAL_H
#define GLOBAL_H
#include <iostream>
#include <fstream>
#include <strings.h>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include <wiringPi.h>
 #include <lirc/lirc_client.h>

// MOJE BIBLIOTEKI

#include "serialib/serialib.h"
#include "parser/parser.hpp"
#include "wiadomosc/wiadomosc.h"
#include "c_connection/c_connection.h"
#include "functions/functions.h"
#include "logger/logger.hpp"
#include "c_master_irda/master_irda.h"
#include "functions/mpd_cli.h"
#include "blockQueue/blockqueue.h"
#include "functions/master_LCD.h"

char *  _logfile  = "/tmp/iDom_log.log";

Logger log_file_mutex(_logfile);

char buffer[20];

int max_msg = MAX_MSG_LEN*sizeof(int32_t);

bool go_while = true;

#endif // GLOBAL_H
