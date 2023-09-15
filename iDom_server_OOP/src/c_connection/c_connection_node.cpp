#include <iostream>
#include "c_connection.h"
#include "../HTTP/http.h"
#include "json.hpp"
#include "../iDomTools/idomtools_interface.h"

void C_connection::setEncriptionKey(const std::string &key)
{
    m_encriptionKey = key;
}

void C_connection::setEncrypted(bool flag)
{
    m_encrypted = flag;
}

void C_connection::crypto(std::string &toEncrypt, std::string key, bool encrypted)
{
    if (!encrypted)
    {
        return;
    }
    unsigned int keySize = key.size() - 1;
#ifdef BT_TEST
    std::cout << "key: " << key << " size: " << key.size() << std::endl;
#endif
    for (unsigned int i = 0; i < toEncrypt.size(); i++)
    {
        if (keySize == 0)
            keySize = key.size() - 1;
        else
            --keySize;
        toEncrypt[i] ^= key[keySize];
    }
}

void C_connection::onStartConnection()
{
}

void C_connection::onStopConnection()
{
    // my_data->main_iDomTools->cameraLedOFF(my_data->server_settings->_camera.cameraLedOFF);
}

void C_connection::cryptoLog(std::string &toEncrypt)
{
    crypto(toEncrypt, m_encriptionKey, m_encrypted);
}

void C_connection::handleHTTP(const std::string &msg)
{
    std::vector<std::string> dataToSend;
    std::string htmlHEAD = R"(<!DOCTYPE html>
                            <html lang="en">
                            <head>
                                <meta charset="UTF-8">
                                <meta http-equiv="X-UA-Compatible" content="IE=edge">
                                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                                <title>iDom gateway</title>
                                <link rel="icon" type="image/x-icon" href="http://cyniu88.no-ip.pl/favicon.ico">
                            </head>)";

    if (Http::getContentType(msg) == Content_Type::ApplicationJSON and Http::getUrl(msg) == "/iDom/log")
    {
        nlohmann::json jj = nlohmann::json::parse(Http::getContent(msg));
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "logowanie z ESP: " << jj["msg"] << " millis: " << jj["millis"] << std::endl;
        log_file_mutex.mutex_unlock();
        std::string msgHTML = R"(ok)";
        std::string msgHTTP = R"(HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: )" + std::to_string(msgHTML.length()) + "\r\n\r\n";

        dataToSend.push_back(msgHTTP);
        dataToSend.push_back(msgHTML);
    }

    else if (Http::getUrl(msg) == "/run/command/")
    {
        auto query = Http::getQuery(msg);

        auto command = my_data->m_keyHandler->getCommand(query["name"]);

        std::string data;
        std::string color = "#00ff00";
        if (my_data->m_keyHandler->useKEY(query["name"], query["key"]))
        {
            auto topic = my_data->server_settings->_mqtt_broker.topicSubscribe;
            topic.pop_back();
            my_data->mqttHandler->publish(topic + "command", command);
            data = " wyslano komende!";
        }
        else
        {
            data = "operacja niedozwolona!";
            color = "#ff0000";
        }
        // std::string msgHTML = htmlHEAD + R"(<body><center><span style="color: red">)" + data + R"(</span></center></body></html>)";
        std::string msgHTML = htmlHEAD + R"(<body>
                            <center><div>
                                <a id="redirect-btn" target="main">
                                <button type="button" style="width: 80%; padding: 80px; cursor: pointer; box-shadow: 6px 6px 5px; #999; -webkit-box-shadow: 6px 6px 5px #999; -moz-box-shadow: 6px 6px 5px #999; font-weight: bold; background: )"
                                 + color 
                                 + R"(; color: #000; border-radius: 10px; border: 1px solid #999; font-size: 150%; text-align: center;"> )" 
                                 + data 
                                 + R"(</button> </a>
                            </div>
                            </center>
                            <br>
                            <iframe  frameborder="0" name="main" width=100% height=600 align="left" >koko</iframe>

                            <center>13-09-2023 <a href=" " target=_blank title="13-09-2023">Cyniu</a></center>

                            <script>
                                const baseLink = "http://cyniu88.no-ip.pl:48833/gateway.html";

                                var urlQuery = new URLSearchParams(window.location.search);
                                //urlQuery = urlQuery.split('=').join(' ');

                                document.getElementById("redirect-btn").href = baseLink + "?" + urlQuery;

                            </script>
                            </body>
                            </html>)";

        std::string msgHTTP = R"(HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: )" + std::to_string(msgHTML.length()) + "\r\n\r\n";

        dataToSend.push_back(msgHTTP);
        dataToSend.push_back(msgHTML);

        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "odebrano HTTP " << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }
    else
    {
        std::string msgHTML = htmlHEAD + R"(<body>
                            <center><div>
                                <a id="redirect-btn" target="main">
                                <button type="button" style="width: 80%; padding: 80px; cursor: pointer; box-shadow: 6px 6px 5px; #999; -webkit-box-shadow: 6px 6px 5px #999; -moz-box-shadow: 6px 6px 5px #999; font-weight: bold; background: #ffff00; color: #000; border-radius: 10px; border: 1px solid #999; font-size: 350%; text-align: center;">Click Me!</button> </a>
                            </div>
                            </center>
                            <br>
                            <iframe  frameborder="0" name="main" width=100% height=600 align="left" >koko</iframe>

                            <center>13-09-2023 <a href=" " target=_blank title="13-09-2023">Cyniu</a></center>

                            <script>
                                const baseLink = "http://cyniu88.no-ip.pl:48833/run/command/";

                                var urlQuery = new URLSearchParams(window.location.search);
                                //urlQuery = urlQuery.split('=').join(' ');

                                document.getElementById("redirect-btn").href = baseLink + "?" + urlQuery;

                            </script>
                            </body>
                            </html>)";

        std::string msgHTTP = R"(HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: )" + std::to_string(msgHTML.length()) + "\r\n\r\n";

        dataToSend.push_back(msgHTTP);
        dataToSend.push_back(msgHTML);

        log_file_mutex.mutex_lock();
        log_file_cout << DEBUG << "odebrano HTTP " << msg << std::endl;
        log_file_mutex.mutex_unlock();
    }
    for (const auto &d : dataToSend)
        c_sendPure(d);
}
