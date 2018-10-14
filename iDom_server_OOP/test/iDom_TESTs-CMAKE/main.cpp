#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gconv.h>
#include <fstream>
#include <sstream>
#include <test_data.h>

std::string TEST_DATA::return_send_to_arduino = "-2:-2";
std::string TEST_DATA::return_httpPost = "ok.\n";
std::string TEST_DATA::return_httpPost_expect = "";
std::string TEST_DATA::return_viber_msg = "";
std::string TEST_DATA::serial_b = "NULL";
std::string TEST_DATA::serial_sended = "NULL";
std::string TEST_DATA::LCD_print = "NULL";
int TEST_DATA::test_pin = 0;
class test_433
{
public:
    std::string config433 = "{ \"BUTTON\": { \"locker\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"01e7be\", \"name\": \"locker\", \"type\": \"BUTTON\" } }, \"SWITCH\": { \"A\": { \"OFF\": \"10;TriState;0280aa;0;OFF\", \"ON\": \"10;TriState;0280aa;0;ON\", \"id\": \"1444\", \"name\": \"A\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"ALARM\": { \"OFF\": \"dummyOFF\", \"ON\": \"dummyON\", \"id\": \"1223\", \"name\": \"ALARM\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"B\": { \"OFF\": \"10;TriState;02822a;0;OFF\", \"ON\": \"10;TriState;02822a;0;ON\", \"id\": \"1445\", \"name\": \"B\", \"on15sec\": \"null\", \"sunrise\": \"ON\", \"sunset\": \"OFF\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"C\": { \"OFF\": \"10;TriState;02828a;0;OFF\", \"ON\": \"10;TriState;02828a;0;ON\", \"id\": \"1446\", \"name\": \"C\", \"on15sec\": \"null\", \"sunrise\": \"OFF\", \"sunset\": \"ON\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"listwa\": { \"OFF\": \"10;Kaku;0ad04d;15;OFF\", \"ON\": \"10;Kaku;0ad04d;15;ON\", \"id\": \"1450\", \"name\": \"listwa\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"OFF\", \"unlock\": \"ON\", \"type\": \"SWITCH\" } }, \"WEATHER\": { \"first\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"0704\", \"name\": \"first\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"WEATHER\" } } }";
    std::string config433_fake = "{ \"BUTTON\": { \"locker\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"01e7be\", \"name\": \"locker\",  \"type\": \"BUTTON\" } }, \"SWITCH\": { \"A\": { \"OFF\": \"10;TriState;0280aa;0;OFF\", \"ON\": \"10;TriState;0280aa;0;ON\", \"id\": \"1444\", \"name\": \"A\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"ALARM\": { \"OFF\": \"dummyOFF\", \"ON\": \"dummyON\", \"id\": \"1223\", \"name\": \"ALARM\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"B\": { \"OFF\": \"10;TriState;02822a;0;OFF\", \"ON\": \"10;TriState;02822a;0;ON\", \"id\": \"1445\", \"name\": \"B\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"C\": { \"OFF\": \"10;TriState;02828a;0;OFF\", \"ON\": \"10;TriState;02828a;0;ON\", \"id\": \"1446\", \"name\": \"C\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"listwa\": { \"OFF\": \"10;Kaku;0ad04d;15;OFF\", \"ON\": \"10;Kaku;0ad04d;15;ON\", \"id\": \"1450\", \"name\": \"listwa\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"OFF\", \"unlock\": \"null\", \"type\": \"SWITCH\" } } } }";

    void makeFile(const std::string& path)
    {
         std::ofstream o(path);
         o << config433 ;
    }
    void makeFileFake(const std::string& path)
    {
         std::ofstream o(path);
         o << config433_fake ;
    }
};

int main(int argc, char **argv) {
    test_433 t;
    t.makeFile("/mnt/ramdisk/433_eq_conf.json");
    t.makeFileFake("/mnt/ramdisk/433_eq_conf_fake.json");

    ::testing::InitGoogleTest( &argc, argv );
    //::testing::GTEST_FLAG(filter) = "functions_fixture.sleepThread";
    //::testing::GTEST_FLAG(filter) = "event_counter_fixture.getLast1minNumberEvent";
    //::testing::GTEST_FLAG(filter) = "command433MHz_Class_fixture.*)";
    return RUN_ALL_TESTS();
}

