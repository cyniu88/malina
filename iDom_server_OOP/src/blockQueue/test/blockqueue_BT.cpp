#include <gtest/gtest.h>
#include "../blockqueue.h"
#include <thread>
#include <iostream>

class blockQueue_Class_fixture : public ::testing::Test
{
public:
    blockQueue_Class_fixture()  {  }

protected:
    blockQueue test_q;

    void SetUp() final
    {
        test_q._clearAll();
        std::cout << "blockQueue_Class_fixture SetUp" << std::endl;
    }

    void TearDown() final
    {
        std::cout << "blockQueue_Class_fixture TearDown" << std::endl;
    }
    static void getFromQ_thread()
    {
        blockQueue q;
        while(q._size() == 0 )
        {
        }
        EXPECT_EQ(q._get(), MPD_COMMAND::STOP);
        while(q._size() == 0 )
        {
        }
        EXPECT_EQ(q._get(), MPD_COMMAND::STOP);
    }

    static void putToQ_thread(MPD_COMMAND c)
    {
        blockQueue q;
        q._add(c);
        sleep(1);
        q._add(c);
    }
};

TEST_F(blockQueue_Class_fixture, main)
{
    test_q._clearAll();
    EXPECT_EQ(test_q._size(), 0);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NULL_);
    test_q._add(MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 0);
}

TEST_F(blockQueue_Class_fixture, inThread)
{
    std::thread get(blockQueue_Class_fixture::getFromQ_thread);
    std::thread put(blockQueue_Class_fixture::putToQ_thread,MPD_COMMAND::STOP);
    get.join();
    put.join();
}

TEST_F(blockQueue_Class_fixture, allClear)
{
    EXPECT_EQ(test_q._size(), 0);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NULL_);
    test_q._add(MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 1);
    test_q._add(MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 2);
    test_q._clearAll();
    EXPECT_EQ(test_q._size(), 0);
}

TEST_F(blockQueue_Class_fixture, capacityExceeded)
{
    EXPECT_EQ(test_q._size(), 0);

    for (auto i = 0 ; i <10 ; ++i)
    {
      test_q._add(MPD_COMMAND::PLAY);
    }

    EXPECT_THROW(test_q._add(MPD_COMMAND::PLAY), std::string );
}
