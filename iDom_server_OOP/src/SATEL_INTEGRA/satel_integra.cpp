#include "satel_integra.h"

#include <bitset>
#include <sstream>

#include "../functions/functions.h"

SATEL_INTEGRA::SATEL_INTEGRA()
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

SATEL_INTEGRA::SATEL_INTEGRA(const std::string & host , int port): m_host(host), m_port(port)
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

SATEL_INTEGRA::~SATEL_INTEGRA()
{
    disconnectIntegra();
    iDom_API::removeFromMap(m_className);
}

void SATEL_INTEGRA::setIntegraPin(const std::string & pin)
{
    m_pin = pin;
}

void SATEL_INTEGRA::connectIntegra(const std::string &host, const int port)
{
    m_host = host;
    m_port = port;

    m_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (m_sock == -1){
        m_connectState = STATE::DISCONNECTED;
        puts("Nie można stworzyć połączenia socket");
        return;
    }
    m_server.sin_addr.s_addr = inet_addr(host.c_str());
    m_server.sin_family = AF_INET;

    m_server.sin_port = htons(port);

    if (connect(m_sock , (struct sockaddr *)&m_server , sizeof(m_server)) < 0)
    {
        m_connectState = STATE::DISCONNECTED;
        puts("Nie udało się nawiązać połączenia");
        return;
    }
    m_connectState = STATE::CONNECTED;
}

void SATEL_INTEGRA::disconnectIntegra()
{
    shutdown(m_sock, SHUT_RDWR );
    m_connectState = STATE::DISCONNECTED;
}

void SATEL_INTEGRA::reconnectIntegra()
{
    disconnectIntegra();
    useful_F::sleep(1s);
    connectIntegra(m_host,m_port);
}

std::string SATEL_INTEGRA::getIntegraInfo()
{
    unsigned char cmd[1] = {INTEGRA_ENUM::VERSION};

    //msg.push_back(INTEGRA_ENUM::VERSION); // get integra info

    if (sendIntegra(cmd, 1) == 0){
        return "BAD INTEGRA DATA";
    }

    // (void) recvIntegra();

    std::stringstream rec;
    rec << "Typ centrali: INTEGRA " << satelType(m_message[3])<< std::endl;
    rec << "Wersja: " << m_message[4] << "." << m_message[5] << m_message[6] << " z dnia: " << m_message[7] <<
           m_message[8] << m_message[9] << m_message[10]  << "-" << m_message[11] << m_message[12] << '-' << m_message[13] <<
           m_message[14] << std::endl;
    rec << "Wersja językowa: " << (int)m_message[15] << " (" << (m_message[15]==0 ? "polska" : "inny język") << ")" << std::endl;
    rec << "Dane zapisane w pamięci FLASH: " << (m_message[16]==255 ? "tak" : "nie") << std::endl;
    return rec.str();
}

std::string SATEL_INTEGRA::checkIntegraOut()
{
    unsigned char cmd[1] = { INTEGRA_ENUM::OUTPUTS_STATE };
    if(sendIntegra(cmd, 1) == 0)
    {
        return "BAD INTEGRA DATA";
    }

    // (void) recvIntegra();
    if(m_message[2] not_eq INTEGRA_ENUM::OUTPUTS_STATE){
        std::cout << "zła ramka" << std::endl;
    }
    std::string data;
    data.push_back(m_message[3]);
    data.push_back(m_message[4]);
    data.push_back(m_message[5]);
    data.push_back(m_message[6]);
    return data;
}

bool SATEL_INTEGRA::isAlarmArmed()
{
    unsigned char cmd[1] = {INTEGRA_ENUM::ARMED_PARTITIONS};

    auto io  = sendIntegra(cmd, 1);
#ifdef BT_TEST
    for (int i = 0 ; i < io; ++i){
        char d = m_message[i];
        auto bs = std::bitset<8>(d);
        std::cout << "BITY3 " << bs.to_string() << std::endl;
    }
#endif

    if(m_message[2] not_eq INTEGRA_ENUM::ARMED_PARTITIONS){
        std::cout << "zła odpowiedź servera " << (int) m_message[2] <<std::endl;
        return false;
    }
    switch (m_message[3]) {
    case 0: return false; //not armed
    case 1: return true; // all armed
    default: std::cout << "blad sprawdzania uzborjenia" << std::endl;
    }

    return false;
}

