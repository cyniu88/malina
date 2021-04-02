#include "satel_integra.h"

#include <bitset>
#include <sstream>

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
    if (m_sock == -1)
        puts("Nie można stworzyć połączenia socket");
    m_server.sin_addr.s_addr = inet_addr(host.c_str());
    m_server.sin_family = AF_INET;

    m_server.sin_port = htons(port);

    if (connect(m_sock , (struct sockaddr *)&m_server , sizeof(m_server)) < 0)
        puts("Nie udało się nawiązać połączenia");
    //printf("Połączony\n");
    m_connectState = STATE::CONNECTED;
}

void SATEL_INTEGRA::disconnectIntegra()
{
    shutdown(m_sock, SHUT_RDWR );
    m_connectState = STATE::DISCONNECTED;
}

std::string SATEL_INTEGRA::getIntegraInfo()
{
    std::string msg;

    msg.push_back(INTEGRA_ENUM::VERSION); // get integra info

    sendIntegra(msg);

    (void) recvIntegra();

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
    std::string msg;
    msg.push_back(INTEGRA_ENUM::OUTPUTS_STATE); // get integra info

    sendIntegra(msg);

    int recSize = recvIntegra();
    if(m_message[2] != INTEGRA_ENUM::OUTPUTS_STATE){
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
    std::string msg;
    msg.push_back(INTEGRA_ENUM::ARMED_PARTITIONS);

    sendIntegra(msg);

    int recSize = recvIntegra();

    if(m_message[2] != INTEGRA_ENUM::ARMED_PARTITIONS){
        std::cout << "zła odpowedz servera" << std::endl;
        return false;
    }
    switch (m_message[3]) {
    case 0: return false; //not armed
    case 3: return true; // all armed
    default: std::cout << "blad sprawdzania uzborjenia" << std::endl;
    }

    return false;
}

void SATEL_INTEGRA::armAlarm()
{
    std::stringstream buffer1(m_pin.substr(0,2));
    uint64_t value1;
    buffer1 >> std::hex >> value1;

    std::stringstream buffer2(m_pin.substr(2,2));
    uint64_t value2;
    buffer2 >> std::hex >> value2;

    std::string msg;
    msg.push_back(INTEGRA_ENUM::ARM);
    msg.push_back(value1);
    msg.push_back(value2);
    sendIntegra(msg);

    int recSize = recvIntegra();
}

void SATEL_INTEGRA::disarmAlarm()
{
    std::string msg;
    msg.push_back(INTEGRA_ENUM::DISARM);

    sendIntegra(msg);

    int recSize = recvIntegra();
}

std::string SATEL_INTEGRA::dump() const
{
    std::stringstream ss;
    ss << "m_message " << m_message << std::endl;
    ss << "m_host " << m_host << std::endl;
    ss << "m_port" << m_port << std::endl;
    return ss.str();
}

STATE SATEL_INTEGRA::connectionState()
{
    return m_connectState;
}

unsigned short SATEL_INTEGRA::calculateCRC(const std::string& msg)
{
    unsigned short crc = 0x147A;

    for (int i = 0; i < msg.size(); ++i)
    {
        crc = (crc << 1) | (crc >> 15);
        crc = crc ^ 0xFFFF;
        crc = crc + (crc >> 8) + msg.at(i);
    }

    return crc;
}

int SATEL_INTEGRA::sendIntegra(const std::string &msg)
{
    std::string message;
    //header
    message.push_back(INTEGRA_ENUM::HEADER_MSG);
    message.push_back(INTEGRA_ENUM::HEADER_MSG);

    // command
    auto crc = calculateCRC(msg);
    message.append(msg);
    message.push_back((crc >> 8) & 0xff); //crc1
    message.push_back(crc & 0xff);   //crc2
    // end message
    message.push_back(INTEGRA_ENUM::HEADER_MSG);
    message.push_back(INTEGRA_ENUM::END);

    int state = send(m_sock, message.c_str(), message.length(), 0 );
    return state;
}

int SATEL_INTEGRA::recvIntegra()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO,(char*)&tv , sizeof(struct timeval));

    int size = recv(m_sock, m_message, 2000, 0);
    if (size < 0) {
        puts("Nie udało się pobrać odpowiedzi z serwera");
        connectIntegra(m_host,  m_port);
        return 0;
    }
    if (m_message[0] != INTEGRA_ENUM::HEADER_MSG
            || m_message[1] != INTEGRA_ENUM::HEADER_MSG
            || m_message[size-1] != INTEGRA_ENUM::END
            || m_message[size-2] != INTEGRA_ENUM::HEADER_MSG)
        puts("Urządzenie zajęte (busy) lub niewłaściwy format odpowiedzi");
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
