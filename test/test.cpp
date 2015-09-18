#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>     
//    sprintf(command, "pgrep %s > /dev/null", name);
std::string ssystem (const char *command) {
    char tmpname [L_tmpnam];
    std::tmpnam ( tmpname );
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    std::system(cmd.c_str());
    std::ifstream file(tmpname, std::ios::in );
    std::string result;
        if (file) {
      while (!file.eof()) result.push_back(file.get());
          file.close();
    }
    remove(tmpname);
    return result;
}
int main(int argc, char *argv[])
{
        std::string bash = "free ";
    std::string in;
        std::string s = ssystem(bash.c_str());
    std::istringstream iss(s);
        std::string line;
        while ( std::getline(iss, line) )
        {
      std::cout << "LINE-> " + line + "  length: " << line.length() << std::endl;
        }
    std::cin >> in;
    return 0;
}
