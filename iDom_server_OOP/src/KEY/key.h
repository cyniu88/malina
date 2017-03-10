#ifndef KEY_H
#define KEY_H
#include <iostream>
#include <map>
#include <utility>      // std::pair
#include <memory>

class KEY
{
public:
    KEY(char value, std::string name) ;
    virtual ~KEY() ;
    std::string getName() const;
    virtual  char getValue() const;

protected:
    char char_value;
    std::string key_name;
};

class SuperKEY : public KEY
{
public:
    SuperKEY (char v, std::string n,std::string LogName) ;
    virtual ~SuperKEY() ;
    char getValue() const;
protected:

    std::string LogName;
};

class pilot
{
    std::map <std::string , std::unique_ptr <KEY>  > *key_map;
public:
    pilot(std::map <std::string , std::unique_ptr <KEY>  > *key_map) ;
    virtual ~pilot( );
    void setup();
};

#endif // KEY_H
