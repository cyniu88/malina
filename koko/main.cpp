#include <iostream>
#include <iomanip>
#include <ctime>

int main()
{
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    std::cout << "ja_JP: " << std::put_time(&tm, "%c %Z") << '\n';
}
