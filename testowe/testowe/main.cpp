#include <iostream>
#include "../../iDom_server_OOP/json2/json.hpp"

using json = nlohmann::json;

int main()
{
    // create a JSON object
    json j_object = {{"one", 1}, {"two", 2}};

    // call erase
    auto count_one = j_object.erase("one");
    auto count_three = j_object.erase("three");

    // print values
    std::cout << j_object << '\n';
    std::cout << count_one << " " << count_three << '\n';

    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

    // also use emplace_back
    j.emplace_back(1.78);

    // iterate the array
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << *it << '\n';
    }

}
