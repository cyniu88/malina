#include <gtest/gtest.h>
#include "../../iDomTools/test/iDomTools_fixture.h"

#include "../c_connection.h"

class c_connection_fixture : public iDomTOOLS_ClassTest
{
public:
    c_connection_fixture() {
        this->test_connection = std::nullptr_t();
    }

protected:
    C_connection* test_connection;
    void SetUp() final
    {
        std::cout << "c_connection_fixture SetUp()" << std::endl;
        iDomTOOLS_ClassTest::SetUp();
        test_connection = new C_connection(&test_my_data);
    }
    void TearDown() final
    {
        delete test_connection;
        iDomTOOLS_ClassTest::TearDown();
        std::cout << "c_connection_fixture TearDown()" << std::endl;
    }
    void crypto_fixture(std::string &toEncrypt, std::string key)
    {
        test_connection->crypto(toEncrypt, std::move(key), true);
    }
};

TEST_F(c_connection_fixture, crypto)
{
    std::string key = "210116556";
    std::string test_msg = "kokosowa ksiezniczka";
    std::string toEncrypt = test_msg;
    crypto_fixture(toEncrypt, key);

    for(int i = 0; i < toEncrypt.size(); ++i)
    {
        EXPECT_NE(test_msg[i],toEncrypt[i]) << " niestety równe: " << toEncrypt[i]
                                            << " na indeksie: " << i;
    }
    std::cout << "wiadomość: " << test_msg << " zakodowane: "<< toEncrypt << std::endl;
    crypto_fixture(toEncrypt, key);
    std::cout << "wiadomość: " << test_msg << " odkodowane: "<< toEncrypt << std::endl;
    EXPECT_STREQ(toEncrypt.c_str(), test_msg.c_str()) << "wiadomosci nie są równe";
}
