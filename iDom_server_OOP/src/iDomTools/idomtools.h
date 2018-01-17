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

struct thread_data;
struct LED_Strip;

enum class PIN_STATE{
    LOW_STATE = 0,
    HIGH_STATE = 1,
    UNKNOWN_STATE = 2
};

enum class USER_LEVEL{
    ROOT,
    USER
};

class iDomTOOLS
{
    THERMOMETER_CONTAINER allThermometer;
    THERMOMETER_CONTAINER allThermometerUpdate;

    thread_data *my_data;
    SunRiseSet sun;
    std::string key;
    viber_API m_viber;
    FACEBOOK_API m_facebook;
    std::mutex m_msgMutex;

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
    //////////////////////////// weather /////////////////////////
    std::string getSunrise(bool extend = false);
    std::string getSunset(bool extend = false);
    Clock getSunsetClock();
    Clock getSunriseClock();
    std::string getDayLenght(bool extend = false);
    std::string getSystemInfo();
    std::string getWeatherEvent(std::string city, unsigned int radius );

    void textToSpeach(std::vector <std::string> *textVector);
    std::string getTextToSpeach();

    std::vector <std::string> getTemperature();
    std::string getTemperatureString();
    std::string getSmog();
    void send_temperature_thingSpeak();
    static size_t  WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static std::string find_tag (const std::string &temp);

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
    std::string ledOn(LED_Strip ledColor);
    ////////////////////   ALERT  //////////////////////////
    void checkAlarm();
    //////////////////// MPD part //////////////////////////
    static void MPD_play();
    static void MPD_stop();
    static void MPD_next();
    static void MPD_prev();
    static void MPD_pause();
    static void MPD_volumeUp();
    static void MPD_volumeDown();
    static void MPD_volumeSet(thread_data* my_data, int vol);
    static void MPD_playId(thread_data* my_data,int id);
    static int  MPD_getVolume(thread_data *my_data);
};

#endif // IDOMTOOLS_H
