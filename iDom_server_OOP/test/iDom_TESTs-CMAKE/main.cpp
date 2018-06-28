#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gconv.h>
#include <fstream>
#include <sstream>

class test_433
{
public:
    std::string config433 = "{ \"BUTTON\": { \"locker\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"01e7be\", \"name\": \"locker\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"BUTTON\" } }, \"SWITCH\": { \"A\": { \"OFF\": \"10;TriState;0280aa;0;OFF\", \"ON\": \"10;TriState;0280aa;0;ON\", \"id\": \"1444\", \"name\": \"A\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"SWITCH\" }, \"ALARM\": { \"OFF\": \"dummyOFF\", \"ON\": \"dummyON\", \"id\": \"1223\", \"name\": \"ALARM\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"SWITCH\" }, \"B\": { \"OFF\": \"10;TriState;02822a;0;OFF\", \"ON\": \"10;TriState;02822a;0;ON\", \"id\": \"1445\", \"name\": \"B\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"SWITCH\" }, \"C\": { \"OFF\": \"10;TriState;02828a;0;OFF\", \"ON\": \"10;TriState;02828a;0;ON\", \"id\": \"1446\", \"name\": \"C\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"SWITCH\" }, \"listwa\": { \"OFF\": \"10;Kaku;0ad04d;15;OFF\", \"ON\": \"10;Kaku;0ad04d;15;ON\", \"id\": \"1450\", \"name\": \"listwa\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"SWITCH\" } }, \"WEATHER\": { \"first\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"0704\", \"name\": \"first\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"WEATHER\" } } }";
    void makeFile(std::string path)
    {
         std::ofstream o(path);
         o << config433 ;
    }
};

int main(int argc, char **argv) {
    test_433 t;
    t.makeFile("/mnt/ramdisk/433_eq_conf.json");

    ::testing::InitGoogleTest( &argc, argv );
    //::testing::GTEST_FLAG(filter) = "functions_fixture.sleepThread";
    //::testing::GTEST_FLAG(filter) = "Switch_Class.switch_alarm_on";
    ::testing::GTEST_FLAG(filter) = "command433MHz_Class_fixture.wrongMSGformat";
    return RUN_ALL_TESTS();
}

