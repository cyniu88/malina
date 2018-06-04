#ifndef IDOMTOOLS_H
#define IDOMTOOLS_H

#include <map>
#include <string>
#include <chrono>
#include "../iDom_server_OOP.h"
#include "../../libs/sunrise-sunset/sunriseset.h"
#include "../../libs/viberAPI/viber_api.h"
#include "../../libs/facebookAPI/facebookAPI.h"
#include "../../libs/Statistic/statistic.h"
#include "sys/sysinfo.h"
#include "../THERMOMETER_CONTAINER/thermometer_container.h"
#include "idomtools_useful.h"
#include "json.hpp"

struct thread_data;
struct LED_Strip;
class RADIO_BUTTON;
class CARDINAL_DIRECTIONS{
public:
    enum CARDINAL_DIRECTIONS_ENUM{
        /*
           N
        NNW NNE
      NW       NE
   WNW		     ENE
 W				    E
   WSW			 ESE
      SW	   SE
        SSW	 SSE
           S
    */

        N = 1,NNE,NE,ENE,E,ESE,SE,SSE,S,SSW,SW,WSW,W,WNW,NW,NNW,ERROR
    };
    static CARDINAL_DIRECTIONS_ENUM stringToCardinalDirectionsEnum(std::string s){
             if (s == "N")      return CARDINAL_DIRECTIONS_ENUM::N;
        else if (s == "NNE")    return CARDINAL_DIRECTIONS_ENUM::NNE;
        else if (s == " NE")    return CARDINAL_DIRECTIONS_ENUM::NE;
        else if (s == "ENE")    return CARDINAL_DIRECTIONS_ENUM::ENE;
        else if (s == "E")      return CARDINAL_DIRECTIONS_ENUM::E;
        else if (s == "ESE")    return CARDINAL_DIRECTIONS_ENUM::ESE;
        else if (s == "SE")     return CARDINAL_DIRECTIONS_ENUM::SE;
        else if (s == "SSE")    return CARDINAL_DIRECTIONS_ENUM::SSE;
        else if (s == "S")      return CARDINAL_DIRECTIONS_ENUM::S;
        else if (s == "SSW")    return CARDINAL_DIRECTIONS_ENUM::SSW;
        else if (s == "SW")     return CARDINAL_DIRECTIONS_ENUM::SW;
        else if (s == "WSW")    return CARDINAL_DIRECTIONS_ENUM::WSW;
        else if (s == "W")      return CARDINAL_DIRECTIONS_ENUM::W;
        else if (s == "WNW")    return CARDINAL_DIRECTIONS_ENUM::WNW;
        else if (s == "NW")     return CARDINAL_DIRECTIONS_ENUM::NW;
        else if (s == "NNW")    return CARDINAL_DIRECTIONS_ENUM::NNW;
        else                    return CARDINAL_DIRECTIONS_ENUM::ERROR;
    }

    static std::string cardinalDirectionsEnumToString(CARDINAL_DIRECTIONS_ENUM e){
        switch (e){
        case CARDINAL_DIRECTIONS_ENUM::N:
            return "N";
        case CARDINAL_DIRECTIONS_ENUM::NNE:
            return "NNE";
        case CARDINAL_DIRECTIONS_ENUM:: NE:
            return "NE";
        case CARDINAL_DIRECTIONS_ENUM::ENE:
            return "ENE";
        case CARDINAL_DIRECTIONS_ENUM::E:
            return "E";
        case CARDINAL_DIRECTIONS_ENUM::ESE:
            return "ESE";
        case CARDINAL_DIRECTIONS_ENUM::SE:
            return "SE";
        case CARDINAL_DIRECTIONS_ENUM::SSE:
            return "SSE";
        case CARDINAL_DIRECTIONS_ENUM::S:
            return "S";
        case CARDINAL_DIRECTIONS_ENUM::SSW:
            return "SSW";
        case CARDINAL_DIRECTIONS_ENUM::SW:
            return "SW";
        case CARDINAL_DIRECTIONS_ENUM::WSW:
            return "WSW";
        case CARDINAL_DIRECTIONS_ENUM::W:
            return "W";
        case CARDINAL_DIRECTIONS_ENUM::WNW:
            return "WNW";
        case CARDINAL_DIRECTIONS_ENUM::NW:
            return "NW";
        case CARDINAL_DIRECTIONS_ENUM::NNW:
            return "NNW";
        default:
            return "UNKNOWN DIRECTION";
        }
    }
};

