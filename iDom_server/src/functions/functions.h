#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <stdlib.h>

std::string conv_dns (std::string temp);

void ChangeEndianness(int32_t * value, int MAX_MSG_LEN_INT);
void binary(int val);

#endif // FUNCTIONS_H
