#include <stdio.h>
#include <curl/curl.h>
/* For older cURL versions you will also need
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string>

#include "../iDom_server_OOP/ftplibpp/ftplib.h"

int main(void) {
    ///////////// ftp
    ///
    ///
    ftplib myFTP;

    myFTP.Connect("192.168.1.1:21");
    myFTP.Login("cyniu","$506cyniU");

    myFTP.Put("/home/pi/plik.jpg","/twardziel/iDom/mojFTPplik.jpg", ftplib::image);

    myFTP.Quit();
    return 0;
}
