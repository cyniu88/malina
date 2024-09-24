#include <gtest/gtest.h>

#include "../command_idom.h"
#include "../../../iDomTools/test/iDomTools_fixture.h"
#include "../../../thread_functions/iDom_thread.h"
#include "../../../MENU/menu_root.h"
#include "../../../blockQueue/blockqueue.h"
#include "test_data.h"

class commandiDom_Class_fixture : public iDomTOOLS_ClassTest
{
public:
  commandiDom_Class_fixture(): test_v({"iDom"}),
    test_command_iDom(std::make_unique<command_iDom>("iDom"))
  {
    test_q._clearAll();
    test_context.main_RFLink = std::make_shared<RFLinkHandler>(&test_context);
  }

protected:
  std::vector<std::string> test_v;
  blockQueue test_q;
  std::unique_ptr<command_iDom> test_command_iDom;
};

TEST_F(commandiDom_Class_fixture, getName)
{
  EXPECT_THAT(test_command_iDom->getCommandName(),testing::HasSubstr("iDom"));
}

TEST_F(commandiDom_Class_fixture, help)
{
  std::string helpStr = test_command_iDom->help();
  EXPECT_THAT(helpStr,testing::HasSubstr("iDom"));
  std::cout << "test " << helpStr.size() << std::endl;
  EXPECT_EQ(helpStr.size(), 1297);
}

TEST_F(commandiDom_Class_fixture, less_param)
{
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  EXPECT_THAT(retStr,testing::HasSubstr("need parameter!"));
}

TEST_F(commandiDom_Class_fixture, unknonw_para)
{
  test_v.push_back("fake");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  EXPECT_THAT(retStr,testing::HasSubstr("iDom - unknown parameter:"));
}

TEST_F(commandiDom_Class_fixture, speakers)
{
  test_context.idom_all_state.houseState = STATE::LOCK;
  test_context.main_iDomTools->unlockHome();
  //////////////// fake command
  test_v.push_back("speakers");
  test_v.push_back("fake");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  EXPECT_THAT(retStr,testing::HasSubstr("unknow speakers action: fake"));
  ///////////////// ON
  test_context.main_iDomStatus->setObjectState("speakers",STATE::UNDEFINE);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("speakers");
  test_v.push_back("ON");
  retStr = test_command_iDom->execute(test_v, &test_context);
  EXPECT_THAT(retStr,testing::HasSubstr("speakers ON"));
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::ON);
  ///////////////// OFF
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("speakers");
  test_v.push_back("OFF");
  retStr = test_command_iDom->execute(test_v, &test_context);
  EXPECT_THAT(retStr,testing::HasSubstr("speakers OFF"));
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
  // */
}

TEST_F(commandiDom_Class_fixture, sunset_sunrise)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("sunset");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("Sunset time:"));

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("sunrise");
  retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("Sunrise time:"));
}

TEST_F(commandiDom_Class_fixture, day_lenght)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("day");
  test_v.push_back("lenght");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("Day Lenght :"));
}

TEST_F(commandiDom_Class_fixture, sun)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("sun");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("Day Lenght :"));
}

TEST_F(commandiDom_Class_fixture, sysinfo)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("sysinfo");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("free RAM"));
}

TEST_F(commandiDom_Class_fixture, temperature)
{
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-12.0));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(62.2));    EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(12));
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("temperature");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("inside\":-12.0"));

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("temperature");
  test_v.push_back("stats");
  test_v.push_back("insideee");
  retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("not found!"));
}

TEST_F(commandiDom_Class_fixture, text)
{
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("text");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("Godzina"));
}

TEST_F(commandiDom_Class_fixture, lock_unlock)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("lock");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_EQ(test_context.idom_all_state.houseState, STATE::LOCK);

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("unlock");
  retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_EQ(test_context.idom_all_state.houseState, STATE::UNLOCK);
}

TEST_F(commandiDom_Class_fixture, smog)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("smog");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr(" mg/m^3"));
}

