#ifndef IDOMTOOLS_H
#define IDOMTOOLS_H

#include <map>
#include <string>
#include <chrono>

#include "idomtools_useful.h"
#include "json.hpp"
#include "lightning.h"

#include "../iDom_server_OOP.h"
#include "../THERMOMETER_CONTAINER/thermometer_container.h"

#include "../../libs/sunrise-sunset/sunriseset.h"
#include "../../libs/viberAPI/viber_api.h"
#include "../../libs/facebookAPI/facebookAPI.h"
#include "../../libs/Statistic/statistic.h"

#include "../idom_api.h"

struct thread_data;
struct LED_Strip;
class RADIO_BUTTON;

class iDomTOOLS: public iDom_API
{
#ifdef BT_TEST
public:
#endif
    THERMOMETER_CONTAINER m_allThermometer;
    THERMOMETER_CONTAINER m_allThermometerUpdate;
private:
    std::string m_className;
    thread_data *my_data;
    int m_timezone;
    SunRiseSet m_sun;
    std::string m_key;
    viber_API m_viber;
    FACEBOOK_API m_facebook;
    std::mutex m_msgMutex;
    std::vector<RADIO_BUTTON*> m_buttonPointerVector;
    Clock m_lastButton433MHzLockUnlockTime;

    int m_restartAlarmRFLink = 0;

public:
    std::vector <std::string> textToSpeachVector;
    iDomTOOLS(thread_data *myData);
    ~iDomTOOLS();

    TEMPERATURE_STATE hasTemperatureChange(const std::string &thermometerName, double reference, double histereza);
    void sendSMSifTempChanged(const std::string& thermomethernName, int reference);
    std::string getThermoStats(const std::string &name);
    void updateTemperatureStats();

    static void turnOnSpeakers();
    static void turnOffSpeakers();
    void turnOnPrinter();
    void turnOffPrinter();
    static PIN_STATE getPinState(int pin_number);
    void turnOnOffPrinter();
    void turnOnOff433MHzSwitch(const std::string &name);
    void turnOn433MHzSwitch(std::string name);
    void turnOff433MHzSwitch(std::string name);
    void runOnSunset();
    void runOnSunrise();
    void lockHome();
    void unlockHome();
    //////////////////////////// 433MHz action ///////////////////
    void switchActionOnLockHome();
    void switchActionOnUnlockHome();
    //////////////////////////// 433MHz button ///////////////////

    std::string buttonPressed(const std::string &id);
    void button433MHzPressedAction(const std::string &name);
    void button433mhzLockerPressed(RADIO_BUTTON* radioButton);
    void button433mhzNightLightPressed(RADIO_BUTTON* radioButton);

    void buttonLockHome();
    void buttonUnlockHome();
    //////////////////////////// useful /////////////////////////
    bool isItDay();
    std::string getAllDataSunrisesunset();
    //////////////////////////// lightning ////////////////////////
    /// \brief lightning
private:
    CARDINAL_DIRECTIONS::ALARM_INFO m_lightningStruct;
public:
    LIGHTNING m_lightning;
    std::mutex m_lightningMutex;
    CARDINAL_DIRECTIONS::ALARM_INFO getLightningStruct();
    void setLightningStruct(CARDINAL_DIRECTIONS::ALARM_INFO& s);
    void checkLightning();
    //////////////////////////// weather /////////////////////////
    std::string getSunrise(bool extend = false);
    std::string getSunset(bool extend = false);
    Clock getSunsetClock();
    Clock getSunriseClock();
    std::string getDayLenght(bool extend = false);
    std::string getWeatherEvent(const std::string &city, unsigned int radius );

    std::vector<WEATHER_ALER> getAlert(std::string data);

    void textToSpeach(std::vector <std::string> *textVector);
    std::string getTextToSpeach();

    std::vector <std::string> getTemperature();
    std::string getTemperatureString();
    std::string getSmog();
    void send_temperature_thingSpeak();

    //////////////////// camera part ///////////////////////
    void cameraLedON(const std::string &link);
    void cameraLedOFF(const std::string &link);
    //////////////////// viber msg /////////////////////////
    nlohmann::json sendViberMsg(const std::string& msg,
                                const std::string& receiver,
                                const std::string& senderName,
                                const std::string &accessToken = "NULL",
                                const std::string &url = "NULL");
    nlohmann::json sendViberPicture(const std::string& msg,
                                    const std::string& image,
                                    const std::string& receiver,
                                    const std::string& senderName,
                                    const std::string &accessToken = "NULL",
                                    const std::string &url = "NULL");
    STATE sendViberMsgBool(const std::string& msg,
                           const std::string& receiver,
                           const std::string& senderName,
                           const std::string &accessToken = "NULL",
                           const std::string &url = "NULL");
    STATE sendViberPictureBool(const std::string& msg,
                               const std::string& image,
                               const std::string& receiver,
                               const std::string& senderName,
                               const std::string &accessToken = "NULL",
                               const std::string &url = "NULL");
    //////////////////// facebook //////////////////////////
    std::string postOnFacebook(const std::string& msg, const std::string& image ="NULL");

    //////////////////// LED part //////////////////////////
    std::string ledOFF();
    std::string ledClear();
    std::string ledClear(unsigned int from, unsigned int to);
    std::string ledOn(const LED_Strip& ledColor, unsigned int from = 0, unsigned int to = 60);
    //////////////////// ALERT //////////////////////////
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

    /////////////////////// SAVE STATE iDOM ////////////////
    void saveState_iDom();
    void readState_iDom(nlohmann::json jj);

    /////////////////////// system linux ///////////////////
    std::string getSystemInfo();
    /////////////////////// KODI ////////////////////////
    std::string startKodi_Thread();
    /////////////////////// WiFi //////////////////////////
    STATE m_anyClientWiFi = STATE::UNKNOWN;
    void wifiClientConnected();
    void wifiClientDisconnected();

    /////////////////////// health check //////////////////
    void healthCheck();

    ////////////////////// system /////////////////////////

    void close_iDomServer [[ noreturn ]]();
    void reloadSoft_iDomServer[[ noreturn ]] ();
    void reloadHard_iDomServer[[ noreturn ]] ();
    //////////////////////// iDom_API /////////////////////
    std::string dump() const;
};

#endif // IDOMTOOLS_H
