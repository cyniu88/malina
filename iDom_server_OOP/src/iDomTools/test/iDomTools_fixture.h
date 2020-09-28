#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "test_data.h"
#include "../idomtools.h"
#include "../../src/functions/functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "testJSON.h"

class iDomTOOLS_ClassTest : public testing::Test
{
public:
    TEST_JSON test_Json;
    LIGHTNING test_lightning;
    CARDINAL_DIRECTIONS::ALARM_INFO test_struct;
    thread_data test_my_data;
    CONFIG_JSON test_server_set;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    iDOM_STATE main_iDomStatus;
    ALERT test_alarmTime;
    pilot_led test_pilot_led;

    /////// method
    iDomTOOLS_ClassTest()
    {
        std::cout << "konstruktor testu " <<std::endl;
    }

    void SetUp()
    {
        std::cout << "SetUP testu iDomTOOLS_ClassTest" <<std::endl;
        test_rec = std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data);
        test_server_set._server.TS_KEY = "key test";
        test_server_set._fb_viber.viberSender = "test sender";
        test_server_set._fb_viber.viberReceiver = {"R1","R2"};
        test_server_set._server.saveFilePath = "/mnt/ramdisk/iDomStateTest2.save";
        test_server_set._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_server_set._server.keyDatabasePath = "/mnt/ramdisk/key.database";

        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();

        test_rec->loadConfig(test_server_set._server.radio433MHzConfigFile);

        test_my_data.main_REC = test_rec;
        test_my_data.server_settings = &test_server_set;
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_my_data.alarmTime = test_alarmTime;
        test_my_data.idom_all_state = main_iDomStatus;
        test_my_data.ptr_pilot_led = &test_pilot_led;
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();

        test_my_data.main_iDomStatus->addObject("house");

        /////////// create
        test_my_data.ptr_buderus = std::make_unique<BUDERUS>();
        test_my_data.main_iDomTools = std::make_unique<iDomTOOLS>(&test_my_data);
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
        test_my_data.ptr_MPD_info->volume = 3;
        useful_F::myStaticData = &test_my_data;
    }

    void TearDown()
    {
    }
};
