#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <curl/curl.h>

#include "iDomTools_fixture.h"
#include "../mqtt_mosquitto/MQTT_mosquitto/mqtt.h"
#include "test_data.h"
#include "../src/blockQueue/blockqueue.h"
#include "../src/iDomSaveState/idom_save_state.h"
// #include

TEST_F(iDomTOOLS_ClassTest, smog)
{
  TEST_DATA::return_httpPost = R"delim({
   "@context":{
      "meta":"https://schema.org/meta",
      "totalPages":"https://schema.org/totalPages",
      "links":"https://schema.org/links"
   },
   "@context":{
      "Lista danych pomiarowych":{
         "@id":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData"
      },
      "links":"https://schema.org/relatedLink",
      "totalPages":"https://schema.org/numberOfPages"
   },
   "Lista danych pomiarowych":[
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 10:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 09:00:00",
         "Wartość":1.1
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 08:00:00",
         "Wartość":6.4
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 07:00:00",
         "Wartość":10.8
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 06:00:00",
         "Wartość":14.7
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 05:00:00",
         "Wartość":14.8
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 04:00:00",
         "Wartość":12.1
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 03:00:00",
         "Wartość":17.0
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 02:00:00",
         "Wartość":18.2
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 01:00:00",
         "Wartość":19.9
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 00:00:00",
         "Wartość":13.6
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 23:00:00",
         "Wartość":13.2
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 22:00:00",
         "Wartość":21.9
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 21:00:00",
         "Wartość":16.7
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 20:00:00",
         "Wartość":16.5
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 19:00:00",
         "Wartość":15.2
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 18:00:00",
         "Wartość":12.5
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 17:00:00",
         "Wartość":12.5
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 16:00:00",
         "Wartość":16.0
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 15:00:00",
         "Wartość":18.8
      }
   ],
   "meta":{
      "dc:description":"Usługa sieciowa udostępniająca wyniki jednogodzinnych pomiarów dwutlenku siarki (SO2), dwutlenku azotu (NO2), pyłu PM10, pyłu PM2,5, tlenku węgla (CO), benzenu (C6H6), ozonu (O3), od ostatniego dostępnego wyniku do trzeciej doby wstecz, na podstawie identyfikatora stanowiska pomiarowego. Dane udostępniane są w czasie lokalnym w jednostce μg/m3",
      "sy:updateFrequency":"1",
      "dc:language":"pol (ISO 639-2 / ISO 639-3)",
      "schema:contentType":"application/ld+json",
      "schema:provider":{
         "@context":"https://schema.org",
         "@type":"Organization",
         "address":{
            "@type":"PostalAddress",
            "addressLocality":"Warszawa",
            "postalCode":"02-362",
            "streetAddress":"Bitwy Warszawskiej 1920 3"
         },
         "email":"api@gios.gov.pl",
         "name":"Główny Inspektorat Ochrony Środowiska"
      },
      "schema:dateModified":"03.11.2022",
      "dc:title":"Bieżące dane pomiarowe z wybranego stanowiska",
      "schema:datePublished":"10.04.2017",
      "schema:keywords":"dane pomiarowe, wyniki pomiarów, powietrze, jakość powietrza, zanieczyszczenie powietrza, stacje pomiarowe, stanowiska pomiarowe, monitoring powietrza, Państwowy Monitoring Środowiska, środowisko, stan środowiska",
      "sy:updatePeriod":"hourly",
      "schema:isPartOf":"Państwowy Monitoring Środowiska, Jakość powietrza"
   },
   "links":{
      "next":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=1&size=20",
      "last":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=2&size=20",
      "prev":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20",
      "self":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20",
      "first":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20"
   },
   "totalPages":3
})delim";
  std::string smog = test_context.main_iDomTools->getSmog().value();
  puts(smog.c_str());
  puts(" smog");
  ASSERT_GE(smog.size(), 1);

  unsigned int smog_int = std::stoi(smog);
  ASSERT_EQ(smog_int, 1);
}

