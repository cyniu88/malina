
#include "../recuperator_interface.h"
#include <gmock/gmock.h>
#include <unordered_map>
#include <string>
#include <optional>
#include <any>

class RecuperatorMock : public RecuperatorInterface {
public:
    ~RecuperatorMock() = default;
    MOCK_METHOD0(getData,std::unordered_map<std::string, std::unordered_map<std::string, std::optional<std::any>>>());
    MOCK_METHOD1(setDataFromMqtt, void(const std::pair<std::string, std::string>& data));
    MOCK_METHOD0(stop, void());
    MOCK_METHOD1(setSpeed, void(const std::string_view speed));
    MOCK_METHOD3(setAwayModeTime, void(bool state, int time, std::string_view unit));
    MOCK_METHOD0(setAwayMode, void());
    MOCK_METHOD3(setBoostMode, void(int time, std::string_view unit, bool state));

};
