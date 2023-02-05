#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../http.h"

class http_class_fixture : public testing::Test
{
public:
    const std::string content{R"({"msg":"test viber msg","millis":529443897})"};
    std::string msgPOST = R"(HTTP POST /iDom/cyniu?kkk=999&uu=88&lolo HTTP/1.1
Host: cyniu88.no-ip.pl:48833
User-Agent: ESP8266HTTPClient
Accept-Encoding: identity;q=1,chunked;q=0.1,*;q=0
Connection: keep-alive
Content-Type: iDom/logging
Content-Length: 43\r\n\r\n)" + content;

    std::string msgGET = R"(HTTP GET /api/gate_open.html?kkk=999&uu=88&lolo HTTP/1.1
Host: cyniu88.no-ip.pl:48833
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:102.0) Gecko/20100101 Firefox/102.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: pl,en-US;q=0.7,en;q=0.3
Accept-Encoding: gzip, deflate
DNT: 1
Connection: keep-alive
Referer: http://cyniu88.no-ip.pl:48833/?kkk&uu
Upgrade-Insecure-Requests: 1
)";
};

TEST_F(http_class_fixture, baseTestPOST)
{
    EXPECT_EQ(Http::getContentLength(msgPOST), 43);
    ASSERT_EQ(Http::getContentType(msgPOST), Content_Type::iDom_logging);
    EXPECT_STREQ(std::any_cast<std::string>(Http::getContent(msgPOST)).c_str(), content.c_str());
    EXPECT_EQ(Http::getMethod(msgPOST), METHOD_HTTP::POST);
}

TEST_F(http_class_fixture, baseTestGET)
{
    ASSERT_EQ(Http::getMethod(msgGET), METHOD_HTTP::GET);
}

TEST_F(http_class_fixture, getURL)
{
    EXPECT_STREQ( Http::getUrl(msgGET).c_str(), "/api/gate_open.html" );
    EXPECT_STREQ( Http::getUrl(msgPOST).c_str(), "/iDom/cyniu" );
}

TEST_F(http_class_fixture, getQuery)
{
   auto ov = Http::getQuery(msgPOST);

   for(const auto& o : ov)
   {
       std::cout << "key: " << o.first << ", value: " << o.second << std::endl;
   }

   EXPECT_EQ(ov.at("kkk"), "999");
   EXPECT_EQ(ov.at("lolo"), "");
}
