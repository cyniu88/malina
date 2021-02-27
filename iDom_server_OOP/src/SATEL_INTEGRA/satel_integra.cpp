#include "satel_integra.h"
const char *rodzajCentrali(unsigned char kod)
{
        switch(kod)
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

SATEL_INTEGRA::SATEL_INTEGRA()
{
    m_className.append(typeid (this).name());
    iDom_API::addToMap(m_className,this);
}

SATEL_INTEGRA::~SATEL_INTEGRA()
{
    iDom_API::removeFromMap(m_className);
}

void SATEL_INTEGRA::connectIntegra(const std::string &host, const int port)
{
    m_sock = socket(AF_INET , SOCK_STREAM , 0);
    if (m_sock == -1)
        puts("Nie można stworzyć połączenia socket");
    m_server.sin_addr.s_addr = inet_addr(host.c_str());
    m_server.sin_family = AF_INET;

    m_server.sin_port = htons(port);

    if (connect(m_sock , (struct sockaddr *)&m_server , sizeof(m_server)) < 0)
        puts("Nie udało się nawiązać połączenia");
    printf("Połączony\n");
}

void SATEL_INTEGRA::disconnectIntegra()
{
    shutdown(m_sock, SHUT_RDWR );
}

std::string SATEL_INTEGRA::getIntegraInfo()
{
    std::string msg;
    std::string msg2;
    //header
    msg.push_back(0xfe);
    msg.push_back(0xfe);

    // command
    msg2.push_back(0x7e); // get integra info

    auto crc = calculateCRC(msg2);
    msg.append(msg2);
    msg.push_back((crc >> 8) & 0xff); //crc1
    msg.push_back( crc & 0xff);   //crc2
    msg.push_back(0xfe); //koniec transmisji 2 bajty
    msg.push_back(0x0d);
    sendIntegra(msg);

    int recSize = recvIntegra();
    const char *typ = rodzajCentrali(m_message[3]);
    printf ("Typ centrali: INTEGRA %s\n", typ);
    printf ("Wersja: %c.%c%c z dnia: %c%c%c%c-%c%c-%c%c\n", m_message[4], m_message[5], m_message[6], m_message[7], m_message[8], m_message[9], m_message[10], m_message[11], m_message[12], m_message[13], m_message[14]);
    printf( "Wersja językowa: %i (%s)\n", m_message[15], (m_message[15]==1 ? "angielska" : "inny język"));
    printf ("Dane zapisane w pamięci FLASH: %s \n", (m_message[16]==255 ? "tak" : "nie"));

    std::stringstream rec;

    return rec.str();
}

std::string SATEL_INTEGRA::dump() const
{
    std::stringstream ss;
    ss << "m_message " << m_message << std::endl;
    return ss.str();
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
    std::cout << "wysylam: " << msg << std::endl;
    return send( m_sock, msg.c_str() ,msg.length(), 0 );
}

int SATEL_INTEGRA::recvIntegra()
{
    int size = recv(m_sock, m_message, 2000, 0);
    std::cout << " cyniu size" << size << std::endl;
    if (size < 0) //rozmiar odp
        puts("Nie udało się pobrać odpowiedzi z serwera");
    if (m_message[0]!=0xfe || m_message[1]!=0xfe || m_message[size-1]!=0x0d || m_message[size-2]!=0xfe)
        puts("Urządzenie zajęte (busy) lub niewłaściwy format odpowiedzi");
    return size;
}