TEST_F(iDomTOOLS_ClassTest, smog_null)
{
  TEST_DATA::return_httpPost = R"delim({
   "@context":{
      "meta":"https://schema.org/meta",
      "totalPages":"https://schema.org/totalPages",
      "links":"https://schema.org/links"
   },
   "@context":{
      "Lista danych pomiarowych":{
         "@id":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData"
      },
      "links":"https://schema.org/relatedLink",
      "totalPages":"https://schema.org/numberOfPages"
   },
   "Lista danych pomiarowych":[
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 10:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 09:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 08:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 07:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 06:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 05:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 04:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 03:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 02:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 01:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-07-01 00:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 23:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 22:00:00",
         "Wartość":null
      },
      {
         "Kod stanowiska":"MpKrakSwoszo-PM10-1g",
         "Data":"2025-06-30 21:00:00",
         "Wartość":null
      }
   ],
   "meta":{
      "dc:description":"Usługa sieciowa udostępniająca wyniki jednogodzinnych pomiarów dwutlenku siarki (SO2), dwutlenku azotu (NO2), pyłu PM10, pyłu PM2,5, tlenku węgla (CO), benzenu (C6H6), ozonu (O3), od ostatniego dostępnego wyniku do trzeciej doby wstecz, na podstawie identyfikatora stanowiska pomiarowego. Dane udostępniane są w czasie lokalnym w jednostce μg/m3",
      "sy:updateFrequency":"1",
      "dc:language":"pol (ISO 639-2 / ISO 639-3)",
      "schema:contentType":"application/ld+json",
      "schema:provider":{
         "@context":"https://schema.org",
         "@type":"Organization",
         "address":{
            "@type":"PostalAddress",
            "addressLocality":"Warszawa",
            "postalCode":"02-362",
            "streetAddress":"Bitwy Warszawskiej 1920 3"
         },
         "email":"api@gios.gov.pl",
         "name":"Główny Inspektorat Ochrony Środowiska"
      },
      "schema:dateModified":"03.11.2022",
      "dc:title":"Bieżące dane pomiarowe z wybranego stanowiska",
      "schema:datePublished":"10.04.2017",
      "schema:keywords":"dane pomiarowe, wyniki pomiarów, powietrze, jakość powietrza, zanieczyszczenie powietrza, stacje pomiarowe, stanowiska pomiarowe, monitoring powietrza, Państwowy Monitoring Środowiska, środowisko, stan środowiska",
      "sy:updatePeriod":"hourly",
      "schema:isPartOf":"Państwowy Monitoring Środowiska, Jakość powietrza"
   },
   "links":{
      "next":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=1&size=20",
      "last":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=2&size=20",
      "prev":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20",
      "self":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20",
      "first":"https://api.gios.gov.pl/pjp-api/v1/rest/data/getData/20320?page=0&size=20"
   },
   "totalPages":3
})delim";
  EXPECT_FALSE(test_context.main_iDomTools->getSmog().has_value());
}

TEST_F(iDomTOOLS_ClassTest, smog_no_data)
{
  TEST_DATA::return_httpPost = "";
  EXPECT_FALSE(test_context.main_iDomTools->getSmog().has_value());
}

