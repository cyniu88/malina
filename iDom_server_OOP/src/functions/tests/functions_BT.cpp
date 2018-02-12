#include <gtest/gtest.h>
#include "../functions.h"

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
     std::string test_pure_str = useful_F::removeHtmlTag(test_msg);

     EXPECT_STREQ(test_pure_str.c_str(),"test");
}

TEST(functions_, repalceAll)
{
     std::string test_msg = "one two three";
     std::string test_pure_str = useful_F::replaceAll(test_msg,"two","zero");

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
