#include <gtest/gtest.h>
#include "../blockqueue.h"

class blockQueue_Class_fixture : public ::testing::Test
{
public:
    blockQueue_Class_fixture()  {  }

protected:
    blockQueue test_q;

    void SetUp()
    {
        std::cout << "blockQueue_Class_fixture SetUp" << std::endl;
    }

    void TearDown()
    {
        std::cout << "blockQueue_Class_fixture TearDown" << std::endl;
    }

};

TEST_F(blockQueue_Class_fixture, main)
{
    EXPECT_EQ(test_q._size(), 0);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::NULL_);
    test_q._add(MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 1);
    EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
    EXPECT_EQ(test_q._size(), 0);
}
