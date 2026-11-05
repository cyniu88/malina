#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <any>
#include <optional>

#include "../db-client.hpp"
#include "../../../src/iDom_server_OOP.h"

// Pomocnik do budowania mapy danych
static DATABASE makeConfig()
{
    DATABASE cfg;
    cfg.ip     = "127.0.0.1";
    cfg.port   = 9999;   // port niedostępny – client zwróci błąd, nie rzuci
    cfg.token  = "tok";
    cfg.org    = "org";
    cfg.bucket = "buck";
    return cfg;
}

class db_client_fixture2 : public testing::Test
{
protected:
    DATABASE config = makeConfig();
    dbClient client;
};

// ──────────────────────────────────────────────────────────────────────────────
// upload_iDomData – wszystkie pola wypełnione wartościami (nie nullopt)
// pokrywa gałęzie has_value() == true
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_iDomData_all_fields_present)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["temperatura"]["outdoor"]  = std::make_optional<std::any>((float)10.0f);
    data["temperatura"]["inside"]   = std::make_optional<std::any>((float)21.0f);
    data["temperatura"]["floor"]    = std::make_optional<std::any>((float)22.0f);
    data["temperatura"]["bojler"]   = std::make_optional<std::any>((double)60.0);
    data["temperatura"]["domek"]    = std::make_optional<std::any>((double)5.0);
    data["temperatura"]["flow"]     = std::make_optional<std::any>((double)40.0);
    data["temperatura"]["shedTemp"] = std::make_optional<std::any>((float)18.0f);
    data["wilgoc"]["humi"]          = std::make_optional<std::any>((float)55.0f);
    data["smog"]["smog"]            = std::make_optional<std::any>((float)25.0f);
    data["cisnienie"]["dom"]        = std::make_optional<std::any>((float)1013.0f);
    data["acdc"]["acdc"]            = std::make_optional<std::any>((float)3.9f);
    data["piec"]["praca"]           = std::make_optional<std::any>((bool)true);

    EXPECT_NO_THROW(client.upload_iDomData(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_iDomData – wszystkie pola nullopt (gałęzie log WARNING)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_iDomData_all_nullopt)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["temperatura"]["outdoor"]  = std::nullopt;
    data["temperatura"]["inside"]   = std::nullopt;
    data["temperatura"]["floor"]    = std::nullopt;
    data["temperatura"]["bojler"]   = std::nullopt;
    data["temperatura"]["domek"]    = std::nullopt;
    data["temperatura"]["flow"]     = std::nullopt;
    data["temperatura"]["shedTemp"] = std::nullopt;
    data["wilgoc"]["humi"]          = std::nullopt;
    data["smog"]["smog"]            = std::nullopt;
    data["cisnienie"]["dom"]        = std::nullopt;
    data["acdc"]["acdc"]            = std::nullopt;
    data["piec"]["praca"]           = std::nullopt;

    EXPECT_NO_THROW(client.upload_iDomData(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// uploadBulbData – config nullptr → zwraca FailedDependency
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, uploadBulbData_null_config)
{
    auto code = client.uploadBulbData("bulb1", true, std::nullopt, nullptr);
    EXPECT_EQ(code, HttpStatus::Code::FailedDependency);
}

// ──────────────────────────────────────────────────────────────────────────────
// uploadBulbData – z timestampem
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, uploadBulbData_with_timestamp)
{
    EXPECT_NO_THROW(client.uploadBulbData("bulb1", true, 123456789ULL, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// uploadBulbData – bez timestampa
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, uploadBulbData_without_timestamp)
{
    EXPECT_NO_THROW(client.uploadBulbData("bulb2", false, std::nullopt, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_systemData – dane RAM i CPU obecne
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_systemData_with_values)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["system"]["RAM"] = std::make_optional<std::any>((long)512L);
    data["system"]["CPU"] = std::make_optional<std::any>((double)12.5);

    EXPECT_NO_THROW(client.upload_systemData(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_systemData – brak danych (nullopt) → gałąź WARNING
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_systemData_nullopt)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["system"]["RAM"] = std::nullopt;
    data["system"]["CPU"] = std::nullopt;

    EXPECT_NO_THROW(client.upload_systemData(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_universal – różne typy (int, double, float, string, bool)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_universal_all_types)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["measure"]["int_val"]    = std::make_optional<std::any>((int)42);
    data["measure"]["double_val"] = std::make_optional<std::any>((double)3.14);
    data["measure"]["float_val"]  = std::make_optional<std::any>((float)1.5f);
    data["measure"]["string_val"] = std::make_optional<std::any>(std::string("hello"));
    data["measure"]["bool_val"]   = std::make_optional<std::any>((bool)true);

    EXPECT_NO_THROW(client.upload_universal(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_universal – nullopt (gałąź "No value")
// po naprawieniu bugu (guard !fields.empty()) nie crashuje
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_universal_nullopt)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["measure"]["empty"] = std::nullopt;

    EXPECT_NO_THROW(client.upload_universal(data, 0, &config));
}

// ──────────────────────────────────────────────────────────────────────────────
// upload_universal – alternatywny bucket
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(db_client_fixture2, upload_universal_custom_bucket)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>> data;
    data["sensor"]["temp"] = std::make_optional<std::any>((double)20.0);

    EXPECT_NO_THROW(client.upload_universal(data, 0, &config, "custom_bucket"));
}
