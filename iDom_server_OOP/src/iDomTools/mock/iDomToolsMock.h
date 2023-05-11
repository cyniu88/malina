#ifndef IDOMTOOLSMOCK_H
#define IDOMTOOLSMOCK_H
#include <gmock/gmock.h>

#include "../idomtools_interface.h"

class iDomToolsMock : public iDomTOOLS_INTERFACE
{
public:
    ~iDomToolsMock() = default;
    MOCK_METHOD1(getTHERMOMETER_CONTAINERlastState, TEMPERATURE_STATE(const std::string &name));
    MOCK_METHOD3(hasTemperatureChange, TEMPERATURE_STATE(const std::string &thermometerName, double reference, double histereza));
    MOCK_METHOD2(sendSMSifTempChanged, void(const std::string &thermomethernName, int reference));
    MOCK_METHOD1(getThermoStats, std::string(const std::string &name));
    MOCK_METHOD0(updateTemperatureStats, void());

    MOCK_METHOD0(turnOnSpeakers, void());
    MOCK_METHOD0(turnOffSpeakers, void());
    MOCK_METHOD0(turnOnOff230vOutdoor, void());
    MOCK_METHOD1(turnOnOff433MHzSwitch, void(const std::string &name));
    MOCK_METHOD1(turnOn433MHzSwitch, void(std::string name));
    MOCK_METHOD1(turnOff433MHzSwitch, void(std::string name));
    MOCK_METHOD0(runOnSunset, void());
    MOCK_METHOD0(runOnSunrise, void());
    MOCK_METHOD0(lockHome, void());
    MOCK_METHOD0(unlockHome, void());
    //////////////////////////// 433MHz action ///////////////////
    MOCK_METHOD0(switchActionOnLockHome, void());
    MOCK_METHOD0(switchActionOnUnlockHome, void());

    MOCK_METHOD0(buttonLockHome, void());
    MOCK_METHOD0(buttonUnlockHome, void());
    //////////////////////////// useful /////////////////////////
    MOCK_METHOD0(isItDay, bool());
    MOCK_METHOD0(getAllDataSunriseSunset, std::string());
    //////////////////////////// lightning ////////////////////////
    MOCK_METHOD0(checkLightning, void());

    //////////////////////////// weather /////////////////////////
    MOCK_METHOD1(getSunrise, std::string(bool extend));
    MOCK_METHOD1(getSunset, std::string(bool extende));
    MOCK_METHOD0(getSunsetClock, Clock());
    MOCK_METHOD0(getSunriseClock, Clock());
    MOCK_METHOD1(getDayLenght, std::string(bool extend));
    MOCK_METHOD2(getWeatherEvent, std::string(const std::string &city, unsigned int radius));

    MOCK_METHOD0(getAlert, WEATHER_DATABASE());

    MOCK_METHOD1(textToSpeach, void(std::vector<std::string> *textVector));
    MOCK_METHOD0(getTextToSpeach, std::string());

    MOCK_METHOD0(getTemperature, std::vector<std::string>());
    MOCK_METHOD0(getTemperatureString, std::string());
    MOCK_METHOD0(getSmog, std::string());
    MOCK_METHOD0(send_data_to_thingSpeak, void());
    MOCK_METHOD0(getFloorTemp, std::string());

    //////////////////// camera part ///////////////////////
    MOCK_METHOD1(cameraLedON, void(const std::string &link));
    MOCK_METHOD1(cameraLedOFF, void(const std::string &link));
    //////////////////// viber msg /////////////////////////
    MOCK_METHOD5(sendViberMsg, nlohmann::json(const std::string &msg,
                                              const std::string &receiver,
                                              const std::string &senderName,
                                              const std::string &accessToken,
                                              const std::string &url));
    MOCK_METHOD6(sendViberPicture, nlohmann::json(const std::string &msg,
                                                  const std::string &image,
                                                  const std::string &receiver,
                                                  const std::string &senderName,
                                                  const std::string &accessToken,
                                                  const std::string &url));
    MOCK_METHOD6(sendViberUrl, nlohmann::json(const std::string &msg,
                                              const std::string &url2,
                                              const std::string &receiver,
                                              const std::string &senderName,
                                              const std::string &accessToken,
                                              const std::string &url));
    MOCK_METHOD5(sendViberMsgBool, STATE(const std::string &msg,
                                         const std::string &receiver,
                                         const std::string &senderName,
                                         const std::string &accessToken,
                                         const std::string &url));
    MOCK_METHOD6(sendViberPictureBool, STATE(const std::string &msg,
                                             const std::string &image,
                                             const std::string &receiver,
                                             const std::string &senderName,
                                             const std::string &accessToken,
                                             const std::string &url));
    //////////////////// facebook //////////////////////////
    MOCK_METHOD2(postOnFacebook, std::string(const std::string &msg, const std::string &image));

    //////////////////// LED part //////////////////////////
    MOCK_METHOD0(ledOFF, std::string());
    MOCK_METHOD0(ledClear, std::string());
    MOCK_METHOD2(ledClear, std::string(unsigned int from, unsigned int to));
    MOCK_METHOD0(ledOn, std::string());
    //////////////////// ALERT //////////////////////////
    MOCK_METHOD0(checkAlarm, void());
    //////////////////// MPD part //////////////////////////
    MOCK_METHOD1(MPD_play, void(thread_data *my_data));
    MOCK_METHOD0(MPD_stop, void());
    MOCK_METHOD0(MPD_next, void());
    MOCK_METHOD0(MPD_prev, void());
    MOCK_METHOD0(MPD_pause, void());
    MOCK_METHOD0(MPD_volumeUp, void());
    MOCK_METHOD0(MPD_volumeDown, void());
    MOCK_METHOD2(MPD_volumeSet, void(thread_data *my_data, int vol));
    MOCK_METHOD2(MPD_play, void(thread_data *my_data, int id));
    MOCK_METHOD1(MPD_getVolume, int(thread_data *my_data));

    /////////////////////// SAVE STATE iDOM ////////////////
    MOCK_METHOD1(saveState_iDom, void(const bool &started));
    MOCK_METHOD1(readState_iDom, void(nlohmann::json jj));

    /////////////////////// system linux ///////////////////
    MOCK_METHOD0(getSystemInfo, std::string());
    /////////////////////// KODI ////////////////////////
    MOCK_METHOD0(startKodi_Thread, std::string());
    /////////////////////// WiFi //////////////////////////
    MOCK_METHOD0(wifiClientConnected, void());
    MOCK_METHOD0(wifiClientDisconnected, void());

    /////////////////////// health check //////////////////
    MOCK_METHOD0(healthCheck, void());

    ////////////////////// system /////////////////////////
    MOCK_METHOD0(close_iDomServer, void());
    MOCK_METHOD0(reloadSoft_iDomServer, void());
    MOCK_METHOD0(reloadHard_iDomServer, void());
    MOCK_METHOD0(raspberryReboot, void());

    //////////////////////// iDom_API /////////////////////
    // std::string dump() const override;

    //////////////////////// link generator ///////////////
    MOCK_METHOD1(openGateLink, std::string(std::vector<std::string> v));

    //////////////////  doorbell  ////////////////////////
    MOCK_METHOD0(doorbellDingDong, void());
};

#endif // IDOMTOOLSMOCK_H
