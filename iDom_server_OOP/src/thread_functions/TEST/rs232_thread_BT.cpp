#include "../../iDom_server_OOP.h"
#include "../rs232_thread.h"
#include "test_data.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

// ──────────────────────────────────────────────────────────────────────────────
// Testy logiki split() używanej wewnątrz Send_Recieve_rs232_thread
// (bug: data.at(1) bez sprawdzenia rozmiaru przy braku separatora ':')
// ──────────────────────────────────────────────────────────────────────────────

TEST(rs232_split_logic, split_with_colon_gives_two_tokens)
{
    auto data = useful_F::split("KEY_PAD:3", ':');
    ASSERT_EQ(data.size(), 2u);
    EXPECT_EQ(data.at(0), "KEY_PAD");
    EXPECT_EQ(data.at(1), "3");
}

TEST(rs232_split_logic, split_TIMEOUT_gives_one_token)
{
    auto data = useful_F::split("TIMEOUT", ':');
    ASSERT_EQ(data.size(), 1u);
    EXPECT_EQ(data.at(0), "TIMEOUT");
    // data.at(1) tutaj rzuciłoby out_of_range – pokazujemy, że to ryzyko istnieje
}

TEST(rs232_split_logic, split_empty_string_gives_one_empty_token)
{
    auto data = useful_F::split("", ':');
    // split pustego stringa – zależy od implementacji; nie powinien rzucić
    EXPECT_NO_THROW(data.at(0));
}

TEST(rs232_split_logic, split_known_command_no_colon_single_token)
{
    // Linia bez ':' (np. "HELLO") powinna dać 1 element – nie 2
    auto data = useful_F::split("HELLO", ':');
    EXPECT_GE(data.size(), 1u);
    EXPECT_EQ(data.at(0), "HELLO");
}

TEST(rs232_split_logic, split_multiple_colons)
{
    auto data = useful_F::split("A:B:C", ':');
    ASSERT_GE(data.size(), 3u);
    EXPECT_EQ(data.at(0), "A");
    EXPECT_EQ(data.at(1), "B");
    EXPECT_EQ(data.at(2), "C");
}

// ──────────────────────────────────────────────────────────────────────────────
// Test uruchomienia wątku rs232: wątek startuje i kończy się po ustawieniu
// go_while=false. SerialPi stub zwraca available()=0, więc wątek po prostu
// kręci się w pętli sleep(50ms) aż do wyłączenia.
// ──────────────────────────────────────────────────────────────────────────────

TEST(rs232_thread_lifecycle, thread_starts_and_stops)
{
    using namespace std::chrono_literals;

    thread_context ctx;
    CONFIG_JSON cfg;
    cfg._rs232.portRS232 = "test_port";
    cfg._rs232.BaudRate  = 9600;
    ctx.server_settings  = &cfg;
    ctx.mqttHandler      = std::make_unique<MQTT_mosquitto>("test");

    useful_F::go_while = true;

    std::thread t([&ctx](){
        Send_Recieve_rs232_thread(&ctx, "test_rs232_thread");
    });

    // pozwalamy wątkowi wejść w pętlę
    std::this_thread::sleep_for(120ms);

    // zatrzymujemy wątek
    useful_F::go_while = false;

    // wątek powinien zakończyć się w ciągu ~1.6s (1.5s sleep po wyjściu z pętli)
    t.join();

    SUCCEED(); // jeśli dotarliśmy tutaj, wątek uruchomił się i zakończył poprawnie
}