TEST_F(iDomTOOLS_ClassTest, hasTemperatureChange)
{
  std::cout << "##################################### 0" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(-7.7));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-1.0));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();

  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::Under);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::NoChanges);
  std::cout << "##################################### 1" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(-7.7));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(27.7));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();

  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::NoChanges);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::Over);

  std::cout << "##################################### 2" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(20.3));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::Over);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::Under);

  std::cout << "##################################### 3" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::NoChanges);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::NoChanges);

  std::cout << "##################################### 4" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::NoChanges);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::NoChanges);

  std::cout << "##################################### 5" << std::endl;

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(30.3));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(60));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  test_context.main_iDomTools->send_data_to_thingSpeak();
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("outside"), TEMPERATURE_STATE::NoChanges);
  EXPECT_EQ(test_context.main_iDomTools->getTHERMOMETER_CONTAINERlastState("inside"), TEMPERATURE_STATE::Over);
  std::cout << "##################################### 6" << std::endl;

  ////// getThermoStats

  std::cout << "WYNIK: " << test_context.main_iDomTools->getThermoStats("inside") << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, weatherAlert)
{
  /* std::string test_data_from_www = R"( <div style="margin:0;padding:0;width:350px;font:0.8em Lucida,Arial,sans-seri f;background:#FFC">)"
                                     R"(<p style="margin:1px;padding:1px;text-align:center;background:#FF9;borde \\ r:1px dotted"><b><a href="http://burze.dzis.net?page=wyszukiwarka&amp;miejscowos\\ c=krakow" target="_blank" style="color:#00E">krakow</a></b>)"
                                     R"(<i>(50°03'N 19°57'E)</i> </p>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:\\ center">Zarejestrowano 54 wyładowania atmosferyczne w promieniu 300km . Najbliższe 79.76km na zachód.)"
                                     R"(</dl> <dl style="margin:1px 1px 0 1px;padding:0;cl ear:both;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align: \\ center">Mróz, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Upał, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Wiatr, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Opady, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Burze, brak ostrzeżeń</dl>)"
                                     R"(<dl style="margin:1px 1px 0 1px;padding:0;clear:both ;background:#FFD;border:1px dotted;overflow:auto;color:green;text-align:center">  Trąby powietrzne, brak ostrzeżeń</dl></div>)";
*/
  WEATHER_DATABASE test_WA;
  test_WA = test_context.main_iDomTools->getAlert();
  std::cout << std::endl
            << test_WA.lightning.alert << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, send_data_to_thingSpeak)
{
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(true));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(62.2));
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(12));
  TEST_DATA::return_send_to_arduino = "-2.3:-2";
  TEST_DATA::return_httpPost_expect = "NULL";
  EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(), "NULL");
  test_context.main_iDomTools->send_data_to_thingSpeak();
  std::cout << "DATA: " << TEST_DATA::return_httpPost_expect << std::endl;
  EXPECT_STREQ(TEST_DATA::return_httpPost_expect.c_str(), "httpPost");
}

TEST_F(iDomTOOLS_ClassTest, checkAlarm)
{
  blockQueue test_q;
  unsigned int fromVol = 48;
  unsigned int toVol = 57;

  ///////////////////////////////////// to save
  test_context.main_iDomStatus->setObjectState("house", STATE::UNLOCK);
  test_context.main_iDomStatus->setObjectState("music", STATE::PLAY);
  test_context.main_iDomStatus->setObjectState("speakers", STATE::ON);
  test_context.idom_all_state.houseState = STATE::LOCK;

  test_context.main_iDomStatus->setObjectState("listwa", STATE::ON);

  test_alarmTime.time = Clock::getTime();
  test_alarmTime.state = STATE::ACTIVE;
  test_alarmTime.toVolume = 58;
  test_alarmTime.fromVolume = 48;

  test_alarmTime.commands.push_back("komenda 1");
  test_alarmTime.commands.push_back("komenda 2");

  test_context.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);
  test_context.alarmTime = test_alarmTime;
  useful_F::myStaticCtx = &test_context;

  //////////////////////////////////////////////////////////////

  test_context.alarmTime.time = Clock::getTime();
  test_context.alarmTime.state = STATE::ACTIVE;

  EXPECT_EQ(test_context.alarmTime.state, STATE::ACTIVE);

  for (unsigned int i = fromVol; i < toVol; ++i)
  {
    test_context.main_iDomTools->checkAlarm();
    test_q._get();
    EXPECT_EQ(test_context.alarmTime.state, STATE::WORKING) << "zły stan w for " << i << " " << toVol;
    EXPECT_EQ(test_context.ptr_MPD_info->volume, i + 1) << "zły poziom glosnosci w for";
  }

  test_context.main_iDomTools->checkAlarm();

  EXPECT_EQ(test_context.alarmTime.state, STATE::DEACTIVE) << "nie jest STATE::DEACTIVE";
  EXPECT_EQ(test_context.ptr_MPD_info->volume, toVol) << "nie inkrementowane?";
}

