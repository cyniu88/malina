#pragma once

#include <map>
#include <string>
#include <chrono>
#include <memory>

#include "idomtools_interface.h"
#include "../libs/sunrise-sunset/sunriseset.h"
#include "../libs/viberAPI/viber_api.h"
#include "../dbClient/db-client-factory.hpp"
#include "../dbClient/DB_DATA.hpp"

struct thread_context;
class RADIO_BUTTON;

class iDomTOOLS : public iDomTOOLS_INTERFACE, iDom_API
{
protected:
    THERMOMETER_CONTAINER m_allThermometer;
    THERMOMETER_CONTAINER m_allThermometerUpdate;
    thread_context *context;
    int m_timezone;
    SunRiseSet m_sun;
    std::string m_key;
    std::string m_key2;
    viber_API m_viber;
    bool m_viber_notif = true;
    std::mutex m_msgMutex;
    std::vector<RADIO_BUTTON *> m_buttonPointerVector;
    Clock m_lastButton433MHzLockUnlockTime;
    int m_restartAlarmRFLink = 0;
    std::vector<std::string> textToSpeachVector;

public:
    explicit iDomTOOLS(thread_context *myData);
    ~iDomTOOLS();

    TEMPERATURE_STATE getTHERMOMETER_CONTAINERlastState(const std::string &name) override;
    TEMPERATURE_STATE hasTemperatureChange(const std::string &thermometerName, double reference, double histereza) override;
    void sendSMSifTempChanged(const std::string &thermomethernName, int reference) override;
    std::string getThermoStats(const std::string &name) override;
    void updateTemperatureStats() override;

    void turnOnSpeakers() override;
    void turnOffSpeakers() override;
    void turnOnOff230vOutdoor() override;
    void turnOnOff433MHzSwitch(const std::string &name) override;
    void turnOn433MHzSwitch(std::string name) override;
    void turnOff433MHzSwitch(std::string name) override;
    void runOnSunset() override;
    void runOnSunrise() override;
    void lockHome() override;
    void unlockHome() override;

    void buttonLockHome() override;
    void buttonUnlockHome() override;
    //////////////////////////// useful /////////////////////////
    bool isItDay() override;
    std::string getAllDataSunriseSunset() override;
    //////////////////////////// lightning ////////////////////////
    void checkLightning() override;

    //////////////////////////// weather /////////////////////////
    std::string getSunrise(bool extend = false) override;
    std::string getSunset(bool extend = false) override;
    Clock getSunsetClock() override;
    Clock getSunriseClock() override;
    std::string getDayLenght(bool extend = false) override;
    std::string getWeatherEvent(const std::string &city, unsigned int radius) override;

    WEATHER_DATABASE getAlert() override;

    void textToSpeach(std::vector<std::string> *textVector) override;
    std::string getTextToSpeach() override;

    std::vector<std::string> getTemperature() override;
    std::string getTemperatureString() override;
    std::optional<std::string> getSmog() override;
    void send_data_to_thingSpeak() override;
    void send_data_to_influxdb() override;
    std::string getFloorTemp() override;
    void uploadRecuperatorData() override;

    //////////////////// viber msg /////////////////////////
    nlohmann::json sendViberMsg(const std::string &msg,
                                const std::string &receiver,
                                const std::string &senderName,
                                const std::string &accessToken = "NULL",
                                const std::string &url = "NULL") override;
    nlohmann::json sendViberPicture(const std::string &msg,
                                    const std::string &image,
                                    const std::string &receiver,
                                    const std::string &senderName,
                                    const std::string &accessToken = "NULL",
                                    const std::string &url = "NULL") override;
    nlohmann::json sendViberUrl(const std::string &msg,
                                const std::string &url2,
                                const std::string &receiver,
                                const std::string &senderName,
                                const std::string &accessToken = "NULL",
                                const std::string &url = "NULL") override;
    STATE sendViberMsgBool(const std::string &msg,
                           const std::string &receiver,
                           const std::string &senderName,
                           const std::string &accessToken = "NULL",
                           const std::string &url = "NULL") override;
    STATE sendViberPictureBool(const std::string &msg,
                               const std::string &image,
                               const std::string &receiver,
                               const std::string &senderName,
                               const std::string &accessToken = "NULL",
                               const std::string &url = "NULL") override;

    //////////////////// LED part //////////////////////////
    std::string ledOFF() override;
    std::string ledClear() override;
    std::string ledClear(unsigned int from, unsigned int to) override;
    std::string ledOn() override;
    //////////////////// ALERT //////////////////////////
    void checkAlarm() override;
    //////////////////// MPD part //////////////////////////
    void MPD_play(thread_context *context) override;
    void MPD_stop() override;
    void MPD_next() override;
    void MPD_prev() override;
    void MPD_pause() override;
    void MPD_volumeUp() override;
    void MPD_volumeDown() override;
    void MPD_volumeSet(thread_context *context, int vol) override;
    void MPD_play(thread_context *context, int id) override;
    int MPD_getVolume(thread_context *context) override;

    /////////////////////// SAVE STATE iDOM ////////////////
    void saveState_iDom(const bool &started) override;
    void readState_iDom(nlohmann::json jj) override;

    /////////////////////// system linux ///////////////////
    std::string getSystemInfo() override;
    /////////////////////// KODI ////////////////////////
    std::string startKodi_Thread() override;
    /////////////////////// WiFi //////////////////////////
    STATE m_anyClientWiFi = STATE::UNKNOWN;
    void wifiClientConnected() override;
    void wifiClientDisconnected() override;

    /////////////////////// health check //////////////////
    void healthCheck() override;

    ////////////////////// system /////////////////////////
    void close_iDomServer() override;
    void reloadSoft_iDomServer() override;
    void reloadHard_iDomServer() override;
    void raspberryReboot() override;
    void uploadRamCpuUsage() override;

    //////////////////////// iDom_API /////////////////////
    std::string dump() const override;

    //////////////////////// link generator ///////////////
    std::string getLink(std::vector<std::string> v) override;

    //////////////////  doorbell  ////////////////////////
    void doorbellDingDong() override;

    void runCommandFromJson(const std::vector<std::string> &jj) override;
};

