#include "db-client.hpp"
#include "../../libs/influxm/client.h"
#include <iostream>

HttpStatus::Code dbClient::upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &iDomData)
{
    char points[4096];
    int pointsSize = 4096;

    influx_client::flux::Client client(
        "10.9.0.34", /* port */ 8086, /* token */
        " "
        "-aaaapov11112lj2-ovr5bbbbso6q==",
        "organization", "iDom");

    if(iDomData.at("smog").at("smog").has_value()){
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("smog", std::any_cast<float>(iDomData.at("smog").at("smog").value()));
        // fields.emplace_back("field123", 1123);
        auto code = client.write("smog", tags, fields);
    }

    auto code = client.writes(
        {
            {
                "temperatura",
                {},
                {{"outdoor", std::any_cast<float>(iDomData.at("temperatura").at("outdoor").value())},
                 {"inside", std::any_cast<float>(iDomData.at("temperatura").at("inside").value())},
                 {"floor", std::any_cast<float>(iDomData.at("temperatura").at("floor").value())},
                 {"bojler", std::any_cast<double>(iDomData.at("temperatura").at("bojler").value())},
                 {"domek", std::any_cast<double>(iDomData.at("temperatura").at("domek").value())},
                 {"flow", std::any_cast<double>(iDomData.at("temperatura").at("flow").value())},
                 {"shedTemp", std::any_cast<float>(iDomData.at("temperatura").at("shedTemp").value())}},
                0,
            },
            {
                "wilgoc",
                {},
                {{"humi", std::any_cast<float>(iDomData.at("wilgoc").at("humi").value())}},
                0,
            },
            {
                "cisnienie",
                {},
                {{"dom", std::any_cast<float>(iDomData.at("cisnienie").at("dom").value())}},
                0,
            },
            {
                "piec",
                {},
                {{"praca", std::any_cast<bool>(iDomData.at("piec").at("praca").value())}},
                0,
            },
            {
                "acdc",
                {},
                {{"acdc", std::any_cast<float>(iDomData.at("acdc").at("acdc").value())}},
                0,
            },
        },
        points, pointsSize);
    return HttpStatus::Code(code);
}

HttpStatus::Code dbClient::uploadBulbData(const std::string &name, bool state)
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

    // std::cout << std::boolalpha << "zarowka bulb : " <<  name << " satus: " << state << std::endl;
    return HttpStatus::Code(code);
}

// int dbClient::uploadBulbData(const std::string &name, bool state){
//     puts("dupa jas");
// }

// int dbClient::upload_iDomData(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &iDomData){
//     puts("dupa jas 2 ");
// }