TEST_F(commandiDom_Class_fixture, say)
{
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));
  Clock::setTime_forBT_usage(23,23);
  TEST_DATA::return_send_to_arduino = "-3:6";
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("say");
  test_v.push_back("dummy");
  test_v.push_back("dummy");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  //EXPECT_THAT(retStr,testing::HasSubstr("sad"));
}

TEST_F(commandiDom_Class_fixture, wifi)
{
  TEST_DATA::return_httpPost = "ok";
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("wifi");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("ok"));
}

TEST_F(commandiDom_Class_fixture, kodi)
{
  MENU_STATE_MACHINE stateMechine;
  LCD_c lcdStub(1,1,1);
  auto ptr = std::make_unique<MENU_ROOT>(&test_context, &lcdStub, &stateMechine);
  stateMechine.setStateMachine(std::move(ptr));
  test_context.main_key_menu_handler = std::make_unique<KEY_HANDLER>(&stateMechine);
  test_context.main_iDomStatus->setObjectState("KODI",STATE::ACTIVE);
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("KODI");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);

  std::cout << "retString: " << retStr << std::endl;
  EXPECT_STREQ(retStr.c_str(),"kodi already run");

  test_context.main_iDomStatus->setObjectState("KODI",STATE::DEACTIVE);
  test_context.main_iDomStatus->setObjectState("music",STATE::PLAY);
  test_context.main_iDomStatus->setObjectState("speakers",STATE::ON);

  std::array<Thread_array_struc,iDomConst::MAX_CONNECTION> test_ThreadArrayStruc;

  for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
    test_ThreadArrayStruc.at(i).thread_socket = 0;
  test_ThreadArrayStruc.at(3).thread_socket = 0;
  test_ThreadArrayStruc.at(3).thread_ID = std::this_thread::get_id();
  test_context.main_THREAD_arr = &test_ThreadArrayStruc;

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("KODI");
  retStr = test_command_iDom->execute(test_v, &test_context);

  //sleep(1);
  iDOM_THREAD::waitUntilAllThreadEnd(&test_context);

  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("STARTED"));

  for (std::size_t i = 0 ; i < iDomConst::MAX_CONNECTION; i++)
    test_ThreadArrayStruc[i].thread_socket = 1;

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("KODI");
  retStr = test_command_iDom->execute(test_v, &test_context);

  std::cout << "retString: " << retStr << std::endl;
  EXPECT_STREQ(retStr.c_str(),"not free space to new thread");
}

TEST_F(commandiDom_Class_fixture, health_alarm)
{
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("health");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr("no alarms!"));
}
#include<memory>

TEST_F(commandiDom_Class_fixture, addAccessKEY)
{
  std::string s = "kokos";
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("key");
  test_v.push_back("add");
  test_v.push_back("kokos");
  test_v.push_back("20");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr(s));
  test_context.m_keyHandler->removeKEY(s);
}

TEST_F(commandiDom_Class_fixture, addAccessKEY_temp)
{
  std::string s = "kokos";
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("key");
  test_v.push_back("add");
  test_v.push_back("tmp");
  test_v.push_back(s);
  test_v.push_back("20");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr(s));
  test_context.m_keyHandler->removeKEY(s);
}

TEST_F(commandiDom_Class_fixture, removeAccessKEY)
{
  std::string s = "ttt";
  test_context.m_keyHandler->addKEY(s,"null", 23);

  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("key");
  test_v.push_back("remove");
  test_v.push_back(s);
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr,testing::HasSubstr(s));
  test_context.m_keyHandler->removeKEY(s);
}

TEST_F(commandiDom_Class_fixture, getOpenLink)
{
  test_context.server_settings->_gateway.url = "http://test.pl";
  test_context.server_settings->_gateway.keySize = 128;
  test_v.clear();
  test_v.push_back("iDom");
  test_v.push_back("link");
  test_v.push_back("gate");
  std::string retStr = test_command_iDom->execute(test_v, &test_context);
  std::cout << "retString: " << retStr << std::endl;
  EXPECT_THAT(retStr, testing::HasSubstr("http://"));
  EXPECT_EQ(retStr.at(19) , '=');
  EXPECT_EQ(retStr.at(45 + test_context.server_settings->_gateway.keySize) , '&');
}
