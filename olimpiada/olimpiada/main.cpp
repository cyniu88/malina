#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

void print(std::vector< long long>& _db)
{
    /*
    for(const auto& a : _db)
    {
      std::cout << " " << a << ", " ;
    }
    */
    std::cout  << "minimum: " << *min_element(_db.begin(), _db.end())<< std::endl;
    std::cout  << "maximum: " << *max_element(_db.begin(), _db.end())<< std::endl;
    std::cout  << "size : " << _db.size() << std::endl;

    std::cout << std::endl;
}


void multi(std::vector<long long>& _db)
{
    for(auto& a : _db)
    {
        if(a*2 < 100'000)
        {
         a = a*2;
        }
    }
}

void dev(std::vector<long long>& _db, std::vector<long long>& _dbTemp)
{
    for(const auto& a : _db)
    {
      //  std::cout << "cyniu: a "  << a << " --- "<< (a-1) % 3 << std::endl;
        if(a != 4 and a !=1 and (a-1) % 3 == 0)
        {
            auto w = (a-1)/3;
            if(w%2 !=0)
            {
               _dbTemp.push_back(w);

            }
        }
    }
}


int main()
{
    std::vector<long long> db = {1};
    std::vector<long long> dbTemp;
    const auto startTime = std::time(0);
    int counter = 100;

    std::cout << "iter " << 1 << std::endl;
    print(db);
    for(int i = 2 ; i< counter+2; ++i)
    {
        std::cout << "iter " << i << std::endl;
        dev(db, dbTemp);
        multi(db);
        // merge
        if(!dbTemp.empty())
            db.insert(db.end(), dbTemp.begin(), dbTemp.end());
        //cleaa
        dbTemp.clear();
        print(db);
    }

    std::cout << "czas trwania : " <<  std::time(0) - startTime  << " sek" <<std::endl;
    return 0;
}
