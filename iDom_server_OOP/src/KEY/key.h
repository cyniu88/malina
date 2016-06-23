#ifndef KEY_H
#define KEY_H
#include <iostream>
#include <map>
#include <utility>      // std::pair
#include <memory>

class KEY
{
public:
    KEY(std::string name) ;
    ~KEY() ;
   std::string getName();

private:
   std::string key_name;
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
