#include <gtest/gtest.h>

#include "../key.h"

TEST(keyBT, main)
{
    std::map <std::string, std::unique_ptr <KEY> > key_map;
    EXPECT_EQ (key_map.size(), 0);
    std::unique_ptr <pilot> pilotPTR(new pilot(&key_map));
    pilotPTR->setup();
    EXPECT_EQ (key_map.size(), 32);

    EXPECT_EQ(PILOT_KEY::KEY_POWER, key_map["KEY_POWER"]->getValue());
    EXPECT_EQ(PILOT_KEY::KEY_0, key_map["KEY_0"]->getValue());
}