TEST_F(iDomTOOLS_ClassTest, homeLockPlayStopMusic)
{
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(false));
  ///////////////////////////////////// to save
  test_context.main_iDomStatus->setObjectState("house", STATE::UNDEFINE);
  test_context.main_iDomStatus->setObjectState("music", STATE::PLAY);
  test_context.main_iDomStatus->setObjectState("speakers", STATE::ON);
  test_context.idom_all_state.houseState = STATE::UNDEFINE;

  test_context.main_iDomStatus->setObjectState("listwa", STATE::ON);

  test_alarmTime.time = Clock::getTime();
  test_alarmTime.state = STATE::ACTIVE;
  test_context.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);

  blockQueue test_q;
  test_q._clearAll();
  EXPECT_EQ(test_q._size(), 0);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("house"), STATE::UNDEFINE);
  test_context.main_iDomTools->lockHome();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("house"), STATE::LOCK);
  test_context.main_iDomTools->MPD_play(&test_context);
  EXPECT_EQ(test_q._size(), 0);
  test_context.main_iDomTools->unlockHome();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("house"), STATE::UNLOCK);
  test_context.main_iDomTools->MPD_play(&test_context);
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
  EXPECT_EQ(test_q._size(), 0);
  test_context.main_iDomTools->lockHome();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("house"), STATE::LOCK);
  test_context.main_iDomTools->MPD_stop();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
  EXPECT_EQ(test_q._size(), 0);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("house"), STATE::LOCK);
  std::string returnedString = test_context.main_iDomStatus->getAllObjectsStateString();
  EXPECT_THAT(returnedString, testing::HasSubstr("LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, testCPU_Load)
{
  std::cout << "TEST LOAD" << std::endl;
  std::cout << test_context.main_iDomTools->getSystemInfo() << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, saveState_readState)
{
  test_context.server_settings->_server.saveFilePath = "/mnt/ramdisk/iDomStateTest3.save";
  test_context.main_iDomStatus->setObjectState("house", STATE::UNLOCK);
  //////////////////// mpd
  test_context.main_iDomStatus->setObjectState("music", STATE::PLAY);
  test_context.main_iDomStatus->setObjectState("speakers", STATE::ON);
  test_context.idom_all_state.houseState = STATE::LOCK;

  test_context.main_iDomStatus->setObjectState("listwa", STATE::ON);
  test_alarmTime.time = Clock::getTime();
  test_alarmTime.state = STATE::ACTIVE;
  test_alarmTime.fromVolume = 0;
  test_alarmTime.toVolume = 100;
  test_alarmTime.radioID = 44;
  test_alarmTime.commands.push_back("command 1");
  test_alarmTime.commands.push_back("command 2");
  test_context.alarmTime = test_alarmTime;
  test_context.main_iDomStatus->setObjectState("alarm", test_alarmTime.state);
  test_context.serverStarted = true;

  test_context.main_iDomTools->saveState_iDom(test_context.serverStarted);

  nlohmann::json testJson;
  std::ifstream i(test_server_set._server.saveFilePath);
  i >> testJson;
  EXPECT_STREQ(test_context.main_iDomStatus->getObjectStateString("music").c_str(),
               testJson.at("MPD").at("music").get<std::string>().c_str());
  EXPECT_STREQ((test_context.main_iDomStatus->getObjectStateString("alarm")).c_str(),
               testJson.at("ALARM").at("alarm").get<std::string>().c_str());
  EXPECT_EQ(test_alarmTime.radioID,
            testJson.at("ALARM").at("radioID").get<int>());
  EXPECT_EQ(test_alarmTime.fromVolume,
            testJson.at("ALARM").at("fromVolume").get<int>());
  EXPECT_EQ(test_alarmTime.toVolume,
            testJson.at("ALARM").at("toVolume").get<int>());

  ////////////////////////////////// read
  test_context.alarmTime.state = STATE::UNDEFINE;
  test_context.alarmTime.commands.clear();
  iDom_SAVE_STATE info(test_context.server_settings->_server.saveFilePath);
  nlohmann::json inf = info.read();
  test_context.main_iDomTools->readState_iDom(inf);
  EXPECT_EQ(test_context.alarmTime.state, STATE::ACTIVE);

  EXPECT_STREQ(test_context.alarmTime.commands.at(0).c_str(), "command 1");
  test_context.server_settings->_server.saveFilePath = "null";
  EXPECT_NO_THROW(test_context.main_iDomTools->readState_iDom(info.read()));
}

TEST_F(iDomTOOLS_ClassTest, checkLightning)
{
  test_context.server_settings->_server.lightningApiURL = "http://45.90.3.84/test/json/lightning.json";
  test_context.main_iDomTools->checkLightning();
}

TEST_F(iDomTOOLS_ClassTest, updateTemperatureStats)
{
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));
  test_context.main_iDomTools->updateTemperatureStats();
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(16));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(16));

  test_context.main_iDomTools->updateTemperatureStats();

  ////////////// maleje na mieskzaniu
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(16));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));

  test_context.main_iDomTools->updateTemperatureStats();
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("temperatura maleje"));
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("mieszkaniu"));

  ////////////// maleje na polu
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(12));

  test_context.main_iDomTools->updateTemperatureStats();
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("temperatura maleje"));
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("polu"));

  ////////////// rośnie na mieskzaniu
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(17));

  test_context.main_iDomTools->updateTemperatureStats();
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("temperatura rośnie"));
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("mieszkaniu"));

  ////////////// rośnie na polu
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(17));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(17));

  test_context.main_iDomTools->updateTemperatureStats();
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("temperatura rośnie"));
  EXPECT_THAT(TEST_DATA::return_viber_msg, testing::HasSubstr("polu"));
}

