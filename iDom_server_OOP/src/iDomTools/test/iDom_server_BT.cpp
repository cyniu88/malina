#include <gtest/gtest.h>
#include <curl/curl.h>
#include "iDomTools_fixture.h"

class iDom_server_Class : public iDomTOOLS_ClassTest
{
protected:
    virtual void SetUp() final
    {
        std::cout << "konfiguracja przed testem iDom_server_Class " <<std::endl;
    }

    virtual void TearDown() final
    {
        std::cout << "czyszczenie po tescie iDom_server_Class " <<std::endl;
    }
};
