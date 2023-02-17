#ifndef _HTTP_h_
#define _HTTP_h_

#include <string>
#include <map>

enum class METHOD_HTTP{
    GET,
    POST,
    UNDEF
};

enum class Content_Type{
    ApplicationJSON,
    TextHtml,
    iDom_logging,
    unknonwn
};
struct QUERY
{
    QUERY(const std::string& _key, const std::string& _value): key(_key), value(_value){};
    std::string key;
    std::string value;
};

class Http {
public:
  static std::string getContent(const std::string& msg);
  static int getContentLength(const std::string& msg);
  static Content_Type getContentType(const std::string& msg);
  static METHOD_HTTP getMethod(const std::string& msg);
  static std::string getUrl(const std::string& msg);
  static std::map <std::string, std::string> getQuery(const std::string& msg);
};

#endif // _HTTP_h_
