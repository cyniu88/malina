#include <stdio.h>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include <iostream>
// Client side implementation of UDP client-server model
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // strlen()
#include <stdbool.h>

#include <sys/socket.h> // socket()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // inet_pton()
#include <netdb.h>      // gethostbyname()

#define SERWER_PORT 56747
#define SERWER_IP "192.168.13.121"

const char *getIpByName(const char *hostName)
{
  struct hostent *he = NULL;

  if ((he = gethostbyname(hostName)) == NULL)
  {
    herror("gethostbyname");
    exit(-1);
  }

  const char *ipAddress = inet_ntoa(**(struct in_addr **)he->h_addr_list);
  puts(ipAddress);
  return ipAddress;
}
int main()
{
  {
    struct sockaddr_in serwer =
        {
            .sin_family = AF_INET,
            .sin_port = htons(SERWER_PORT)};
    if (inet_pton(AF_INET, SERWER_IP, &serwer.sin_addr) <= 0)
    {
      perror("inet_pton() ERROR");
      exit(1);
    }

    const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0)
    {
      perror("socket() ERROR");
      exit(1);
    }

    char buffer[4096];
    buffer[0] = 0x0a;
    buffer[1] = 0x00;
    printf("|Message for server|: %s \n", buffer);

    socklen_t len = sizeof(serwer);

    if (sendto(socket_, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&serwer, len) < 0)
    {
      perror("sendto() ERROR");
      exit(1);
    }

    struct sockaddr_in from = {};

    memset(buffer, 0, sizeof(buffer));
    if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &len) < 0)
    {
      perror("recvfrom() ERROR");
      exit(1);
    }
    printf("|Server's reply|: %s \n end \n", buffer);

    shutdown(socket_, SHUT_RDWR);
  }

  ///////////////////////////////////////////////////////////////// tcp //////////////////////////////////////////////
  std::cout << "start " << std::endl;

  struct sockaddr_in serwer =
      {
          .sin_family = AF_INET,
          .sin_port = htons(SERWER_PORT)};

  const char *ipAddress = getIpByName(SERWER_IP);

  inet_pton(serwer.sin_family, ipAddress, &serwer.sin_addr);

  const int s = socket(serwer.sin_family, SOCK_STREAM, 0);

  connect(s, (struct sockaddr *)&serwer, sizeof(serwer));

  char buffer[10000];
  std::string number;

  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x28);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x01);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x2b);
  number.push_back(0x10);
  number.push_back(0x14);
  number.push_back(0x80);
  number.push_back(0x01);
  number.push_back(0x14);
  number.push_back(0x4f);
  number.push_back(0xd7);
  number.push_back(0x1e);
  number.push_back(0x09);
  number.push_back(0xc8);
  number.push_back(0x00);
  number.push_back(0x04);
  number.push_back(0x08);
  number.push_back(0x03);
  number.push_back(0x20);
  number.push_back(0x01);
  number.push_back(0x08);
  number.push_back(0x01);

  std::cout << "wysłano: " << send(s, number.c_str(), number.size(), 0) << std::endl;

  std::cout << "odebrano: " << recv(s, buffer, sizeof(buffer), 0) << std::endl;
  number.clear();

  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x37);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x01);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x2b);
  number.push_back(0x10);
  number.push_back(0x14);
  number.push_back(0x80);
  number.push_back(0x01);
  number.push_back(0x14);
  number.push_back(0x4f);
  number.push_back(0xd7);
  number.push_back(0x1e);
  number.push_back(0x09);
  number.push_back(0xc8);
  number.push_back(0x00);
  number.push_back(0x04);
  number.push_back(0x08);
  number.push_back(0x21);
  number.push_back(0x20);
  number.push_back(0x02);
  number.push_back(0x08);
  number.push_back(0x01);
  number.push_back(0x12);
  number.push_back(0x0d);
  number.push_back(0x84);
  number.push_back(0x15);
  number.push_back(0x01);
  number.push_back(0x01);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x01);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  std::cout << "2 wysłano: " << send(s, number.c_str(), number.size(), 0) << std::endl;

  std::cout << " 2 odebrano: " << recv(s, buffer, sizeof(buffer), 0) << std::endl;
  std::cout << " 2a odebrano: " << recv(s, buffer, sizeof(buffer), 0) << std::endl;
number.clear();

  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x28);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x2b);
  number.push_back(0x10);
  number.push_back(0x14);
  number.push_back(0x80);
  number.push_back(0x01);
  number.push_back(0x14);
  number.push_back(0x4f);
  number.push_back(0xd7);
  number.push_back(0x1e);
  number.push_back(0x09);
  number.push_back(0xc8);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x00);
  number.push_back(0x01);
  number.push_back(0x00);
  number.push_back(0x04);
  number.push_back(0x08);
  number.push_back(0x22);
  number.push_back(0x20);
  number.push_back(0x02);
  number.push_back(0x12);
  number.push_back(0x00);
  
  std::cout << "3 wysłano: " << send(s, number.c_str(), number.size(), 0) << std::endl;
  
  std::cout << std::hex << buffer << std::endl;
  shutdown(s, SHUT_RDWR);
  return 0;
}

// int main()
// {
//
// }
