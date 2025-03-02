#pragma once

#include <optional>

#include "idomtools_useful.h"
#include "json.hpp"

#include "../THERMOMETER_CONTAINER/thermometer_container.h"
#include "../../libs/useful/useful.h"

struct thread_context;

class iDomTOOLS_INTERFACE
{

public:
    virtual ~iDomTOOLS_INTERFACE() = default;

    virtual TEMPERATURE_STATE getTHERMOMETER_CONTAINERlastState(const std::string &name) = 0;
    virtual TEMPERATURE_STATE hasTemperatureChange(const std::string &thermometerName, double reference, double histereza) = 0;
    virtual void sendSMSifTempChanged(const std::string &thermomethernName, int reference) = 0;
    virtual std::string getThermoStats(const std::string &name) = 0;
    virtual void updateTemperatureStats() = 0;

    virtual void turnOnSpeakers() = 0;
    virtual void turnOffSpeakers() = 0;
    virtual void turnOnOff230vOutdoor() = 0;
    virtual void turnOnOff433MHzSwitch(const std::string &name) = 0;
    virtual void turnOn433MHzSwitch(std::string name) = 0;
    virtual void turnOff433MHzSwitch(std::string name) = 0;
    virtual void runOnSunset() = 0;
    virtual void runOnSunrise() = 0;
    virtual void lockHome() = 0;
    virtual void unlockHome() = 0;

    virtual void buttonLockHome() = 0;
    virtual void buttonUnlockHome() = 0;
    //////////////////////////// useful /////////////////////////
    virtual bool isItDay() = 0;
    virtual std::string getAllDataSunriseSunset() = 0;
    //////////////////////////// lightning ////////////////////////
    virtual void checkLightning() = 0;

    //////////////////////////// weather /////////////////////////
    virtual std::string getSunrise(bool extend = false) = 0;
    virtual std::string getSunset(bool extend = false) = 0;
    virtual Clock getSunsetClock() = 0;
    virtual Clock getSunriseClock() = 0;
    virtual std::string getDayLenght(bool extend = false) = 0;
    virtual std::string getWeatherEvent(const std::string &city, unsigned int radius) = 0;

    virtual WEATHER_DATABASE getAlert() = 0;

    virtual void textToSpeach(std::vector<std::string> *textVector) = 0;
    virtual std::string getTextToSpeach() = 0;

    virtual std::vector<std::string> getTemperature() = 0;
    virtual std::string getTemperatureString() = 0;
    virtual std::optional<std::string> getSmog() = 0;
    virtual void send_data_to_thingSpeak() = 0;
    virtual void send_data_to_influxdb() = 0;
    virtual std::string getFloorTemp() = 0;
    virtual void uploadRecuperatorData() = 0;

    //////////////////// viber msg /////////////////////////
    virtual nlohmann::json sendViberMsg(const std::string &msg,
                                        const std::string &receiver,
                                        const std::string &senderName,
                                        const std::string &accessToken = "NULL",
                                        const std::string &url = "NULL") = 0;
    virtual nlohmann::json sendViberPicture(const std::string &msg,
                                            const std::string &image,
                                            const std::string &receiver,
                                            const std::string &senderName,
                                            const std::string &accessToken = "NULL",
                                            const std::string &url = "NULL") = 0;
    virtual nlohmann::json sendViberUrl(const std::string &msg,
                                        const std::string &url2,
                                        const std::string &receiver,
                                        const std::string &senderName,
                                        const std::string &accessToken = "NULL",
                                        const std::string &url = "NULL") = 0;
    virtual STATE sendViberMsgBool(const std::string &msg,
                                   const std::string &receiver,
                                   const std::string &senderName,
                                   const std::string &accessToken = "NULL",
                                   const std::string &url = "NULL") = 0;
    virtual STATE sendViberPictureBool(const std::string &msg,
                                       const std::string &image,
                                       const std::string &receiver,
                                       const std::string &senderName,
                                       const std::string &accessToken = "NULL",
                                       const std::string &url = "NULL") = 0;

    //////////////////// LED part //////////////////////////
    virtual std::string ledOFF() = 0;
    virtual std::string ledClear() = 0;
    virtual std::string ledClear(unsigned int from, unsigned int to) = 0;
    virtual std::string ledOn() = 0;
    //////////////////// ALERT //////////////////////////
    virtual void checkAlarm() = 0;
    //////////////////// MPD part //////////////////////////
    virtual void MPD_play(thread_context *context) = 0;
    virtual void MPD_stop() = 0;
    virtual void MPD_next() = 0;
    virtual void MPD_prev() = 0;
    virtual void MPD_pause() = 0;
    virtual void MPD_volumeUp() = 0;
    virtual void MPD_volumeDown() = 0;
    virtual void MPD_volumeSet(thread_context *context, int vol) = 0;
    virtual void MPD_play(thread_context *context, int id) = 0;
    virtual int MPD_getVolume(thread_context *context) = 0;

    /////////////////////// SAVE STATE iDOM ////////////////
    virtual void saveState_iDom(const bool &started) = 0;
    virtual void readState_iDom(nlohmann::json jj) = 0;

    /////////////////////// system linux ///////////////////
    virtual std::string getSystemInfo() = 0;
    /////////////////////// KODI ////////////////////////
    virtual std::string startKodi_Thread() = 0;
    /////////////////////// WiFi //////////////////////////
    virtual void wifiClientConnected() = 0;
    virtual void wifiClientDisconnected() = 0;

    /////////////////////// health check //////////////////
    virtual void healthCheck() = 0;

    ////////////////////// system /////////////////////////
    virtual void close_iDomServer [[noreturn]] () = 0;
    virtual void reloadSoft_iDomServer [[noreturn]] () = 0;
    virtual void reloadHard_iDomServer [[noreturn]] () = 0;
    virtual void raspberryReboot [[noreturn]] () = 0;
    virtual void uploadRamCpuUsage [[noreturn]] () = 0;

    //////////////////////// iDom_API /////////////////////
    // std::string dump() const override;

    //////////////////////// link generator ///////////////
    virtual std::string getLink(std::vector<std::string> v) = 0;

    //////////////////  doorbell  ////////////////////////
    virtual void doorbellDingDong() = 0;

    virtual void runCommandFromJson(const std::vector<std::string> &jj) = 0;
};