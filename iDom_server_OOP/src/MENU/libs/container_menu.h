#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H
#include <vector>
#include <string>
#include <functional>
#include <map>

namespace cyniu{

template <class T_NAME, class T_FUNC>
struct MenuStruct{
    T_NAME name;
    T_FUNC function;
};

//using DataBase = MenuStruct<std::string,std::function<void()>>;

template <class T_NAME, class T_FUNC>
class ContainerMenu
{ 
    std::vector<MenuStruct<T_NAME, T_FUNC>> database;
    int counter = 0;
public:
    ContainerMenu() = default;
    ~ContainerMenu() = default;
    auto size() {return database.size();}
    void pushBack(MenuStruct< T_NAME, T_FUNC> data){database.push_back(data);}
    void up(){
        if(++counter == size())
            counter = 0;
    }
    void down(){
        if(--counter < 0)
            counter = size()-1;
    }
    auto getCurrent(){
        return database.at(counter);
    }
};

template <class T_KEY, class T_VALUE>
class ContainerMenuMap
{
    typedef typename std::map<T_KEY, T_VALUE>::iterator iteratorDatabaseMap;
    iteratorDatabaseMap it;
public:
    std::map<T_KEY, T_VALUE> databaseMap;
    ContainerMenuMap() = default;
    ~ContainerMenuMap() = default;
    void insert(std::pair<T_KEY, T_VALUE> data){
        databaseMap.insert(data);
    }
    void begin(){
        it = databaseMap.begin();
    }
    void up(){
        if( ++it == databaseMap.end())
            it = databaseMap.begin();
    }
    void down(){
        if(it == databaseMap.begin())
        {
            it = databaseMap.end();
            --it;
        }
        else
            --it;
    }
    iteratorDatabaseMap& getCurrent(){
        return it;
    }

};

} // end namespace cyniu
#endif // UNORDERED_MAP_H
