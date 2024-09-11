
/*
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <experimental/source_location>
#include <string_view>

#include <sys/ioctl.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void log(const std::string_view message,
         const std::experimental::fundamentals_v2::source_location location =
               std::experimental::fundamentals_v2::source_location::current())
{
    std::cout << "file: "
              << location.file_name() << "("
              << location.line() << ":"
              << location.column() << ") `"
              << location.function_name() << "`: "
              << message << '\n';
}

template <typename T> void fun(T x)
{
    log(x);
}

int main(int, char *[])
{
        log("Hello world!");
        fun("Hello C++20!");
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

    struct ifreq ifr{};
    strcpy(ifr.ifr_name, "wlo1");
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);

    char ip[INET_ADDRSTRLEN];
    strcpy(ip, inet_ntoa(((sockaddr_in *) &ifr.ifr_addr)->sin_addr));

    std::cout << ip << std::endl;

    return 0;
}

*/

#include <iostream>
#include <signal.h>
#include <atomic>

static std::atomic<bool> my_work = true;
void my_sig_handler(int s)
{
    printf("\nCaught signal %d\n", s);

    std::cout << std::boolalpha << "zamykam po sygnale " << my_work << std::endl;
    if (my_work == true)
        my_work = false;
    else
        my_work = true;
}

void catchSigInt()
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_sig_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGTERM, &sigIntHandler, NULL);
    sigaction(SIGINT, &sigIntHandler, NULL);
}

// int main(void)
// {
//     catchSigInt();
//     std::cout << "start" << std::endl;
//     while (true)
//     {
//         if (my_work == false)
//         {
//             std::cout << "wychodzę " << std::endl;
//             break;
//         }
//     }
//     std::cout << "stop" << std::endl;
// }

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> command;
    command.push_back("jedne");
    command.push_back("dwa");

    std::string str = std::accumulate(command.begin(), command.end(), std::string(), [](const std::string &a, const std::string &b)
                                      { return a.empty() ? b : a + " " + b; });

    std::cout << "|" << str << "|" << std::endl;
    return 0;
}
