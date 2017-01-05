#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include "../iDom_server_OOP.h"
#include "../c_connection/c_connection.h"

std::string conv_dns (std::string temp);

void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
void binary(int val);
//std::string intToStr(int n);
std::string send_to_arduino (thread_data *my_data_logic, std::string msg);
std::string send_to_arduino_clock (thread_data *my_data_logic, std::string msg);
std::string  sek_to_uptime(long long secy );
void sleeper_mpd(thread_data  *my_data);
void tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text);
std::string  RSHash(int offset =0);
void write_to_mkfifo(std::string msg);
std::string read_from_mkfifo();
std::string l_send_file(std::string path, std::string find  , bool reverse =false );
std::vector<std::string> split(const std::string& s, char separator );
#endif // FUNCTIONS_H
