#include <iostream>
#include <gtest/gtest.h>
#include <gconv.h>
#include <fstream>
#include <sstream>
#include <test_data.h>
#include "../../libs/useful/useful.h"
#include "../../src/SerialPi/serialpi.h"
#include "../../src/functions/functions.h"

#define log_file_cout f_log //std::cout   zmien f_log na std::cout  i bedzie wypisywac na
std::string  _logfile = "/mnt/ramdisk/iDom_log_BT.log";

Logger log_file_mutex(_logfile);


std::string TEST_DATA::return_send_to_arduino = "-2:-2";
std::string TEST_DATA::return_httpPost = "ok.\n";
std::string TEST_DATA::return_httpPost_expect = "";
std::string TEST_DATA::return_viber_msg = "";
std::string TEST_DATA::serial_b = "NULL";
std::string TEST_DATA::serial_sended = "NULL";
std::string TEST_DATA::LCD_print = "NULL";
int TEST_DATA::test_pin = 0;
int TEST_DATA::runLinuxCommandReturn = 0;

unsigned int Clock::m_BT_H = 0;
unsigned int Clock::m_BT_M = 0;

void SerialPi_set_recv_msg(const std::string& m){
    TEST_DATA::serial_b = m;
}

void SerialPi_set_serial_sended(const std::string& m){
    TEST_DATA::serial_sended = m;
}
SerialPi::SerialPi(const std::string& a):m_serial_port(10){
    std::cout << "SerialPi() addres: " << a << std::endl;
}
SerialPi::~SerialPi(){
    std::cout << "~SerialPi()" << std::endl;
}

void SerialPi::print(const std::string& msg){
    std::cout << "SerialPi::print() msg: " << msg << std::endl;
    SerialPi_set_serial_sended(msg);
}


void SerialPi::begin(int serialSpeed){
    std::cout << "SerialPi::int() serialSpeed: " << serialSpeed << std::endl;
}

void SerialPi::flush(){
    std::cout << "SerialPi::flush()" << std::endl;
    TEST_DATA::serial_b.clear();
}

int SerialPi::available(){
    std::cout << "SerialPi::available() " << TEST_DATA::serial_b.size() << std::endl;
    return static_cast<int>(TEST_DATA::serial_b.size());
}

char SerialPi::read(){
    char r = TEST_DATA::serial_b.at(0);
    TEST_DATA::serial_b.erase(0,1);
    std::cout << "SerialPi::read(): "<<r<< std::endl;
    return r;
}

void digitalWrite(int pin, int mode){}

void setReturnPinState(int i)
{
    TEST_DATA::test_pin = i;
}

viber_API::viber_API(){}
void viber_API::setAccessToken(const std::string& accessToken){}
void viber_API::setURL(const std::string& url){}
void viber_API::setAvatar(const std::string& avatar){}
std::string viber_API::sendViberMSG(const std::string& msg,
                                    const std::string& receiver,
                                    const std::string& senderName,
                                    const std::string& accessToken,
                                    const std::string& url){

    std::cout << "sendViberMSG() "<< msg <<" to: "<< receiver << std::endl;
    TEST_DATA::return_viber_msg = msg;
    return"{\"message_status\":\"ok\"}";
}
std::string viber_API::sendViberPicture(const std::string& msg,
                                        const std::string& image,
                                        const std::string& receiver,
                                        const std::string& senderName,
                                        const std::string& accessToken ,
                                        const std::string& url){
    std::cout << "sendViberPicture() "<< msg <<" to: "<< receiver << std::endl;
    TEST_DATA::return_viber_msg = msg;
    return"{\"message_status\":\"ok\"}";
}
FACEBOOK_API::FACEBOOK_API(){}
std::string FACEBOOK_API::postTxtOnWall(const std::string& msg,
                                        const std::string& accessToken ){return "";}
std::string FACEBOOK_API::postPhotoOnWall(const std::string& url,
                                          const std::string& msg ,
                                          const std::string& accessToken ){return "";}
void FACEBOOK_API::setAccessToken(const std::string& token){}

void LCD_c::set_lcd_STATE(int i){}
void LCD_c::printString(bool clear, int col, int row, const std::string& str){
    std::cout << "LCD_c::printString() "<< str << std::endl;
    TEST_DATA::LCD_print = str;
}

std::string useful_F_libs::httpPost(const std::string& url, int timeoutSeconds){

    std::cout << "url: "<< url << " return "<< TEST_DATA::return_httpPost << "|"<< std::endl;
    TEST_DATA::return_httpPost_expect = "httpPost";
    return TEST_DATA::return_httpPost;
}
std::string useful_F_libs::httpPost(const std::string& url){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, useful_F_libs::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return readBuffer;
}

int digitalRead(int pin){ return TEST_DATA::test_pin; }

