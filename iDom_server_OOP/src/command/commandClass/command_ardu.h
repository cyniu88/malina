#ifndef COMMND_ARDU_H
#define COMMND_ARDU_H
#include <iostream>
#include <string>
#include <vector>
#include "../command.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

class command_ardu : public command
{
    std::vector<RADIO_BUTTON*> m_button433MHzVector;
    RADIO_BUTTON* m_mainRadioButton;
    std::vector<RADIO_WEATHER_STATION*> m_weatherStVe;
    RADIO_WEATHER_STATION* m_mainWeatherStation;
public:
    explicit command_ardu(const std::string& name);
    command_ardu(const std::string& name, thread_data* my_data );
    std::string execute(std::vector <std::string> &v,thread_data* my_data) override;
    std::string help() const override;
#ifndef BT_TEST
    private:
#endif
    void pingAndOkRecv(thread_data *my_data, const std::string &s);
};

#endif // COMMND_ARDU_H
