#include <gtest/gtest.h>
#include "../../iDomTools/test/iDomTools_fixture.h"

#include "../c_connection.h"

class c_connection_fixture : public iDomTOOLS_ClassTest
{
public:
    c_connection_fixture() {}

protected:
    C_connection* test_connection;
    void SetUp()
    {
        std::cout << "c_connection_fixture SetUp()" << std::endl;
        iDomTOOLS_ClassTest::SetUp();
        test_connection = new C_connection(&test_my_data);
    }
    void TearDown()
    {
        delete test_connection;
        iDomTOOLS_ClassTest::TearDown();
        std::cout << "c_connection_fixture TearDown()" << std::endl;
    }
};

TEST_F(c_connection_fixture, crypto)
{
    std::string key = "1234";
    std::string test_msg = "5kokosowa ksiezniczka";
    std::string toEncrypt = test_msg;
    test_connection->crypto(toEncrypt, key, true);

    std::cout << "wiadomość: " << test_msg << " zakodowane: "<< toEncrypt << std::endl;
    test_connection->crypto(toEncrypt, key, true);
    std::cout << "wiadomość: " << test_msg << " odkodowane: "<< toEncrypt << std::endl;

    EXPECT_STREQ(toEncrypt.c_str(), test_msg.c_str());
}
