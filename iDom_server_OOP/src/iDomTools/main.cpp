
#include <iostream>
//#include "idomtools.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "== Start test ===" << endl;
    int newValue = 0;
    int oldValue = 0;
    int reference= 0;
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

    return 0;
}
