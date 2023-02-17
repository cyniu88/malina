#include "http.h"
#include "../functions/functions.h"
#include <algorithm>


std::string Http::getContent(const std::string& msg)
{
    return msg.substr(msg.length() - Http::getContentLength(msg) );
}

int Http::getContentLength(const std::string &msg)
{
    int size = 0;
    std::string cl{"Content-Length:"};
    auto pos = msg.find(cl);
    size = std::stoi(msg.substr(pos+cl.length(), 3) );
    return size;
}

Content_Type Http::getContentType(const std::string &msg)
{
    if(useful_F_libs::hasSubstring(msg, "iDom/logging"))
        return Content_Type::iDom_logging;
    else if(useful_F_libs::hasSubstring(msg, "application/json"))
        return Content_Type::ApplicationJSON;
    else if(useful_F_libs::hasSubstring(msg, "text/html"))
        return Content_Type::TextHtml;

    return Content_Type::unknonwn;
}

METHOD_HTTP Http::getMethod(const std::string &msg)
{
    METHOD_HTTP method{METHOD_HTTP::GET};
    auto firstLine = msg.substr(0, msg.find("Host"));
    if (firstLine.find("GET") != std::string::npos)
        method = METHOD_HTTP::GET;
    else if (firstLine.find("POST") != std::string::npos)
        method = METHOD_HTTP::POST;
    else
        method = METHOD_HTTP::UNDEF;

    return method;
}

std::string Http::getUrl(const std::string &msg)
{
    std::string str = msg.substr(0,msg.find_first_of("\r\n"));
    auto pos = str.find_first_of("?");
    if(pos == std::string::npos)
        pos = str.find_last_of(" ");
    auto first = str.find_first_of("/");
    str = msg.substr(first, pos - first);
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

std::map <std::string, std::string> Http::getQuery(const std::string &msg)
{
    ///api/gate_open.html?kkk=999&uu=88&lolo
    std::map <std::string, std::string> mapa;

    std::string str = msg.substr(0,msg.find_first_of("\r\n"));
    auto first = str.find_first_of("?") +1;
    auto sec = str.find_last_of(" ");
    str = msg.substr(first, sec - first);
    auto v1  = useful_F::split(str, '&');

    for(const auto& vv : v1)
    {
        std::string::size_type pos{0};
        if( (pos = vv.find('=')) not_eq std::string::npos)
        {
            mapa[vv.substr(0, pos)] =  vv.substr(pos+1) ;
        }
        else{
            mapa[vv.substr(0, pos)] = "" ;
        }
    }

    return mapa;
}