TEST_F(iDomTOOLS_ClassTest, speakersON_OFF)
{
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::UNLOCK;
  test_context.main_iDomTools->turnOnSpeakers();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::ON);
  test_context.main_iDomTools->turnOffSpeakers();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::LOCK;
  test_context.main_iDomTools->turnOnSpeakers();
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
  std::string retStr = useful_F::myStaticCtx->myEventHandler.run("speakers")->getEvent();
  EXPECT_THAT(retStr, testing::HasSubstr("speakers can not start due to home state: LOCK"));
}

TEST_F(iDomTOOLS_ClassTest, onOff230vOutdoor)
{
  //   test_context.main_iDomTools->turnOnOff230vOutdoor();
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_433MHzSwitch)
{
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::UNLOCK;
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::UNKNOWN);
  test_context.main_iDomTools->turnOn433MHzSwitch("B");
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::ON);
  test_context.main_iDomTools->turnOff433MHzSwitch("B");
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, turn_On_Off_fake_433MHzSwitch)
{
  EXPECT_NO_THROW(test_context.main_iDomTools->turnOn433MHzSwitch("B-fake"));
  EXPECT_NO_THROW(test_context.main_iDomTools->turnOff433MHzSwitch("B-fake"));
  EXPECT_NO_THROW(test_context.main_iDomTools->turnOnOff433MHzSwitch("B-fake"));
}

TEST_F(iDomTOOLS_ClassTest, turnOnOff433MHzSwitch)
{
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::UNLOCK;
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::UNKNOWN);
  test_context.main_iDomStatus->setObjectState("B", STATE::ON);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::ON);
  test_context.main_iDomTools->turnOnOff433MHzSwitch("B");
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::OFF);
  test_context.main_iDomTools->turnOnOff433MHzSwitch("B");
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("B"), STATE::ON);
}

