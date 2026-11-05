#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../recuperator.h"

class recuperator_fixture : public testing::Test
{
public:
    recuperator_fixture()
    {
        test_context.server_settings = &test_cfg;
        test_context.mqttHandler     = std::make_unique<MQTT_mosquitto>("test");
        test_cfg._recuperation.MQTT_CONTROL_TOPIC = "iDom/recuperator";
        rec = std::make_unique<Recuperator>(&test_context);
    }

protected:
    thread_context              test_context;
    CONFIG_JSON                 test_cfg;
    std::unique_ptr<Recuperator> rec;
};

// ──────────────────────────────────────────────────────────────────────────────
// setDataFromMqtt – dodaje dane i można je odczytać przez getData()
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setDataFromMqtt_stores_value)
{
    rec->setDataFromMqtt({"iDom/recuperator/temperature", "21.5"});
    auto data = rec->getData();
    EXPECT_TRUE(data.count("recuperator") > 0);
}

TEST_F(recuperator_fixture, setDataFromMqtt_multiple_keys)
{
    rec->setDataFromMqtt({"iDom/rec/humidity",    "55.0"});
    rec->setDataFromMqtt({"iDom/rec/temperature", "22.0"});
    rec->setDataFromMqtt({"iDom/rec/temperature", "24.0"});
    auto data = rec->getData();
    // getData czyści po odczycie – sprawdzamy że nie rzuca
    EXPECT_NO_THROW(rec->getData());
}

// ──────────────────────────────────────────────────────────────────────────────
// getData – po wywołaniu mapa jest czyszczona (keepLatestOnly)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, getData_clears_after_read)
{
    rec->setDataFromMqtt({"iDom/rec/co2", "800"});
    rec->getData();   // pierwsze odczytanie – czyści
    auto data2 = rec->getData();
    // po wyczyszczeniu average zwróci ostatnią wartość (keepLatestOnly zostawia 1)
    EXPECT_TRUE(data2.count("recuperator") > 0);
}

// ──────────────────────────────────────────────────────────────────────────────
// setSpeed – prawidłowe wartości
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setSpeed_valid_values)
{
    EXPECT_NO_THROW(rec->setSpeed("away"));
    EXPECT_NO_THROW(rec->setSpeed("low"));
    EXPECT_NO_THROW(rec->setSpeed("medium"));
    EXPECT_NO_THROW(rec->setSpeed("high"));
}

// ──────────────────────────────────────────────────────────────────────────────
// setSpeed – nieprawidłowa wartość (gałąź logError)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setSpeed_invalid_value)
{
    EXPECT_NO_THROW(rec->setSpeed("turbo"));
}

// ──────────────────────────────────────────────────────────────────────────────
// setAwayModeTime – time > 0 publikuje JSON
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setAwayModeTime_positive_time)
{
    EXPECT_NO_THROW(rec->setAwayModeTime(true, 30, "min"));
}

// ──────────────────────────────────────────────────────────────────────────────
// setAwayModeTime – time <= 0 nie publikuje (tylko loguje)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setAwayModeTime_zero_time)
{
    EXPECT_NO_THROW(rec->setAwayModeTime(false, 0, "min"));
}

// ──────────────────────────────────────────────────────────────────────────────
// setAwayMode – publikuje "true"
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setAwayMode)
{
    EXPECT_NO_THROW(rec->setAwayMode());
}

// ──────────────────────────────────────────────────────────────────────────────
// setBoostMode – time > 0
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setBoostMode_positive_time)
{
    EXPECT_NO_THROW(rec->setBoostMode(10, "min", true));
}

// ──────────────────────────────────────────────────────────────────────────────
// setBoostMode – time <= 0 (gałąź logError)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, setBoostMode_zero_time)
{
    EXPECT_NO_THROW(rec->setBoostMode(0, "min", true));
}

// ──────────────────────────────────────────────────────────────────────────────
// stop
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(recuperator_fixture, stop_does_not_throw)
{
    EXPECT_NO_THROW(rec->stop());
}
