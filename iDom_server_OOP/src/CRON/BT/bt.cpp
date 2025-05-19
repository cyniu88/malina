#include <gtest/gtest.h>
#include "../cron.hpp"
#include "../../iDom_server_OOP.h"
#include <thread>
#include <iostream>

class cron_Class_fixture : public ::testing::Test
{
public:
    cron_Class_fixture()
    {
        test_context.server_settings = &testCS;
        test_context.server_settings->_cron = "../config/cron.yaml";
        test_q = std::make_unique<CRON>(&test_context);
        test_context.idom_all_state.houseState = STATE::LOCK;
    }

protected:
    thread_context test_context;
    CONFIG_JSON testCS;
    std::unique_ptr<CRON> test_q;
};

TEST_F(cron_Class_fixture, main)
{
    test_q->runCommandCron("30min");
}
