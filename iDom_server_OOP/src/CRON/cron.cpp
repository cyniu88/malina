#include "cron.hpp"
// api key  47XSQ0J9CPJ4BO2O
CRON::CRON(thread_data *my_data)
{
    this->my_data = my_data;
    this->check_temperature=TRUE;
}


void CRON::run()
{
    time_t act_time;
    struct tm * act_date;
    int min =0 ;
    char buffer[5];

    while (go_while) {
        sleep(10);
        time(&act_time);
        act_date = localtime(&act_time);
        //std::cout << "dzien tygodnia " << act_date->tm_wday << " godzina " << act_date->tm_hour <<" minuta " << act_date->tm_min <<std::endl;
        if (act_date->tm_min % 15 == 0 )
        {
            if (check_temperature==TRUE){                
                my_data->main_iDomTools->send_temperature_thingSpeak();
            }
            check_temperature=FALSE;
        }
        else
        {
            check_temperature=TRUE;
        }
        if (min != act_date->tm_min  ){

            strftime(buffer,5,"%H%M",act_date);
            // serial_ardu.print(buffer );
            useful_F::send_to_arduino_clock(my_data,buffer);
            min = act_date->tm_min;
            //std::cout << "czas "<< buffer <<" arduino "<<send_to_arduino_clock(my_data,buffer)<< std::endl;
        }
    }
}
