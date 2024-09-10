#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "../db-client.hpp"
#include "../../../src/iDom_server_OOP.h"

class db_client_fixture : public testing::Test
{
public:
    db_client_fixture() {};
    
    DATABASE config;
protected:
};

TEST_F(db_client_fixture, main_TC)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> iDomData;
    iDomData["temperatura"]["outdoor"] = std::nullopt;
    iDomData["temperatura"]["inside"] = std::nullopt;
    iDomData["temperatura"]["floor"] = std::nullopt;
    iDomData["temperatura"]["bojler"] = std::nullopt;
    iDomData["temperatura"]["domek"] = std::nullopt;
    iDomData["temperatura"]["flow"] = std::nullopt;
    iDomData["temperatura"]["shedTemp"] = std::nullopt;

    iDomData["wilgoc"]["humi"] = std::nullopt;
    iDomData["smog"]["smog"] = std::nullopt;
    iDomData["cisnienie"]["dom"] = std::nullopt;
    iDomData["piec"]["praca"] = std::nullopt;
    iDomData["acdc"]["acdc"] = std::nullopt;

    config.ip = "127.0.0.1";
    config.port = 1223;
    config.token = "asdasdasda";
    config.org = "testogr";
    config.bucket = "test";
    dbClient clinet;
    EXPECT_NO_THROW(clinet.upload_iDomData(iDomData, 111, &config));
}