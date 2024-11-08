#include "db-client.hpp"
#include "../../libs/influxm/client.h"
#include "../../src/iDom_server_OOP.h"
#include "../../src/functions/functions.h"

#include <iostream>
#include <experimental/source_location>

HttpStatus::Code dbClient::upload_iDomData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &iDomData, uint64_t timestamp, DATABASE *config)
{
    int code2 = 204;

    influx_client::flux::Client client(
        config->ip,
        config->port,
        config->token,
        config->org,
        config->bucket);

    if (iDomData.at("smog").at("smog").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("smog", std::any_cast<float>(iDomData.at("smog").at("smog").value()));
        auto code = client.write("smog", tags, fields);
        code2 = code;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji smog - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }
    {
        std::vector<influx_client::kv_t> fields;
        if (iDomData.at("temperatura").at("outdoor").has_value())
        {
            fields.emplace_back("outdoor", std::any_cast<float>(iDomData.at("temperatura").at("outdoor").value()));
        }
        if (iDomData.at("temperatura").at("inside").has_value())
        {
            fields.emplace_back("inside", std::any_cast<float>(iDomData.at("temperatura").at("inside").value()));
        }
        if (iDomData.at("temperatura").at("floor").has_value())
        {
            fields.emplace_back("floor", std::any_cast<float>(iDomData.at("temperatura").at("floor").value()));
        }
        if (iDomData.at("temperatura").at("bojler").has_value())
        {
            fields.emplace_back("bojler", std::any_cast<double>(iDomData.at("temperatura").at("bojler").value()));
        }
        if (iDomData.at("temperatura").at("domek").has_value())
        {
            fields.emplace_back("domek", std::any_cast<double>(iDomData.at("temperatura").at("domek").value()));
        }
        if (iDomData.at("temperatura").at("flow").has_value())
        {
            fields.emplace_back("flow", std::any_cast<double>(iDomData.at("temperatura").at("flow").value()));
        }
        if (iDomData.at("temperatura").at("shedTemp").has_value())
        {
            fields.emplace_back("shedTemp", std::any_cast<float>(iDomData.at("temperatura").at("shedTemp").value()));
        }

        if (fields.size() > 0)
        {
            std::vector<influx_client::kv_t> tags;
            auto code = client.write("temperatura", tags, fields);
            if (code2 == 204)
                code2 = code;
        }
        else
        {
            log_file_mutex.mutex_lock();
            log_file_cout << WARNING << "brak pozycji temepratura - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }

    if (iDomData.at("wilgoc").at("humi").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("humi", std::any_cast<float>(iDomData.at("wilgoc").at("humi").value()));
        auto code = client.write("wilgoc", tags, fields);
        if (code2 == 204)
            code2 = code;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji wilgoc humi - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }

    if (iDomData.at("cisnienie").at("dom").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("dom", std::any_cast<float>(iDomData.at("cisnienie").at("dom").value()));
        auto code = client.write("cisnienie", tags, fields);
        if (code2 == 204)
            code2 = code;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji cisnienie dom - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (iDomData.at("acdc").at("acdc").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("acdc", std::any_cast<float>(iDomData.at("acdc").at("acdc").value()));
        auto code = client.write("acdc", tags, fields);
        if (code2 == 204)
            code2 = code;
    }

    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji acdc  - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }
    if (iDomData.at("piec").at("praca").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("praca", std::any_cast<bool>(iDomData.at("piec").at("praca").value()));
        auto code = client.write("piec", tags, fields);
        if (code2 == 204)
            code2 = code;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji praca piec - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }

    return HttpStatus::Code(code2);
}

HttpStatus::Code dbClient::uploadBulbData(const std::string &name, bool state, std::optional<uint64_t> timestamp, DATABASE *config)
{
    if (config == std::nullptr_t())
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "DATABASE *config null ptr " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
        return HttpStatus::Code::FailedDependency;
    }
    influx_client::flux::Client client(
        config->ip,
        config->port,
        config->token,
        config->org,
        config->bucket);

    if (timestamp.has_value())
    {
        auto code = client.write(
            "bulb",
            {},
            {{name, state}},
            timestamp.value());
        return HttpStatus::Code(code);
    }
    auto code = client.write(
        "bulb",
        {},
        {{name, state}});
    return HttpStatus::Code(code);
}

HttpStatus::Code dbClient::upload_systemData(const std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> &iDomData, uint64_t, DATABASE *config)
{
        int code2 = 204;

    influx_client::flux::Client client(
        config->ip,
        config->port,
        config->token,
        config->org,
        config->bucket);

    if (iDomData.at("system").at("RAM").has_value() and iDomData.at("system").at("CPU").has_value())
    {
        std::vector<influx_client::kv_t> tags;
        std::vector<influx_client::kv_t> fields;
        fields.emplace_back("system", std::any_cast<float>(iDomData.at("system").at("RAM").value()));
        fields.emplace_back("system", std::any_cast<float>(iDomData.at("system").at("CPU").value()));
        auto code = client.write("system", tags, fields);
        code2 = code;
    }
    else
    {
        log_file_mutex.mutex_lock();
        log_file_cout << WARNING << "brak pozycji RAM - " << std::experimental::fundamentals_v2::source_location::current().function_name() << std::endl;
        log_file_mutex.mutex_unlock();
    }

    return HttpStatus::Code(code2);
}
