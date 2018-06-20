#include <gtest/gtest.h>
#include "../functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "../../iDomTools/test/iDomTools_fixture.h"

class functions_fixture : public iDomTOOLS_ClassTest{

};

void useful_F::sleep_1min()
{
    std::cout << "sleep_1min()"<<std::endl;
}
TEST(functions_, tokenizer)
{
     std::string test_msg = "one=two three";
     std::vector<std::string> test_v;

     EXPECT_EQ(test_v.size(), 0);
     useful_F::tokenizer(test_v,"= ",test_msg);

     EXPECT_EQ(test_v.size(),3);
     EXPECT_STREQ(test_v.at(2).c_str(),"three");
}

TEST(functions_, removeHtmlTag)
{
     std::string test_msg = "<html>test</html>";
     std::string test_pure_str = useful_F_libs::removeHtmlTag(test_msg);

     EXPECT_STREQ(test_pure_str.c_str(),"test");
}

TEST(functions_, repalceAll)
{
     std::string test_msg = "one two three";
     std::string test_pure_str = useful_F_libs::replaceAll(test_msg,"two","zero");

     EXPECT_STREQ(test_pure_str.c_str(),"one zero three");
}

TEST(functions_, split)
{
    std::string test_msg = "one two three";
    std::vector<std::string> test_v;

    EXPECT_EQ(test_v.size(), 0);
    test_v = useful_F::split(test_msg,' ');

    EXPECT_EQ(test_v.size(),3);
    EXPECT_STREQ(test_v.at(2).c_str(),"three");
}

TEST_F(functions_fixture, sleepThread)
{
    Thread_array_struc test_THRARRSTR;
    test_my_data.main_THREAD_arr = &test_THRARRSTR;

    MPD_info test_ptr_MPD;
    test_ptr_MPD.volume = 3;
    test_my_data.ptr_MPD_info = &test_ptr_MPD;
    RADIO_EQ* test_RS = new RADIO_SWITCH(&test_my_data,"listwa","209888",RADIO_EQ_TYPE::SWITCH);
    iDomSTATUS test_status;
    test_my_data.main_iDomStatus = &test_status;

    RADIO_EQ_CONTAINER_STUB stub_rec(&test_my_data);
    test_my_data.main_REC = (&stub_rec);
    test_my_data.alarmTime.time = Clock::getTime();
    test_my_data.alarmTime.state = STATE::ACTIVE;

    EXPECT_CALL(stub_rec, getEqPointer("listwa")).WillRepeatedly(testing::Return(test_RS));

    test_my_data.sleeper = 10;

    blockQueue test_q;
    test_q._clearAll();
    EXPECT_EQ(test_q._size(),0);
    useful_F::sleeper_mpd(&test_my_data);
    EXPECT_EQ(test_q._size(),1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP)<<"NIE ZATRZYMANO MUZYKI :(";
    delete test_RS;
}
