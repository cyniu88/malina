#ifndef __AVRCOMPAT_H__
#define __AVRCOMPAT_H_

void ltoa(long val, char* buf, int base);
void ultoa(unsigned long val, char* buf, int base);
void itoa(int val, char* buf, int base);
void utoa(unsigned int val, char* buf, int base);

#endif

