#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H
#include <vector>
#include <string>
#include <functional>

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
    std::vector<MenuStruct< T_NAME, T_FUNC>> dataBase;
    int counter = 0;
public:
    ContainerMenu() = default;
    ~ContainerMenu() = default;
    auto size() {return dataBase.size();}
    void pushBack(MenuStruct< T_NAME, T_FUNC> data){dataBase.push_back(data);}
    void up(){
        if(++counter == size())
            counter = 0;
    }
    void down(){
        if(--counter < 0)
            counter = size()-1;
    }
    auto getCurrent(){
        return dataBase.at(counter);
    }
};

} // end namespace cyniu
#endif // UNORDERED_MAP_H
