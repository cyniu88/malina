#include "functions.h"

std::vector<std::string> useful_F::split(const std::string& s, char separator ){
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    try {
        output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    }
    catch (...){
#ifndef BT_TEST
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "wyjatek substr() w useful_F::split() !!!!!!"<< std::endl;
        log_file_mutex.mutex_unlock();
#endif
    }
    return output;
}
thread_data* useful_F::myStaticData = NULL;
void useful_F::setStaticData(thread_data *my_dataPtr)
{
    myStaticData = my_dataPtr;
}

std::string useful_F::httpPost(std::string url, int timeoutSeconds)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, iDomTOOLS::WriteCallback);
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

    return readBuffer;
}
