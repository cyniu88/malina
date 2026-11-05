#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../commandrs232.h"
#include "test_data.h"

class command_RS232_fixture2 : public testing::Test
{
public:
    command_RS232_fixture2()
        : test_command_rs232(std::make_unique<commandRS232>("RS232"))
    {
        test_context.main_Rs232 = std::make_unique<SerialPi>("addr");
    }

protected:
    std::unique_ptr<commandRS232> test_command_rs232;
    std::vector<std::string> test_v;
    thread_context test_context;
};

// ──────────────────────────────────────────────────────────────────────────────
// error – brak dodatkowego parametru
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_RS232_fixture2, error_missing_message)
{
    test_v = {"RS232", "error"};
    auto ret = test_command_rs232->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "add more parameter to error");
}

// ──────────────────────────────────────────────────────────────────────────────
// error – z wiadomością (kilka tokenów)
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_RS232_fixture2, error_with_message)
{
    test_v = {"RS232", "error", "jakis", "blad"};
    // funkcja buduje msg ale nic nie zwraca (zwraca "default") – sprawdzamy brak wyjątku
    auto ret = test_command_rs232->execute(test_v, &test_context);
    // gałąź "error" nie przypisuje do str_buf, więc zwracana jest wartość inicjalna
    EXPECT_STREQ(ret.c_str(), "default");
}

// ──────────────────────────────────────────────────────────────────────────────
// send – pojedynczy token danych
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_RS232_fixture2, send_single_token)
{
    TEST_DATA::return_send_to_arduino = "done";
    test_v = {"RS232", "send", "PING;"};
    auto ret = test_command_rs232->execute(test_v, &test_context);
    EXPECT_STREQ(ret.c_str(), "done");
}

// ──────────────────────────────────────────────────────────────────────────────
// help
// ──────────────────────────────────────────────────────────────────────────────
TEST_F(command_RS232_fixture2, help_contains_send)
{
    std::string h = test_command_rs232->help();
    EXPECT_THAT(h, testing::HasSubstr("send"));
    EXPECT_THAT(h, testing::HasSubstr("RS232"));
}
