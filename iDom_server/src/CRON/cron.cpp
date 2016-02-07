#include "cron.hpp"
// api key  47XSQ0J9CPJ4BO2O
CRON::CRON(thread_data *my_data)
{
    this->my_data = my_data;
    this->check_temperature=TRUE;
}


void CRON::send_temperature_thingSpeak(std::string key)
{
    CURL *curl;
    CURLcode res;


    std::string addres = "api.thingspeak.com//update?key=";
    addres+=key;
    addres+="&field1=";
    addres+= send_to_arduino(my_data,"temperature:2;");
    addres.erase(addres.size()-2,addres.size());
    std::cout << addres << std::endl;
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, addres.c_str());


        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

}

void CRON::run()
{
    time_t act_time;
    struct tm * act_date;

    while (go_while) {

        sleep(10);
        time(&act_time);

        act_date = localtime(&act_time);
        //std::cout << "dzien tygodnia " << act_date->tm_wday << " godzina " << act_date->tm_hour <<" minuta " << act_date->tm_min <<std::endl;
        if (act_date->tm_min == 5 || act_date->tm_min == 35 )
        {
            if (check_temperature==TRUE){
                send_temperature_thingSpeak("47XSQ0J9CPJ4BO2O");
                std::cout << " wysylam temperature" <<std::endl;

            }
            check_temperature=FALSE;
        }
        else
        {
            check_temperature=TRUE;
        }
    }
}
