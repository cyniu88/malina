#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "../db-client.hpp"

class db_client_fixture : public testing::Test
{
public:
    db_client_fixture(){};

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


    dbClient clinet;
    EXPECT_NO_THROW(clinet.upload_iDomData(iDomData, 9));

}
