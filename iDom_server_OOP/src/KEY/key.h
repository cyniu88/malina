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
    ~KEY() ;
   std::string getName();
    virtual char getValue();

protected:
    char char_value;
   std::string key_name;

};




class SuperKEY : public KEY
{
    public:
    SuperKEY (char v, std::string n,std::string LogName) ;
    ~SuperKEY() ;

    char getValue();
    protected:

    std::string LogName;
};



class map_key
{
    std::map <std::string , std::unique_ptr <KEY>  > *key_map;
public:

    map_key(std::map <std::string , std::unique_ptr <KEY>  > *key_map) ;
    ~map_key( );

    void addToMap(std::unique_ptr <KEY> keyPtr);



};


 class pilot
 {   std::map <std::string , std::unique_ptr <KEY>  > *key_map;
    public:
     pilot(std::map <std::string , std::unique_ptr <KEY>  > *key_map) ;
    ~pilot( );
     void setup();
 };

#endif // KEY_H
