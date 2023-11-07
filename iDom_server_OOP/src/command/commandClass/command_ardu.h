#ifndef COMMND_ARDU_H
#define COMMND_ARDU_H

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
    command_ardu(const std::string& name, thread_data* context );
    ~command_ardu() = default;
    std::string execute(std::vector <std::string> &v,thread_data* context) override;
    std::string help() const override;
#ifndef BT_TEST
    private:
#endif
    void pingAndOkRecv(thread_data *context, const std::string &s);
};

#endif // COMMND_ARDU_H
