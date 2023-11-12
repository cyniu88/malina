#include <gmock/gmock.h>

#include "../db-client-if.hpp"

class dbClient_MOCK : public dbClientIf
{
public:
    ~dbClient_MOCK() = default;
    MOCK_METHOD1(upload_iDomData, int(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &));
	MOCK_METHOD2(uploadBulbData, int(const std::string &name, bool state));
};