TEST_F(iDomTOOLS_ClassTest, runOnSunset)
{
  test_context.idom_all_state.houseState = STATE::UNLOCK;
  test_context.main_iDomTools->runOnSunset();
  EXPECT_EQ(test_context.main_REC->getEqPointer("B")->getState(), STATE::ON);

  test_context.idom_all_state.houseState = STATE::LOCK;
  test_context.main_iDomTools->runOnSunset();
  EXPECT_EQ(test_context.main_REC->getEqPointer("B")->getState(), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, runOnSunrise)
{
  test_context.idom_all_state.houseState = STATE::UNLOCK;
  test_context.main_iDomTools->runOnSunrise();
  EXPECT_EQ(test_context.main_REC->getEqPointer("B")->getState(), STATE::ON);

  test_context.idom_all_state.houseState = STATE::LOCK;
  test_context.main_iDomTools->runOnSunrise();
  EXPECT_EQ(test_context.main_REC->getEqPointer("B")->getState(), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, getSunrise_Sunset)
{
  std::string ret = test_context.main_iDomTools->getSunrise();
  EXPECT_THAT(ret, testing::HasSubstr(":"));
  std::cout << "sunrise: " << ret << std::endl;

  ret = test_context.main_iDomTools->getSunset();
  EXPECT_THAT(ret, testing::HasSubstr(":"));

  std::cout << "sunset: " << ret << std::endl;

  ret = test_context.main_iDomTools->getSunrise(true);
  EXPECT_THAT(ret, testing::HasSubstr("Sunrise time:"));

  ret = test_context.main_iDomTools->getSunset(true);
  EXPECT_THAT(ret, testing::HasSubstr("Sunset time:"));
}

TEST_F(iDomTOOLS_ClassTest, getDayLenght)
{
  std::string ret = test_context.main_iDomTools->getDayLenght();
  EXPECT_THAT(ret, testing::HasSubstr(":"));

  ret = test_context.main_iDomTools->getDayLenght(true);
  EXPECT_THAT(ret, testing::HasSubstr("Day Lenght :"));
}

TEST_F(iDomTOOLS_ClassTest, getTextToSpeach)
{

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(22.0));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-12.0));
  TEST_DATA::return_send_to_arduino = "22:23";
  std::string ret = test_context.main_iDomTools->getTextToSpeach();
  EXPECT_THAT(ret, testing::HasSubstr("Smog:"));
  std::cout << "TEXT :" << std::endl
            << ret << std::endl;
}

TEST_F(iDomTOOLS_ClassTest, mpd)
{
  test_context.ptr_MPD_info = std::make_unique<MPD_info>();
  test_context.ptr_MPD_info->volume = 3;
  blockQueue test_q;
  test_q._clearAll();
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::LOCK;
  test_context.main_iDomTools->MPD_play(&test_context);
  std::string retStr = useful_F::myStaticCtx->myEventHandler.run("MPD")->getEvent();
  EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
  EXPECT_EQ(test_q._size(), 0);

  useful_F::myStaticCtx->idom_all_state.houseState = STATE::UNLOCK;
  test_context.main_iDomTools->MPD_play(&test_context);
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY);
  EXPECT_EQ(test_q._size(), 0);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  useful_F::myStaticCtx->idom_all_state.houseState = STATE::LOCK;

  useful_F::myStaticCtx->myEventHandler.run("MPD")->clearEvent();
  test_context.main_iDomTools->MPD_play(&test_context, 2);
  retStr = useful_F::myStaticCtx->myEventHandler.run("MPD")->getEvent();
  EXPECT_THAT(retStr, testing::HasSubstr("MPD can not start due to home state: LOCK"));
  EXPECT_EQ(test_q._size(), 0);

  useful_F::myStaticCtx->idom_all_state.houseState = STATE::UNLOCK;
  test_context.main_iDomTools->MPD_play(&test_context, 2);
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::PLAY_ID);
  EXPECT_EQ(test_q._size(), 0);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  test_context.main_iDomTools->MPD_stop();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::STOP);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_next();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::NEXT);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_prev();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::PREV);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_pause();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::PAUSE);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_volumeUp();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLUP);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_volumeDown();
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLDOWN);
  EXPECT_EQ(test_q._size(), 0);

  test_context.main_iDomTools->MPD_volumeSet(&test_context, 99);
  EXPECT_EQ(test_q._size(), 1);
  EXPECT_EQ(test_q._get(), MPD_COMMAND::VOLSET);
  EXPECT_EQ(test_q._size(), 0);
  EXPECT_EQ(test_context.ptr_MPD_info->volume, 99);

  EXPECT_EQ(test_context.main_iDomTools->MPD_getVolume(&test_context), 99);
}