void SATEL_INTEGRA::armAlarm(unsigned int partitionID)
{
    std::stringstream buffer1(m_pin.substr(0,2));
    uint64_t value1;
    buffer1 >> std::hex >> value1;

    std::stringstream buffer2(m_pin.substr(2,2));
    uint64_t value2;
    buffer2 >> std::hex >> value2;

    unsigned char cmd[13];
    for (unsigned int i = 0 ; i < 10; ++i){
        cmd[i] = 0xFF;
    }
    cmd[0] = INTEGRA_ENUM::ARM;
    cmd[1] = value1;
    cmd[2] = value2;
    unsigned char byteNumber = (partitionID - 1) / 8;
    unsigned char bitNumber = (partitionID - 1) % 8;

    cmd[byteNumber + 9] = 0x01 << bitNumber;

    if(sendIntegra(cmd,13) == 0){
            throw "SATEL BAD RECV";
    }
}

void SATEL_INTEGRA::disarmAlarm(unsigned int partitionID)
{
    std::stringstream buffer1(m_pin.substr(0,2));
    uint64_t value1;
    buffer1 >> std::hex >> value1;

    std::stringstream buffer2(m_pin.substr(2,2));
    uint64_t value2;
    buffer2 >> std::hex >> value2;

    unsigned char cmd[13];
    for (unsigned int i = 0 ; i < 10; ++i){
        cmd[i] = 0xFF;
    }
    cmd[0] = INTEGRA_ENUM::DISARM;
    cmd[1] = value1;
    cmd[2] = value2;
    unsigned char byteNumber = (partitionID - 1) / 8;
    unsigned char bitNumber = (partitionID - 1) % 8;

    cmd[byteNumber + 9] = 0x01 << bitNumber;

    if(sendIntegra(cmd,13) == 0){
            throw "SATEL BAD RECV";
    }
}

void SATEL_INTEGRA::outputOn(unsigned int id)
{
    std::stringstream buffer1(m_pin.substr(0,2));
    uint64_t value1;
    buffer1 >> std::hex >> value1;

    std::stringstream buffer2(m_pin.substr(2,2));
    uint64_t value2;
    buffer2 >> std::hex >> value2;

    unsigned char cmd[41] = { 0xFF };
    for (unsigned int i = 0 ; i < 6; ++i){
        cmd[i] = 0xFF;
    }
    cmd[0] = INTEGRA_ENUM::OUTPUT_ON;
    cmd[1] = value1;
    cmd[2] = value2;

    unsigned char byteNumber = (id - 1) / 8;
    unsigned char bitNumber = (id - 1) % 8;

    cmd[byteNumber + 9] = 0x01 << bitNumber;

    if(sendIntegra(cmd,41) == 0){
            throw "SATEL BAD RECV";
    }
}

void SATEL_INTEGRA::outputOff(unsigned int id)
{

    std::stringstream buffer1(m_pin.substr(0,2));
    uint64_t value1;
    buffer1 >> std::hex >> value1;

    std::stringstream buffer2(m_pin.substr(2,2));
    uint64_t value2;
    buffer2 >> std::hex >> value2;

    unsigned char cmd[41] = { 0xFF };
    for (unsigned int i = 0 ; i < 6; ++i){
        cmd[i] = 0xFF;
    }
    cmd[0] = INTEGRA_ENUM::OUTPUT_OFF;
    cmd[1] = value1;
    cmd[2] = value2;

    unsigned char byteNumber = (id - 1) / 8;
    unsigned char bitNumber = (id - 1) % 8;

    cmd[byteNumber + 9] = 0x01 << bitNumber;

    if(sendIntegra(cmd,41) == 0){
            throw "SATEL BAD RECV";
    }
}

std::string SATEL_INTEGRA::dump() const
{
    std::stringstream ss;
    ss << "m_message " << m_message << std::endl;
    ss << "m_host " << m_host << std::endl;
    ss << "m_port " << m_port << std::endl;
    return ss.str();
}

STATE SATEL_INTEGRA::connectionState()
{
    return m_connectState;
}

void SATEL_INTEGRA::calculateCRC(const unsigned char* pCmd, unsigned int length, unsigned short &result)
{
    unsigned short crc = 0x147A;

    for (unsigned int i = 0; i < length; ++i)
    {
        crc = (crc << 1) | (crc >> 15);
        crc = crc ^ 0xFFFF;
        crc = crc + (crc >> 8) + pCmd[i];
    }

    result = crc;
}


