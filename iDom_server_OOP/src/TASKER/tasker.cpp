#include "tasker.h"
#include "../functions/functions.h"

TASKER::TASKER(thread_data *context) : topic("iDom-client/command"),
                                       context(context)
{
    context->lusina.statHumi.resize(270);
}

int TASKER::runTasker()
{
    if (counter == 0)
    {
        ++counter;
    }
    else
    {
        counter = 0;
    }

    if (context->mqttHandler->getReceiveQueueSize() > 0)
    {
        auto kk = context->mqttHandler->getMessage();

        if (kk.first == "iDom-client/buderus/ems-esp/heating_active")
        {
            std::vector<std::string> v = {"buderus", "heating_active"};
            v.push_back(kk.second);
            (void)commandMQTT.run(v, context);
        }
        else if (kk.first == "iDom-client/buderus/ems-esp/boiler_data" or kk.first == "iDom-client/buderus/ems-esp/boiler_data_ww")
        {
            std::vector<std::string> v = {"buderus", "boiler_data"};
            v.push_back(kk.second);
            (void)commandMQTT.run(v, context);
        }
        else if (kk.first == "iDom-client/buderus/ems-esp/thermostat_data")
        {
            std::vector<std::string> v = {"buderus", "thermostat_data"};
            v.push_back(kk.second);
            (void)commandMQTT.run(v, context);
        }
        else if (kk.first == "iDom-client/shed/put")
        {
            std::vector<std::string> v = {"shed", "put"};
            v.push_back(kk.second);
            auto ret = commandMQTT.run(v, context);
            context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/command",
                                          ret);
        }
        else if (kk.first == topic)
        {
            auto v = useful_F::split(kk.second, ' ');

            if (v.at(1) == "button")
            {
                context->main_house_room_handler->executeButtonComand(std::stoul(v.at(3)), v.at(2), &commandMQTT);
            }
            else
            {
                auto ret = commandMQTT.run(v, context);
                context->mqttHandler->publish(context->server_settings->_mqtt_broker.topicPublish + "/command",
                                              ret);
            }
        }
        // TODO temporary added
        else if (kk.first == topic + "/lusina/h")
        {
            context->myEventHandler.run("lusina")->addEvent(kk.second);
            try
            {
                auto v = useful_F::split(kk.second, ' ');

                context->lusina.humidityDTH = v[1];
                context->lusina.statHumi.push_back(std::stoi(v[1]));
                context->lusina.temperatureDTH = v[3];
            }
            catch (...)
            {
                context->lusina.statHumi.push_back(-1);
            }
        }
        else if (kk.first == topic + "/lusina/t")
        {
            context->myEventHandler.run("lusina")->addEvent(kk.second);
            auto v = useful_F::split(kk.second, ' ');
            if (v.size() == 1 && std::stof(v.front()) not_eq -127)
            {
                context->lusina.temperatureDS20 = v.front();
                context->lusina.statTemp.push_back(std::stof(v.front()));
            }
        }

        return 10;
    }
    return 256;
}
