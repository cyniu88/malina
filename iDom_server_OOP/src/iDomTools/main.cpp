
#include <iostream>
#include <sys/sysinfo.h>
//#include "idomtools.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "== Start test ===" << endl;
    double newValue = 9;
    double oldValue = 0;
    double reference= 0.002;
    while (newValue != 100)
    {
        cout << "podaj wartosc ";
        cin >> newValue;
        if (newValue >= reference && oldValue < reference){

            cout  << "przejscie w gore " << reference <<endl;
        }
        else if (newValue <= reference && oldValue > reference){
            cout << " przejscie w dol " << reference << endl;
        }
        else {
            cout << "bez zmian "<<endl;
        }

        oldValue = newValue;
    }

    struct sysinfo info;
    sysinfo(&info);
float shiftfloat=(float)(1<<SI_LOAD_SHIFT);


    std::cout <<"load average : 1min: "
              << (info.loads[0]*shiftfloat)
              <<" 5min: "
              <<(info.loads[1]*shiftfloat )
              << " 15min: "
              <<(info.loads[2]*shiftfloat)
              <<std::endl;

    return 0;
}