int SATEL_INTEGRA::sendIntegra(const unsigned char* cmd, const unsigned int cmdLength){
    std::lock_guard<std::mutex> m_lock(sendMutex);
    //std::pair<unsigned char*, unsigned int> cmdPayload;
    auto cmdPayload = getFullFrame(cmd, cmdLength);
#ifdef BT_TEST
    std::cout << "cyniu lenght: " << (sizeof(cmd)) << " cmdLength " << cmdLength << std::endl;

    for (unsigned int i = 0 ; i < cmdPayload.size(); ++i){
        char d = (const char)cmdPayload.at(i);
        auto bs = std::bitset<8>(d);
        std::cout << "BITY2 " << bs.to_string() << std::endl;
    }
#endif
    int size = send(m_sock, cmdPayload.c_str(), cmdPayload.size(), MSG_NOSIGNAL);
    if (size <1){
        return size;
    }
    return recvIntegra();
}

int SATEL_INTEGRA::recvIntegra()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv , sizeof(struct timeval));

    int size = recv(m_sock, m_message, 2000, 0);
    if (size < 6) {
        puts("Nie udało się pobrać odpowiedzi z serwera");
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Satel Integra: received frame is too short." << std::endl;
        log_file_mutex.mutex_unlock();
        connectIntegra(m_host,  m_port);
        return 0;
    }
    if (m_message[0] not_eq INTEGRA_ENUM::HEADER_MSG
            or m_message[1] not_eq INTEGRA_ENUM::HEADER_MSG
            or m_message[size-1] not_eq INTEGRA_ENUM::END
            or m_message[size-2] not_eq INTEGRA_ENUM::HEADER_MSG){
        puts("Urządzenie zajęte (busy) lub niewłaściwy format odpowiedzi");
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Satel Integra: received bad frame (prefix or sufix)" << std::endl;
        log_file_mutex.mutex_unlock();
        return 0;
    }
    unsigned int answerLength = 0;
    unsigned char answer[100];
    for (int i = 0; i < size - 6; i++)
        if (m_message[i + 2] != 0xF0 || m_message[i + 1] != 0xFE) // skip special value
        {
            answer[answerLength] = m_message[i + 2];
            answerLength++;
        }
    unsigned short crc;
    calculateCRC(answer, answerLength, crc);

    if ((crc & 0xFF) != m_message[size - 3] || (crc >> 8) != m_message[size - 4])
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Satel Integra: receive bad CRC :(" << std::endl << "crc & 0xFF          = " << (crc & 0xFF)<< std::endl
                      << "m_message[size - 3] = " << (int)m_message[size - 3] <<std::endl
                      << "(crc >> 8)          = " << (crc >> 8)<<std::endl
                      << "m_message[size - 4] = " << (int)m_message[size - 4] <<std::endl;;
        log_file_mutex.mutex_unlock();
        return 0;
    }

    return size;
}

const char* SATEL_INTEGRA::satelType(unsigned char t)
{
    switch(t)
    {
    case 0: return "24";
    case 1: return "32";
    case 2: return "64";
    case 3: return "128";
    case 4: return "128-WRL SIM300";
    case 132: return "128-WRL LEON";
    case 66: return "64 PLUS";
    case 67: return "128 PLUS";
    case 72: return "256 PLUS";
    default: return "Nieznany typ";
    }
}

void SATEL_INTEGRA::expandForSpecialValue(std::list<unsigned char> &result)
{
    const unsigned char specialValue = 0xFE;

    for (auto it = result.begin(); it != result.end(); ++it)
    {
        if (*it == specialValue)
        {
            result.insert(++it, 0xF0);
            it--;
        }
    }
}

std::string SATEL_INTEGRA::getFullFrame(const unsigned char* pCmd, const unsigned int cmdLength)
{
    std::string message;
    std::list<unsigned char> result;

    for (unsigned int i = 0; i< cmdLength; ++i)
    {
        result.push_back(pCmd[i]);
    }

    // add CRC
    unsigned short crc;
    calculateCRC(pCmd, cmdLength, crc);
    result.push_back(crc >> 8);
    result.push_back(crc & 0xFF);
    // check special value
    expandForSpecialValue(result);
    // add prefix
    result.push_front(INTEGRA_ENUM::HEADER_MSG);
    result.push_front(INTEGRA_ENUM::HEADER_MSG);
    // add sufix
    result.push_back(INTEGRA_ENUM::HEADER_MSG);
    result.push_back(INTEGRA_ENUM::END);

    for(const auto& it : result)
    {
        message.push_back(it);
    }
    return message;
}
