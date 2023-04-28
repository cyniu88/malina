#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
/*#include <source_location>
#include <string_view>

void log(const std::string_view message,
         const std::source_location location = 
               std::source_location::current())
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
}*/

int main(int, char *[])
{
    //    log("Hello world!");
    //    fun("Hello C++20!");

    std::unordered_map<std::string, std::string> mapa;
    mapa["dupa"] = "dupa";

    auto lambda = [](const std::unordered_map<std::string, std::string> &mapa) {
        std::cout << "mamy: " << mapa.at("dupa") << std::endl;
        ;
    };

    std::cout << mapa.size() << std::endl;
    auto bb = std::bind(lambda, mapa);
    bb();

    std::cout << mapa.size() << std::endl;
}
