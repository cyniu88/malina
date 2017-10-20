#include <iostream>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;


    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string data = "message="+msg;
    std::string address = "https://graph.facebook.com/v2.10/me/feed?access_token=EAAHkO5uKQ4YBAHDdy7d34k86c4b9g95WYm0YntDQkDiGaReWZCqr0taqXCNyjSICXeIATZCn9aDG4LGhnY7aS78xAqg5ZBwlqCZBHN8YC3r2GwkIuJDossx2ot6rIjoHuDZA0QjlBU5cHLptZBaAXuTOLJ6w9ySNPedwZAaDpyQLrjvqeOOemF2x";
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    std::cout << "bufor" << std::endl;
    std::cout << readBuffer<< std::endl;

//   CURL *curl = curl_easy_init();

//    char *data="message=Posting from C&";
//    strcat(data, "link=http://placekitten.com&");
//    strcat(data, "picture=http://placekitten.com/90/90&");
//    strcat(data, "name=Meow&");
//    strcat(data, "caption={*actor*} places kittens on the wall&");
//    strcat(data, "description=Some description&");
//    strcat(data, "access_token=EAAHkO5uKQ4YBAHDdy7d34k86c4b9g95WYm0YntDQkDiGaReWZCqr0taqXCNyjSICXeIATZCn9aDG4LGhnY7aS78xAqg5ZBwlqCZBHN8YC3r2GwkIuJDossx2ot6rIjoHuDZA0QjlBU5cHLptZBaAXuTOLJ6w9ySNPedwZAaDpyQLrjvqeOOemF2x");

//    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//    curl_easy_setopt(curl, CURLOPT_URL, "https://graph.facebook.com/me/feed");
//    /* Disable SSL check (only for testing) */
//    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

//    curl_easy_perform(curl); /* post */
    return 0;
}
