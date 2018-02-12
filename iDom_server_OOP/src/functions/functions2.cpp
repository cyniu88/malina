#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <vector>

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

std::string useful_F::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

void useful_F::tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text){
    std::string temp;

    for(char n: text) { // the initializer may be an array
        bool is_sep = false;
        for(char m: separator){
            if (n == m)
            {
                is_sep = true;
            }
        }
        if (is_sep == false){
            temp += n;
        }
        else
        {
            if (!temp.empty())
            {
                command.push_back( temp);
                temp = "";
            }
        }
    }
    if (!temp.empty()){
        command.push_back(temp);
    }
}

std::string useful_F::removeHtmlTag(std::string &data)
{
    data = useful_F::replaceAll(data,"</dl>","\n");

    //data = useful_F::replaceAll(data,"    "," ");
    bool copy = true;
    std::string plainString = "";
    std::stringstream convertStream;

    // remove all xml tags
    for (unsigned int i=0; i < data.length(); i++)
    {
        convertStream << data[i];

        if(convertStream.str().compare("<") == 0) copy = false;
        else if(convertStream.str().compare(">") == 0)
        {
            copy = true;
            convertStream.str(std::string());
            continue;
        }

        if(copy) plainString.append(convertStream.str());

        convertStream.str(std::string());
    }

    return plainString;
}