class iDomTOOLS
{
#ifdef BT_TEST
public:
#endif
    THERMOMETER_CONTAINER allThermometer;
    THERMOMETER_CONTAINER allThermometerUpdate;
private:
    thread_data *my_data;
    SunRiseSet sun;
    std::string key;
    viber_API m_viber;
    FACEBOOK_API m_facebook;
    std::mutex m_msgMutex;
    std::vector<RADIO_BUTTON*> buttonPointerVector;
    Clock lastButton433MHzLockUnlockTime;


public:
    std::vector <std::string> textToSpeachVector;
    iDomTOOLS(thread_data *myData);

    TEMPERATURE_STATE hasTemperatureChange(std::string thermometerName, double reference, double histereza);
    void sendSMSifTempChanged(std::string thermomethernName, int reference);
    std::string getThermoStats(std::string name);
    void updateTemperatureStats();

    static void turnOnSpeakers();
    static void turnOffSpeakers();
    void turnOnPrinter();
    void turnOffPrinter();
    static PIN_STATE getPinState(int pin_number);
    void turnOnOffPrinter();
    void turnOnOff433MHzSwitch(std::string name);
    void turnOn433MHzSwitch(std::string name);
    void turnOff433MHzSwitch(std::string name);
    void runOnSunset();
    void runOnSunrise();
    void lockHome();
    void unlockHome();
    //////////////////////////// 433MHz button ///////////////////

    std::string buttonPressed(std::string id);
    void button433MHzPressedAction(std::string name);
    void button433mhzLockerPressed(RADIO_BUTTON* radioButton);

    void buttonLockHome();
    void buttonUnlockHome();
    //////////////////////////// useful  /////////////////////////
    bool isItDay();
    //////////////////////////// lightning ////////////////////////
    void lightningAlert(nlohmann::json jj);
    //////////////////////////// weather /////////////////////////
    std::string getSunrise(bool extend = false);
    std::string getSunset(bool extend = false);
    Clock getSunsetClock();
    Clock getSunriseClock();
    std::string getDayLenght(bool extend = false);
    std::string getSystemInfo();
    std::string getWeatherEvent(std::string city, unsigned int radius );

    std::vector<WEATHER_ALER> getAlert(std::string data);

    void textToSpeach(std::vector <std::string> *textVector);
    std::string getTextToSpeach();

    std::vector <std::string> getTemperature();
    std::string getTemperatureString();
    std::string getSmog();
    void send_temperature_thingSpeak();
    std::string sendSMStoPlusGSM(std::string login,
                                 std::string pass,
                                 std::string number,
                                 std::string msg,
                                 int silentFrom = 0,
                                 int silentTo =0);
    //////////////////// camera part ///////////////////////
    void cameraLedON(std::string link);
    void cameraLedOFF(std::string link);
    //////////////////// viber msg /////////////////////////
    std::string sendViberMsg(std::string msg,
                             std::string receiver,
                             std::string senderName,
                             std::string accessToken = "NULL",
                             std::string url = "NULL");
    std::string sendViberPicture(std::string msg,
                                 std::string image,
                                 std::string receiver,
                                 std::string senderName,
                                 std::string accessToken = "NULL",
                                 std::string url = "NULL");
    //////////////////// facebook //////////////////////////
    std::string postOnFacebook(std::string msg, std::string image ="NULL");

    //////////////////// LED part //////////////////////////
    std::string ledOFF();
    std::string ledClear();
    std::string ledClear(unsigned int from, unsigned int to);
    std::string ledOn(LED_Strip ledColor, unsigned int from = 0, unsigned int to = 60);
    ////////////////////   ALERT  //////////////////////////
    void checkAlarm();
    //////////////////// MPD part //////////////////////////
    static void MPD_play(thread_data* my_data);
    static void MPD_stop();
    static void MPD_next();
    static void MPD_prev();
    static void MPD_pause();
    static void MPD_volumeUp();
    static void MPD_volumeDown();
    static void MPD_volumeSet(thread_data* my_data, int vol);
    static void MPD_play(thread_data* my_data,int id);
    static int  MPD_getVolume(thread_data *my_data);
};

#endif // IDOMTOOLS_H
