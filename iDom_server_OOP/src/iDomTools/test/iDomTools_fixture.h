#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/functions/functions.h"
#include "../../RADIO_433_eq/radio_433_eq.h"
#include "testJSON.h"
#include "../../HOUSE/TEST/house_room_handler_stub.h"
#include "../../SATEL_INTEGRA/BT/satel_integra_stub.h"
#include "../../buderus/mock/buderus_mock.h"
#include "../../iDomTools/idomtools.h"

class iDomTOOLS_ClassTest : public testing::Test
{
public:
    TEST_JSON test_Json;
    thread_data test_my_data;
    CONFIG_JSON test_server_set;
    std::shared_ptr<RADIO_EQ_CONTAINER> test_rec;
    ALERT test_alarmTime;
    SATEL_INTEGRA_HANDLER_STUB test_satel;
    std::shared_ptr<BUDERUS_MOCK> testBuderusMock;

    /////// method
    iDomTOOLS_ClassTest():test_rec(std::make_shared<RADIO_EQ_CONTAINER>(&test_my_data)),
                            testBuderusMock(std::make_shared<BUDERUS_MOCK>())
    {
        std::cout << "konstruktor testu " <<std::endl;
        test_server_set._server.TS_KEY = "key test";
        test_server_set._fb_viber.viberSender = "test sender";
        test_server_set._fb_viber.viberReceiver = {"R1","R2"};
        test_server_set._server.saveFilePath = "/mnt/ramdisk/iDomStateTest2.save";
        test_server_set._server.radio433MHzConfigFile = "/mnt/ramdisk/433_eq_conf.json";
        test_server_set._server.keyDatabasePath = "/mnt/ramdisk/key.database";
        test_server_set._command = nlohmann::json::parse(R"({
        "lock":["jedna komenda", "druga komenda"],
        "unlock":["jedna komenda unlock", "druga komenda unlock"],
        "sunrise":{
            "lock": ["433MHz switch B OFF", "sdruga komenda sunrise lock"],
            "unlock": ["sjedna komenda sunrise unlock", "433MHz switch B ON"]
        },
        "sunset":{
            "unlock": ["sjedna komenda sunset unlock", "433MHz switch B ON"],
            "lock": ["433MHz switch B OFF", "sdruga komenda sunset lock"]
        }
    })");


        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();

        test_rec->loadConfig(test_server_set._server.radio433MHzConfigFile);

        test_server_set._satel_integra.host = "127.0.0.1";
        test_server_set._satel_integra.port = 7094;
        test_server_set._satel_integra.pin = "1234";

        test_server_set._server.lightningApiURL = "http://burze.dzis.net/ramka.php?miejscowosc=krakow&promien=20";

        test_my_data.idom_all_state.houseState = STATE::UNDEFINE;
        test_my_data.idom_all_state.alarmSatelState = STATE::UNDEFINE;

        test_my_data.main_REC = test_rec;
        test_my_data.server_settings = &test_server_set;
        test_my_data.main_iDomStatus = std::make_unique<iDomSTATUS>();
        test_my_data.alarmTime = test_alarmTime;
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();

        test_my_data.lusina.temperatureDS20 =  "21.8";
        test_my_data.main_iDomStatus->addObject("house");

        /////////// create
        test_my_data.ptr_buderus = testBuderusMock;
        test_my_data.main_iDomTools = std::make_shared<iDomTOOLS>(&test_my_data);
        test_my_data.ptr_MPD_info = std::make_unique<MPD_info>();
        test_my_data.ptr_MPD_info->volume = 3;
        test_my_data.main_house_room_handler = std::make_shared<house_room_handler_stub>(&test_my_data);
        test_my_data.server_settings->_satel_integra.partitionID = 1;
        test_my_data.server_settings->_satel_integra.pin = "1122";
        test_my_data.satelIntegraHandler = &test_satel;
        useful_F::myStaticData = &test_my_data;
    }
};
