
// #include "httplib.h"

// int main(void)
// {
//     using namespace httplib;
//     httplib::Server svr;

//     svr.Get("/hi", [](const httplib::Request & req, httplib::Response &res)
//             { res.set_content("Hello World!", "text/plain"); });
//     svr.Get("/stop", [](const httplib::Request & req, httplib::Response &res)
//             {
//     //res.set_content("stop", "text/plain");
//     res.stop(); });

//     svr.listen("0.0.0.0", 8080);
// }

#include "httplib.h"

int main(void)
{

    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &req, httplib::Response &res)
            { res.set_content("Hello World!", "text/plain"); });

    svr.Get("/stop", [&](const httplib::Request &req, httplib::Response &res)
            {
    res.set_content("stop", "text/plain");
    svr.stop(); });

    svr.Get("/play", [](const httplib::Request &req, httplib::Response &res)
            { 
                int r = system("tinyplayer /mnt/UDISK/CYNIU/imp.mp3");
                std::string result{"Wynik odgrywania: "};
                result += std::to_string(r);
                res.set_content(result, "text/plain"); });

    svr.listen("0.0.0.0", 8080);
}
