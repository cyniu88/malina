#include "cron.hpp"
// api key  47XSQ0J9CPJ4BO2O
CRON::CRON(thread_data *my_data)
{
    this->my_data = my_data;
    this->check_temperature=TRUE;
}

size_t CRON::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
std::string CRON::find_tag (std::string temp)
{
     std::string value="";
    for (unsigned int i = 0; i<temp.size();++i){

        if (temp.at(i) =='>')
        {  int z = i+1;
            while (temp.at(z)!='<')
            {
                value+= temp.at(z);
                ++z;
            }
            break;
        }
    }
    return value;
}

std::string CRON::smog( )
{

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "www.smog.krakow.pl");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    int start = readBuffer.find("<h2 class=\"polution\">");
    readBuffer = readBuffer.substr(start, 40);
    readBuffer = find_tag(readBuffer);

    return readBuffer;
}

void CRON::send_temperature_thingSpeak()
{
    CURL *curl;
    CURLcode res;
    std::string _temperature = send_to_arduino(my_data,"temperature:2;");

    std::string addres = "api.thingspeak.com/update?key=";
    addres+=key;
    addres+="&field1=";
    addres+= _temperature;
    addres.erase(addres.size()-2,addres.size());
    addres.insert(addres.find_last_of(':'),"&field3=");
    addres.erase(addres.find_last_of(':'),1);
    addres+="&field2="+smog();

    //////////////////////////////// pozyskanie temperatury
    ///
    ///
    _temperature.erase(_temperature.size()-2,_temperature.size());
    std::string in = _temperature.substr(0,_temperature.find_last_of(':'));
    std::string out = _temperature.substr(_temperature.find_last_of(':')+1,_temperature.size());

    my_data->main_iDomTools.setTemperature("inside",std::stof(in));
    my_data->main_iDomTools.setTemperature("outside",std::stof(out));
    printf("o=inside: %f outside %f\n",   std::stof(in)   ,std::stof(out) );
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
                send_temperature_thingSpeak();
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
            send_to_arduino_clock(my_data,buffer);
             min = act_date->tm_min;
             //std::cout << "czas "<< buffer <<" arduino "<<send_to_arduino_clock(my_data,buffer)<< std::endl;
        }
    }
}
