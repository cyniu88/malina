#include <iostream>

#include <string>

#include <boost/regex.hpp> // jeśli nie mamy kompilatora zgodnego z standardem C++0x, musimy sami zainstalować bibliotekę boost

using namespace boost; //narzędzia biblioteki boost znajdują się w przestrzeni nazw boost, definiujemy by mieć mniej pisania

using namespace std;

int main(){

//cout<<„Program regex znajdujemy maila”<<endl<<„——–„<<endl;

string tekst; //deklarujemy zmienną typu sting

tekst="W moim Tekscie ter  [ RADIO MAX] az tusk@donald.com jest wiele john@deer.pl adresow email kropka@dot.org";

cout<<"nasz ciag"<<endl<<tekst<<endl<<"———-"<<endl; //wyświetli wartość zmiennej tekst

regex wzorzec( "\\[(.*?)\\]" ); //czyli reguła którą będziemy wyszukiwać [@] to pojedyńczy znak specjalny

smatch wynik;//Tworzymy zmienną typu smatch, która będzie przechowywała wyniki.

// zasada użycia funkcji regex – regex_search(tekst_w_ktorym_wyszukujemy, zmienna_do_wyswietlenia_wyniku, regula_jakiej_wyszukujemy)

if( regex_search(tekst,wynik,wzorzec) ) //if służy do tego by sprawdzić czy coś znaleziono, bo jak nie to po co wyświetlać?

{

cout << "co znalazl: " << wynik[1] << endl;

}

return 0;

}
