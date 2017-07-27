#include <algorithm>
#include "idomtools.h"
#include "../functions/functions.h"
#include "../CRON/cron.hpp"

iDomTOOLS::iDomTOOLS(thread_data *myData) : key(myData->server_settings->TS_KEY)
{
    my_data = myData;
    thermometer["inside"];
    thermometer["outside"];
}

void iDomTOOLS::setTemperature(std::string name, float value)
{
    auto cur = thermometer.find(name);
    cur->second.oldTemp = cur->second.newTemp ;
    cur->second.newTemp = value;
    //TODO   dodac ustawianie temperetury i daty
}

TEMPERATURE_STATE iDomTOOLS::hasTemperatureChange(std::string thermometerName, double reference, double histereza )
{
    reference+=0.0055;
    auto cur = thermometer.find(thermometerName);

    if (cur->second.newTemp >= reference + histereza &&
            cur->second.oldTemp  < reference + histereza &&
            cur->second.lastState != TEMPERATURE_STATE::Over)
    {
        my_data->myEventHandler.run("test")->addEvent("over: new "+std::to_string(cur->second.newTemp)+" old: "
                                                      +std::to_string(cur->second.oldTemp)+" ref: "+std::to_string(reference));
        cur->second.lastState = TEMPERATURE_STATE::Over;
        return TEMPERATURE_STATE::Over;
    }
    else if (cur->second.newTemp <= reference - histereza &&
             cur->second.oldTemp >  reference - histereza &&
             cur->second.lastState != TEMPERATURE_STATE::Under)
    {
        my_data->myEventHandler.run("test")->addEvent("under: new "+std::to_string(cur->second.newTemp)+" old: "
                                                      +std::to_string(cur->second.oldTemp)+" ref: "+std::to_string(reference));
        cur->second.lastState = TEMPERATURE_STATE::Under;
        return TEMPERATURE_STATE::Under;
    }
    else
    {
        my_data->myEventHandler.run("test")->addEvent("noChanges: new "+std::to_string(cur->second.newTemp)+" old: "
                                                      +std::to_string(cur->second.oldTemp)+" ref: "+std::to_string(reference));

        return TEMPERATURE_STATE::NoChanges;
    }
    my_data->myEventHandler.run("test")->addEvent("unknown: new "+std::to_string(cur->second.newTemp)+" old: "
                                                  +std::to_string(cur->second.oldTemp)+" ref: "+std::to_string(reference));

    return TEMPERATURE_STATE::Unknown;
}

void iDomTOOLS::sendSMSifTempChanged(std::string thermomethernName, int reference)
{
    TEMPERATURE_STATE status = hasTemperatureChange(thermomethernName,reference,0.5);
    std::string m = "temperature "+thermomethernName+" over ^"+ std::to_string(reference);

    if (status == TEMPERATURE_STATE::Over){
        my_data->myEventHandler.run("temperature")->addEvent(m);
        sendSMStoPlusGSM("yanosik-info","yanosik24","782490815",m);
    }
    else if (status == TEMPERATURE_STATE::Under){
        m ="temperature " + thermomethernName+" under \\/"+std::to_string(reference);
        my_data->myEventHandler.run("temperature")->addEvent(m);
        sendSMStoPlusGSM("yanosik-info","yanosik24","782490815",m);
    }
    else{
        //my_data->myEventHandler.run("unknown")->addEvent("temperatura nie przeszla przez "+std::to_string(reference));
    }
}

void iDomTOOLS::turnOnSpeakers()
{
    digitalWrite(iDomConst::GPIO_SPIK, LOW);
}

void iDomTOOLS::turnOffSpeakers()
{
    digitalWrite(iDomConst::GPIO_SPIK, HIGH);
}

