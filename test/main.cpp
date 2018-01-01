//#include <iostream>
//#include <curl/curl.h>
//#include <stdio.h>
//#include <string.h>
//#include <fstream>

//size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
//{
//    ((std::string*)userp)->append((char*)contents, size * nmemb);
//    return size * nmemb;
//}

//void downloadFile(std::string url, std::string path)
//{
//    CURL *curl;
//    char *fp;
//    CURLcode res;
//    curl = curl_easy_init();
//    if (curl) {
//        std::ofstream file(path);
//        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//        res = curl_easy_perform(curl);
//        file << fp;
//        /* always cleanup */
//        curl_easy_cleanup(curl);
//        file.close();
//    }
//}

//int main(int argc, char *argv[])
//{
//    downloadFile("http://www.telegraph.co.uk/content/dam/Travel/Destinations/Europe/France/Nice/Nice%2C%20france-xlarge.jpg",
//                                              "/home/pi/plik.jpg");


//    return 0;
//}

#include <stdio.h>
#include <curl/curl.h>
/* For older cURL versions you will also need
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "http://www.telegraph.co.uk/content/dam/Travel/Destinations/Europe/France/Nice/Nice%2C%20france-xlarge.jpg";
    char outfilename[FILENAME_MAX] = "/home/pi/zgjecie.jpg";
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}
