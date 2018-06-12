#include <gtest/gtest.h>

#include "/home/pi/programowanie/iDom_server_OOP/test/iDom_TESTs-CMAKE/test_data.h"
#include "../idom_save_state.h"
#include "json.hpp"

TEST(iDomSaveState, main)
{
    iDom_SAVE_STATE test_saveState("/mnt/ramdisk/iDomStateTest.save");
    nlohmann::json test_json ;

    test_json["happy"] = true;
    test_json["pi"] = 3.14;

    nlohmann::json test_json_in_json;
    test_json_in_json["name"] = "cyniu";
    test_json_in_json["age"] = 30;

    test_json["person"] = test_json_in_json;

    test_saveState.write(test_json);
    nlohmann::json test_json2 = test_saveState.read();
    EXPECT_DOUBLE_EQ(test_json.at("pi").get<double>(),
                     test_json2.at("pi").get<double>()) << "wartosci pi nie sa rowne";

    EXPECT_TRUE(test_json2.at("happy").get<bool>()) << " nie jest happy";
}
