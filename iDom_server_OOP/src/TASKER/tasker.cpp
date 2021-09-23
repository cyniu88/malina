#include "tasker.h"
#include "../functions/functions.h"

TASKER::TASKER(thread_data *my_data):  topic("iDom-client/command"),
    my_data(my_data)
{
    my_data->lusina.statHumi.resize(270);
}

int TASKER::runTasker()
{
    if(counter == 0){
        ++counter;
    }
    else{
        counter = 0;
    }

    if(my_data->mqttHandler->getReceiveQueueSize() > 0)
    {
        auto kk = my_data->mqttHandler->getMessage();

        if(kk.first == "iDom-client/buderus/ems-esp/heating_active")
        {
            std::vector<std::string> v = {"buderus","heating_active"};
            v.push_back(kk.second);
            (void) commandMQTT.run(v, my_data);
        }
        else if(kk.first == "iDom-client/buderus/ems-esp/boiler_data")
        {
            std::vector<std::string> v = {"buderus","boiler_data"};
            v.push_back(kk.second);
            (void) commandMQTT.run(v, my_data);
        }
        else if(kk.first == "iDom-client/buderus/ems-esp/thermostat_data")
        {
            std::vector<std::string> v = {"buderus","thermostat_data"};
            v.push_back(kk.second);
            (void) commandMQTT.run(v, my_data);
        }
        else if(kk.first == topic)
        {
            auto v = useful_F::split(kk.second, ' ');
            if(v.at(1) == "button"){
                // my_data->main_house_room_handler->
                useful_F::myStaticData->main_iDomTools->sendViberMsg("przycisk" + v.at(2) + " " + v.at(3),
                                                                     useful_F::myStaticData->server_settings->_fb_viber.viberReceiver.at(0),
                                                                     "button");
            }
            else{
                auto ret = commandMQTT.run(v, my_data);
                my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/command",
                                              ret);
            }
        }
        //TODO temporary added
        else if (kk.first == topic + "/lusina/h")
        {
            my_data->myEventHandler.run("lusina")->addEvent(kk.second);
            try { auto v = useful_F::split(kk.second, ' ');

                my_data->lusina.humidityDTH = v[1];
                my_data->lusina.statHumi.push_back(std::stoi(v[1]));
                my_data->lusina.temperatureDTH = v[3];
            }
            catch (...)
            {
                my_data->lusina.statHumi.push_back(-1);
            }
        }
        else if(kk.first == topic + "/lusina/t")
        {
            my_data->myEventHandler.run("lusina")->addEvent(kk.second);
            auto v = useful_F::split(kk.second, ' ');
            if (v.size() == 1 && std::stof(v[0]) not_eq -127)
            {
                my_data->lusina.temperatureDS20 = v[0];
                my_data->lusina.statTemp.push_back(std::stof(v[0]));
            }
        }
        return 10;
    }
    return 256;
}
