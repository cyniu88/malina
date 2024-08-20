#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "../../iDom_server_OOP.h"
#include "../../functions/functions.h"
#include "../../iDomTools/mock/iDomToolsMock.h"
#include "../../buderus/mock/buderus_mock.h"
#include "../menu_base.h"
#include "../menu_root.h"
#include "../menu_light.h"

class menu_state_machine_fixture: public testing::Test
{
protected:
    menu_state_machine_fixture(): main_iDomTools(std::make_shared<iDomToolsMock>()),
                                    buderusMock(std::make_shared<BUDERUS_MOCK>())
    {
        test_context.main_iDomTools = main_iDomTools;
        test_context.ptr_buderus = buderusMock;
    }
    thread_context test_context;
    std::shared_ptr<iDomToolsMock>main_iDomTools;
    std::shared_ptr<BUDERUS_MOCK> buderusMock;
};

TEST_F(menu_state_machine_fixture, containerMenuMap)
{
    cyniu::ContainerMenuMap<std::string, std::string> testMenuMap;
    testMenuMap.insert(std::pair<std::string, std::string>("a jeden","1"));
    testMenuMap.insert(std::pair<std::string, std::string>("b dwa","2"));
    testMenuMap.insert(std::pair<std::string, std::string>("c trzy","3"));
    testMenuMap.begin();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "1");}
    testMenuMap.up();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "2");}
    testMenuMap.up();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "3");}

    testMenuMap.down();
    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "2");}

    testMenuMap.down();
    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "1");}

    testMenuMap.down();
    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "3");}
}

TEST_F(menu_state_machine_fixture, containerMenuMapSignle)
{
    cyniu::ContainerMenuMap<std::string, std::string> testMenuMap;
    testMenuMap.insert(std::pair<std::string, std::string>("a jeden","1"));
    testMenuMap.begin();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "1");}
    testMenuMap.up();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "1");}
    testMenuMap.up();

    {auto para = testMenuMap.getCurrent();
        EXPECT_THAT(para->second, "1");}
}

TEST_F(menu_state_machine_fixture, main)
{
    test_context.main_iDomStatus = std::make_unique<iDomSTATUS>();
    test_context.main_iDomStatus->addObject("music", STATE::STOP);
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&test_context, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    auto main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);

    auto res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::MENU);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_MAIN"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);

    main_key_menu_handler->recKeyEvent(KEY_PAD::RES);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));
}

TEST_F(menu_state_machine_fixture, MENU_LIGHT){
    test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("test");
    useful_F::myStaticCtx = &test_context;
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    //////// bulb config
    std::string cfg("../config/bulb_config.json");
    test_context.main_house_room_handler = std::make_shared<house_room_handler>(&test_context);
    test_context.main_house_room_handler->loadConfig(cfg);

    auto ptr = std::make_unique<MENU_LIGHT>(&test_context, &lcd, &stateMechine);
    ptr->entry();
    ptr->keyPadDown();
    ptr->keyPadLeft();
    ptr->keyPadLeft();
    ptr->keyPadLeft();
    ptr->keyPadRight();
    ptr->keyPadOk();
}

TEST_F(menu_state_machine_fixture, MENU_BUDERUS)
{
    test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("test");
    test_context.main_iDomStatus = std::make_unique<iDomSTATUS>();
    test_context.main_iDomStatus->addObject("music", STATE::STOP);
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&test_context, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    auto main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);

    auto res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::MENU);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_MAIN"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    main_key_menu_handler->recKeyEvent(KEY_PAD::UP);
    EXPECT_CALL(*main_iDomTools.get(), getSmog());
    EXPECT_CALL(*main_iDomTools.get(), getDayLenght(false));
    EXPECT_CALL(*main_iDomTools.get(), getSunrise(false));
    EXPECT_CALL(*main_iDomTools.get(), getSunset(false));
    EXPECT_CALL(*buderusMock.get(), getCurFlowTemp());
    main_key_menu_handler->recKeyEvent(KEY_PAD::OK);
}

TEST_F(menu_state_machine_fixture, MENU_KODI)
{
    test_context.main_iDomStatus = std::make_unique<iDomSTATUS>();
    test_context.main_iDomStatus->addObject("music", STATE::STOP);
    LCD_c lcd(0x27,16,2);
    MENU_STATE_MACHINE stateMechine;
    auto ptr = std::make_unique<MENU_ROOT>(&test_context, &lcd, &stateMechine);
    stateMechine.setStateMachine(std::move(ptr));
    auto main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);

    auto res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_ROOT"));

    main_key_menu_handler->recKeyEvent(KEY_PAD::MENU);
    res = stateMechine.currentState->getStateName();
    EXPECT_THAT(res, ::testing::HasSubstr("MENU_MAIN"));
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
 //   main_key_menu_handler->recKeyEvent(KEY_PAD::OK);

    // kodi menu

    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);
    main_key_menu_handler->recKeyEvent(KEY_PAD::DOWN);

    EXPECT_CALL(*main_iDomTools.get(), MPD_play(testing::_)).Times(2);
    main_key_menu_handler->recKeyEvent(KEY_PAD::OK);

    main_key_menu_handler->recKeyEvent(KEY_PAD::POWER);
}