TEST_F(iDomTOOLS_ClassTest, getTemperatureString)
{
  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(1.1));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(-0.3));
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(62.2));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(35.2));
  test_context.lusina.shedFloor.push_back(21);

  EXPECT_STREQ(test_context.main_iDomTools->getTemperatureString().c_str(), "{\"boiler\":62.2,\"currentFlow\":35.2,\"floor\":21.0,\"inside\":-0.3,\"outdoor\":1.1}");
}

TEST_F(iDomTOOLS_ClassTest, textToSpeach)
{
  test_context.ptr_MPD_info->isPlay = true;
  test_context.main_iDomStatus->setObjectState("speakers", STATE::UNDEFINE);

  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
  std::vector<std::string> test_v;
  test_context.main_iDomTools->textToSpeach(&test_v); // empty
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);

  test_v = {"test", "msg", "clock"};
  test_context.ptr_MPD_info->isPlay = true;
  test_context.main_iDomTools->textToSpeach(&test_v);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::UNDEFINE);
  test_context.ptr_MPD_info->isPlay = false;
  test_context.main_iDomTools->textToSpeach(&test_v);
  EXPECT_EQ(test_context.main_iDomStatus->getObjectState("speakers"), STATE::OFF);
}

TEST_F(iDomTOOLS_ClassTest, getWeatherEvent)
{
  TEST_DATA::return_httpPost = "httpPost";
  std::string retStr = test_context.main_iDomTools->getWeatherEvent("test", 10);
  EXPECT_STREQ(retStr.c_str(), "httpPost");
}

TEST_F(iDomTOOLS_ClassTest, isItDay)
{
  Clock::setTime_forBT_usage(12, 12);
  EXPECT_TRUE(test_context.main_iDomTools->isItDay());
  Clock::setTime_forBT_usage(2, 12);
  EXPECT_FALSE(test_context.main_iDomTools->isItDay());
}

TEST_F(iDomTOOLS_ClassTest, ledClear)
{
  TEST_DATA::return_send_to_arduino = "done";
  std::string retStr = test_context.main_iDomTools->ledClear();
  EXPECT_STREQ(retStr.c_str(), "done");
}

TEST_F(iDomTOOLS_ClassTest, getAllDataSunriseSunset)
{
  EXPECT_THAT(test_context.main_iDomTools->getAllDataSunriseSunset(),
              testing::HasSubstr("Days until Y2K"));
}

TEST_F(iDomTOOLS_ClassTest, health_check)
{
  test_context.mqttHandler = std::make_unique<MQTT_mosquitto>("iDomSERVER test");
  test_context.mqttHandler->_subscribed = true;
  test_context.server_settings->_runThread.RFLink = true;

  test_context.main_RFLink = std::make_shared<RFLinkHandler>(&test_context);
  test_context.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
  test_context.main_iDomTools->healthCheck();
  EXPECT_EQ(test_context.iDomProgramState, iDomStateEnum::WORKING);
  test_context.main_RFLink->m_pingTime = Clock::getUnixTime() - 321;
  //  EXPECT_EQ(test_context.iDomProgramState, iDomStateEnum::HARD_RELOAD);
}

