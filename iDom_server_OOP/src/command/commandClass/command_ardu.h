#pragma once

#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../command/command.h"

class command_ardu : public command
{
    std::vector<RADIO_BUTTON*> m_button433MHzVector;
    RADIO_BUTTON* m_mainRadioButton;
    std::vector<RADIO_WEATHER_STATION*> m_weatherStVe;
    RADIO_WEATHER_STATION* m_mainWeatherStation;
public:
    explicit command_ardu(const std::string& name);
    command_ardu(const std::string& name, thread_context* context );
    ~command_ardu() = default;
    std::string execute(std::vector <std::string> &v,thread_context* context) override;
    std::string help() const override;
#ifndef BT_TEST
    private:
#endif
    void pingAndOkRecv(thread_context *context, const std::string &s);
};

