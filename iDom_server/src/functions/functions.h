#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include "../iDom_server/src/iDom_server.h"
#include "../iDom_server/src/c_connection/c_connection.h"

std::string conv_dns (std::string temp);

void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
void binary(int val);
//std::string intToStr(int n);
std::string send_to_arduino (thread_data *my_data_logic, std::string msg);
std::string  sek_to_uptime(long long secy );
void *sleeper_mpd (void *data);
void tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text);
std::string  RSHash(int offset =0);
#endif // FUNCTIONS_H
