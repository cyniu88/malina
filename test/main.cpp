#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
/* For older cURL versions you will also need
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string>

#include "../iDom_server_OOP/ftplibpp/ftplib.h"
#include "../iDom_server_OOP/libs/useful/useful.h"

int main(void) {
double d = 38.123456789;

    std::cout << "start " << to_string_with_precision(d,4) << std::endl;
    return 0;
}
