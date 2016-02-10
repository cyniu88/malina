#include <stdio.h>
#include <curl/curl.h>
#include <time.h>
#include <iostream>
int main(void)
{
    time_t act_time;
    struct tm * act_date;
    int minuty =0;
    while (1) {
    time(&act_time);

    act_date = localtime(&act_time);
    std::cout << "dzien tygodnia " << act_date->tm_wday << " godzina " << act_date->tm_hour <<" minuta " << act_date->tm_min <<std::endl;
    std::cin >>act_date->tm_min;
    std::cout << "modulo: " << (minuty % 18) << std::endl;

}

  return 0;
}