std::string iDomTOOLS::getSunrise(bool extend )
{
    Clock tt = sun.getSunRise();
    if (extend == true){
        return "Sunrise time: "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSunset(bool extend )
{
    Clock tt = sun.getSunSet();
    if (extend == true){
        return  "Sunset time: "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getDayLenght(bool extend )
{
    Clock tt = sun.getDayLength();
    if (extend == true){
        return "Day Lenght : "+tt.getString();
    }
    return tt.getString();
}

std::string iDomTOOLS::getSystemInfo()
{
    struct sysinfo info;
    sysinfo(&info);
    long input_seconds = info.uptime;
    auto days = input_seconds / 60 / 60 / 24;
    auto hours = (input_seconds / 60 / 60) % 24;
    auto minutes = (input_seconds / 60) % 60;
    auto seconds = input_seconds % 60;
    
    std::string  ret;
    ret=  "System uptime: " + std::to_string(days)+" day ";
    ret+=  std::to_string(hours) +" hours " ;
    ret+=  std::to_string(minutes)+ " minutes ";
    ret+=  std::to_string(seconds)+ " seconds ";
    ret.append("\n");
    ret+= "Load: ";
    ret+= std::to_string(info.loads[0]/65536)+"% - 1 min, "+ std::to_string(info.loads[1]/65536)+"% - 5 min, "+
            std::to_string(info.loads[2]/65536)+"% - 15 min.\n ";

    std::stringstream k;
    k <<"load average : "<<(info.loads[0] )
            <<" 5min: "  <<(info.loads[1] )
            <<" 15min: " <<(info.loads[2] )<<std::endl;
    puts(k.str().c_str());
    return ret;
}

void iDomTOOLS::textToSpeach(std::vector<std::string> *textVector)
{
    if (textVector->empty() ){
        return;
    }
    std::string txt;
    
    for (auto a : *textVector){
        txt += a;
    }
    /////////// start thread  TTS - python use ////////////////////////
    std::string command = " python /home/pi/programowanie/PYTON/gadacz.py ";
    command += "\""+ txt +"\"";
    if(my_data->ptr_MPD_info->isPlay){

    }
    else {
        turnOnSpeakers();
    }
    system(command.c_str());
    if(my_data->ptr_MPD_info->isPlay){

    }
    else {
        turnOffSpeakers();
    }
}

std::string iDomTOOLS::getTextToSpeach()
{
    std::vector<std::string> dayL = useful_F::split(getDayLenght(),':');
    std::string  text;
    std::string smogText = getSmog();
    int smogInt = std::stoi(smogText);
    text =  "Godzina: "+ getTime().getString();
    text += ". \nWschód słońca: "+getSunrise();
    text += ". \nZachód słońca: "+getSunset();
    text += ". \nDługość dnia: "+ dayL[0]+" godzin "+dayL[1]+" minut";
    text +=". \n";
    dayL = getTemperature();
    text += "Temperatura na zewnątrz: "+ dayL[1]+" stopnia. \n";
    text += "Temperatura w pokoju: "+ dayL[0]+" stopnia. \n";
    text += "Smog: "+ smogText +" mg/m^3. \n";
    if (smogInt > 50){
        int result = smogInt *2 ;
        text += "UWAGA! Maksymalna wartość przekroczona "+ std::to_string(result)+"%.";
    }
    return text;
}

std::vector<std::string> iDomTOOLS::getTemperature()
{
    std::vector<std::string>  vect = useful_F::split(useful_F::send_to_arduino(my_data,"temperature:22;"),':');
    return vect;
}

std::string iDomTOOLS::getTemperatureString()
{
    return useful_F::send_to_arduino(my_data,"temperature:22;");
}

Clock iDomTOOLS::getTime()
{
    Clock t;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    t.set( ltm->tm_hour ,ltm->tm_min );
    return t;
}

std::string iDomTOOLS::getSmog()
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
    try {
        readBuffer = readBuffer.substr(start, 40);
    }
    catch (...){
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() e getSmog() !!!!!!"<< std::endl;
        log_file_mutex.mutex_unlock();
    }

    readBuffer = find_tag(readBuffer);

    return readBuffer;
}

void iDomTOOLS::send_temperature_thingSpeak()
{
    CURL *curl;
    CURLcode res;
    std::vector<std::string> _temperature = getTemperature();
    std::string addres = "api.thingspeak.com/update?key=";
    addres+=key;
    addres+="&field1=";
    addres+= _temperature.at(0);
    // addres.erase(addres.size()-2,addres.size());
    addres+= "&field3="+_temperature.at(1);
    addres+="&field2="+getSmog();
    //////////////////////////////// pozyskanie temperatury
    ///
    ///
    //_temperature.erase(_temperature.size()-2,_temperature.size());
    std::string in = _temperature.at(0);
    std::string out = _temperature.at(1);

    setTemperature("inside",std::stod(in));
    setTemperature("outside",std::stod(out));
    sendSMSifTempChanged("outside",0);
    sendSMSifTempChanged("inside",24);
    //printf("o=inside: %f outside %f\n",   std::stof(in)   ,std::stof(out) );
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        std::string dummyString;
        curl_easy_setopt(curl, CURLOPT_URL, addres.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dummyString);

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

size_t iDomTOOLS::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string iDomTOOLS::find_tag(const std::string& temp)
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

std::string iDomTOOLS::sendSMStoPlusGSM(std::string login, std::string pass, std::string number,
                                        std::string msg,int silentFrom , int silentTo  )
{
    if (silentFrom !=0 && silentTo !=0){
        // TODO
    }
    std::replace(msg.begin(),msg.end(),' ','+');
    std::string address = "http://darsonserver.5v.pl/bramkaPlus?login=";
    address +=login+"&password="+pass+"&sender=iDom&number="+number+"&message="+msg;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
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
    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"wysłano SMSa otreśći: " <<  msg<<std::endl;
    log_file_mutex.mutex_unlock();
    return readBuffer +"\n"+address;
}

std::string iDomTOOLS::ledOFF()
{
    return useful_F::send_to_arduino(my_data,"LED_STOP:2;");
}

std::string iDomTOOLS::ledClear()
{
    return useful_F::send_to_arduino(my_data,"LED_CLEAR:2;");
}

std::string iDomTOOLS::ledOn(LED_Strip ledColor)
{
    return useful_F::send_to_arduino(my_data,ledColor.get());
}