class test_433
{
public:
    std::string config433 = "{ \"BUTTON\": { \"locker\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"01e7be\", \"name\": \"locker\", \"type\": \"BUTTON\" } }, \"SWITCH\": { \"A\": { \"OFF\": \"10;TriState;0280aa;0;OFF\", \"ON\": \"10;TriState;0280aa;0;ON\", \"id\": \"1444\", \"name\": \"A\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"ALARM\": { \"OFF\": \"dummyOFF\", \"ON\": \"dummyON\", \"id\": \"1223\", \"name\": \"ALARM\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"B\": { \"OFF\": \"10;TriState;02822a;0;OFF\", \"ON\": \"10;TriState;02822a;0;ON\", \"id\": \"1445\", \"name\": \"B\", \"on15sec\": \"10;TriState;02822a;0;ON\", \"sunrise\": \"ON\", \"sunset\": \"OFF\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"C\": { \"OFF\": \"null\", \"ON\": \"null\", \"id\": \"1446\", \"name\": \"C\", \"on15sec\": \"null\", \"sunrise\": \"OFF\", \"sunset\": \"ON\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"listwa\": { \"OFF\": \"10;Kaku;0ad04d;15;OFF\", \"ON\": \"10;Kaku;0ad04d;15;ON\", \"id\": \"1450\", \"name\": \"listwa\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"OFF\", \"unlock\": \"ON\", \"type\": \"SWITCH\" } }, \"WEATHER\": { \"first\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"0704\", \"name\": \"first\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"type\": \"WEATHER\" } } }";
    std::string config433_fake = "{ \"BUTTON\": { \"locker\": { \"OFF\": \"NULL\", \"ON\": \"NULL\", \"id\": \"01e7be\", \"name\": \"locker\",  \"type\": \"BUTTON\" } }, \"SWITCH\": { \"A\": { \"OFF\": \"10;TriState;0280aa;0;OFF\", \"ON\": \"10;TriState;0280aa;0;ON\", \"id\": \"1444\", \"name\": \"A\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"ALARM\": { \"OFF\": \"dummyOFF\", \"ON\": \"dummyON\", \"id\": \"1223\", \"name\": \"ALARM\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"B\": { \"OFF\": \"10;TriState;02822a;0;OFF\", \"ON\": \"10;TriState;02822a;0;ON\", \"id\": \"1445\", \"name\": \"B\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\", \"lock\": \"null\", \"unlock\": \"null\",\"type\": \"SWITCH\" }, \"C\": { \"OFF\": \"10;TriState;02828a;0;OFF\", \"ON\": \"10;TriState;02828a;0;ON\", \"id\": \"1446\", \"name\": \"C\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"null\", \"unlock\": \"null\", \"type\": \"SWITCH\" }, \"listwa\": { \"OFF\": \"10;Kaku;0ad04d;15;OFF\", \"ON\": \"10;Kaku;0ad04d;15;ON\", \"id\": \"1450\", \"name\": \"listwa\", \"on15sec\": \"null\", \"sunrise\": \"NULL\", \"sunset\": \"NULL\",\"lock\": \"OFF\", \"unlock\": \"null\", \"type\": \"SWITCH\" } } } }";

    void makeFile(const std::string& path)
    {
        std::ofstream o(path);
        o << config433 ;
    }
    void makeFileFake(const std::string& path)
    {
        std::ofstream o(path);
        o << config433_fake ;
    }
};

int main(int argc, char **argv)
{
    test_433 t;
    t.makeFile("/mnt/ramdisk/433_eq_conf.json");
    t.makeFileFake("/mnt/ramdisk/433_eq_conf_fake.json");

    std::fstream ofs;
    ofs.open(_logfile, std::ios::out | std::ios::trunc);
    ofs.close(); //Using microsoft incremental linker version 14

    pthread_mutex_init(&Logger::mutex_log, NULL);

    log_file_mutex.mutex_lock();
    log_file_cout << std::endl<< std::endl<<"============================================"
                  << std::endl<< std::endl<< std::endl<< INFO << "START BT "<< std::endl;
    log_file_mutex.mutex_unlock();

    ::testing::InitGoogleTest( &argc, argv );
    //::testing::GTEST_FLAG(filter) = "functions_fixture.sleepThread";
    //::testing::GTEST_FLAG(filter) = "event_counter_fixture.getLast1minNumberEvent";
    //::testing::GTEST_FLAG(filter) = "command433MHz_Class_fixture.*)";
    auto ret = RUN_ALL_TESTS();
    pthread_mutex_destroy(&Logger::mutex_log);

    const char * PROG_INFO = " Basic TEST iDomServer: " __DATE__ ", " __TIME__;
    std::cout << "wersja " <<PROG_INFO <<" "<< GIT_BRANCH <<" " << GIT_COMMIT_HASH << std::endl;
    return ret;
}
