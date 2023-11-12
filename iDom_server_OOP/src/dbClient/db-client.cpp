#include "db-client.hpp"
#include "../../libs/influxm/client.h"
#include <iostream>

int dbClient::upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &iDomData)
{
    char points[4096];
    int pointsSize = 4096;

    influx_client::flux::Client client(
        "10.9.0.34", /* port */ 8086, /* token */
        " "
        "-aaaapov11112lj2-ovr5bbbbso6q==",
        "organization", "iDom");

    /* do something with client */
    // get temperature in gardener house

    auto code = client.writes(
        {
            {
                "temperatura",
                {},
                {{"outdoor", std::any_cast<float>(iDomData.at("temperatura").at("outdoor"))},
                 {"inside", std::any_cast<float>(iDomData.at("temperatura").at("inside"))},
                 {"floor", std::any_cast<float>(iDomData.at("temperatura").at("floor"))},
                 {"bojler", std::any_cast<double>(iDomData.at("temperatura").at("bojler"))},
                 {"domek", std::any_cast<double>(iDomData.at("temperatura").at("domek"))},
                 {"flow", std::any_cast<double>(iDomData.at("temperatura").at("flow"))},
                 {"shedTemp", std::any_cast<float>(iDomData.at("temperatura").at("shedTemp"))}},
                0,
            },
            {
                "wilgoc",
                {},
                {{"humi", std::any_cast<float>(iDomData.at("wilgoc").at("humi"))}},
                0,
            },
            {
                "smog",
                {},
                {{"smog", std::any_cast<float>(iDomData.at("smog").at("smog"))}},
                0,
            },
            {
                "cisnienie",
                {},
                {{"dom", std::any_cast<float>(iDomData.at("cisnienie").at("dom"))}},
                0,
            },
            {
                "piec",
                {},
                {{"praca", std::any_cast<bool>(iDomData.at("piec").at("praca"))}},
                0,
            },
        },
        points, pointsSize);
    return code;
}

int dbClient::uploadBulbData(const std::string &name, bool state)
{
    influx_client::flux::Client client(
        "10.9.0.34", /* port */ 8086, /* token */
        " "
        "-aaaapov11112lj2-ovr5bbbbso6q==",
        "organization", "iDom");

    /* do something with client */
    // get temperature in gardener house
    auto code = client.write(
        "bulb",
        {},
        {{name, state}});
   // int code = 204;

    std::cout << std::boolalpha << "zarowka bulb : " <<  name << " satus: " << state << std::endl;
    return code;
}

// int dbClient::uploadBulbData(const std::string &name, bool state){
//     puts("dupa jas");
// }

// int dbClient::upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &iDomData){
//     puts("dupa jas 2 ");
// }