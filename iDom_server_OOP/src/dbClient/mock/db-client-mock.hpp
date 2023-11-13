#include <gmock/gmock.h>

#include "../db-client-if.hpp"

class dbClient_MOCK : public dbClientIf
{
public:
    ~dbClient_MOCK() = default;
    MOCK_METHOD1(upload_iDomData, HttpStatus::Code(std::unordered_map<std::string, std::unordered_map<std::string, std::any>> &));
	MOCK_METHOD2(uploadBulbData, HttpStatus::Code(const std::string &name, bool state));
};