TEST_F(iDomTOOLS_ClassTest, getSystemInfo)
{
  std::string ret = test_context.main_iDomTools->getSystemInfo();
  EXPECT_THAT(ret, testing::HasSubstr("uptime"));
  EXPECT_THAT(ret, testing::HasSubstr("Load"));
  EXPECT_THAT(ret, testing::HasSubstr("total RAM"));
  EXPECT_THAT(ret, testing::HasSubstr("free RAM"));
  std::cout << "SYSTEM INFO: " << ret << std::endl;
}

/*
TEST_F(iDomTOOLS_ClassTest, send_data_to_influxdb)
{
  test_context.server_settings->_database.ip = "127.0.0.1";
  test_context.server_settings->_database.port = 12345;
  test_context.server_settings->_database.bucket = "test_user";
  test_context.server_settings->_database.token = "test_pass";
  test_context.server_settings->_database.org = "cynixowo";

  EXPECT_CALL(*testBuderusMock.get(), getOutdoorTemp()).WillOnce(testing::Return(12.0));
  EXPECT_CALL(*testBuderusMock.get(), getInsideTemp()).WillOnce(testing::Return(22.0)); 
  EXPECT_CALL(*testBuderusMock.get(), getBoilerTemp()).WillOnce(testing::Return(60.0));
  EXPECT_CALL(*testBuderusMock.get(), getCurFlowTemp()).WillOnce(testing::Return(35.0));
  EXPECT_CALL(*testBuderusMock.get(), isHeatingActiv()).WillOnce(testing::Return(true));

  test_context.main_iDomTools->send_data_to_influxdb();

  EXPECT_THAT(TEST_DATA::return_httpPost_expect, testing::HasSubstr("http://test.pl"));
  EXPECT_THAT(TEST_DATA::return_httpPost_expect, testing::HasSubstr("test_db"));
  EXPECT_THAT(TEST_DATA::return_httpPost_expect, testing::HasSubstr("test_user"));
}

TEST_F(iDomTOOLS_ClassTest, access_KEYGEN)
{
    test_context.main_iDomTools->m_keyHandler->addKEY("test",256);
    std::cout << "KEY " << test_context.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    test_context.main_iDomTools->m_keyHandler->addTempKEY("tmp",20);

    std::cout << "KEY " << test_context.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    auto ret = test_context.main_iDomTools->m_keyHandler->getKEY("tmp");

    EXPECT_TRUE(test_context.main_iDomTools->m_keyHandler->useKEY("tmp",ret));

    std::cout << "KEY " << test_context.main_iDomTools->m_keyHandler->listKEY() << std::endl;

    ////// remove expired keys
    std::cout << std::endl << "======== remove expired keys ==========" << std::endl;
    test_context.main_iDomTools->m_keyHandler->addTempKEY("tmp1",20);
    test_context.main_iDomTools->m_keyHandler->addTempKEY("tmp2",20);

    std::cout << "KEY " << test_context.main_iDomTools->m_keyHandler->listKEY() << std::endl;
    sleep(1);
    ret = test_context.main_iDomTools->m_keyHandler->getKEY("tmp2");
    test_context.main_iDomTools->m_keyHandler->removeExpiredKeys(0);

    EXPECT_FALSE(test_context.main_iDomTools->m_keyHandler->useKEY("tmp2",ret));
}

TEST_F(iDomTOOLS_ClassTest, openGate_getLink)
{
    test_context.server_settings->_gateway.url = "http://tst.pl?";
    test_context.server_settings->_gateway.keySize = 128;
    auto ret = test_context.main_iDomTools->getLink({"t1","t2"});
    EXPECT_THAT(ret, testing::HasSubstr("http://"));

    std::string t_name = ret.substr(14,20);
    std::string t_key = ret.substr(35,128);

    EXPECT_TRUE(test_context.main_iDomTools->m_keyHandler->useKEY(t_name,t_key));

    std::cout << "wygenerowalem link: " << ret << std::endl;
}
*/
