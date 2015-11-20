#include <iostream>

#include <string>

#include <boost/regex.hpp> // je.li nie mamy kompilatora zgodnego z standardem C++0x, musimy sami zainstalowa. bibliotek. boost

using namespace boost; //narz.dzia biblioteki boost znajduj. si. w przestrzeni nazw boost, definiujemy by mie. mniej pisania

using namespace std;

int main(int argc, char *argv[]){

cout<<"Program regex znajdujemy maila"<<endl<<"________________"<<endl;

string tekst; //deklarujemy zmienn. typu sting

tekst="W moim Tekscie terazs032E543 tusk@donald.com jest wiele john@deer.pl adresow email kropka@dot.org. the.big.bang.theory_S09E33-DE.avi";
tekst=argv[1];
cout<<"nasz ciag"<<endl<<tekst<<endl<<"....-"<<endl; //wy.wietli warto.. zmiennej tekst

regex wzorzec( "\\w*[@]\\w*[.]\\w*" ); //czyli regu.a któb.dziemy wyszukiwa. [@] to pojedy.czy znak specjalny
regex wzorzec_serial ( "[Ss]\\d*[Ee]\\d*");

smatch wynik;//Tworzymy zmienn. typu smatch, któb.dzie przechowywa.a wyniki.
smatch wynik_serial;
// zasada u.ycia funkcji regex . regex_search(tekst_w_ktorym_wyszukujemy, zmienna_do_wyswietlenia_wyniku, regula_jakiej_wyszukujemy)

if( regex_search(tekst,wynik,wzorzec) ) //if s.u.y do tego by sprawdzi. czy co. znaleziono, bo jak nie to po co wy.wietla.?

{

//cout << "co znalazl: "<< wynik[0] << endl;

}


if( regex_search(tekst,wynik_serial,wzorzec_serial) ) //if s.u.y do tego by sprawdzi. czy co. znaleziono, bo jak nie to po co wy.wietla.?

{
for (int i =0 ; i < wynik_serial.size() ; ++i){
cout << "co znalazl serial: "<< wynik_serial[i] << endl;
}
}

return 0;

};
