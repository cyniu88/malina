#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <experimental/source_location>
#include <string_view>

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


}
