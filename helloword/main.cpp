#include <iostream>
#include <dirent.h>
#include <cstdio>
#include <cerrno>
#include <string>
#include <set>
#include <iomanip>
using namespace std;

int pokazErrno();

set < string > listujPlikiPosortowane( const char * nazwa_sciezki, unsigned ilosc_w_linii = 4, unsigned rozmiar_bufora = 30, bool zlicznie = true, ostream & strumien = cout ) { // (1)
    set < string > nazwy_plikow;
   
    struct dirent * plik;
    DIR * sciezka;
   
    if(( sciezka = opendir( nazwa_sciezki ) ) ) {
        pokazErrno();
       
        while(( plik = readdir( sciezka ) ) )
             nazwy_plikow.insert( plik->d_name );
       
        pokazErrno();
       
        closedir( sciezka );
        pokazErrno();
       
        unsigned licznik = 0;
        if( zlicznie )
        for( set < string >::iterator it = nazwy_plikow.begin(); it != nazwy_plikow.end(); ++it )
             strumien << ++licznik << ")\t" << setw( rozmiar_bufora ) << left << * it << '\t' <<(( !( licznik % ilosc_w_linii - 3 ) ) ? "\n"
            : "" );
        else
        for( set < string >::iterator it = nazwy_plikow.begin(); it != nazwy_plikow.end(); ++it )
             strumien << setw( rozmiar_bufora ) << left << * it << '\t' <<(( !( licznik % ilosc_w_linii - ilosc_w_linii + 1 ) ) ? "\n"
            : "" );
       
        strumien << endl;
    }
    else
         pokazErrno();
   
    return nazwy_plikow;
}

int pokazErrno() {
    int errno_kopia = errno;
    if( errno ) {
        printf( "errno = %d:\t", errno ); fflush( stdout );
        switch( errno ) {
        case EACCES: perror( "! (problem w OPENDIR) brak dostepu do czytanej sciezki" ); break;
        case ELOOP: perror( "! (problem w OPENDIR) zbyt wiele symbolicznych powiązań napotkanych w ścieżce" ); break;
        case ENAMETOOLONG: perror( "! (problem w OPENDIR) długość ścieżki przekracza PATH_MAX, lub długość nazw jej komponentów przekracza NAME_MAX" ); break;
        case ENOENT: perror( "! (problem w OPENDIR) ścieżka jest pustym stringiem, lub (problem w OPENDIR lub READDIR) składnik ścieżki nie jest nazwą istniejącej ścieżki\n" ); break;
        case ENOTDIR: perror( "! (problem w OPENDIR) komponent lub nazwa ścieżki nie jest ścieżką" ); break;
        case EMFILE: perror( "! (problem w OPENDIR) jest otwarta liczba plików równa OPEN_MAX" ); break;
        case ENFILE: perror( "! (problem w OPENDIR) zbyt wiele plików jest obecnie otwartych przez system" ); break;
        case EOVERFLOW: perror( "! (problem w READDIR) jedna z wartości w strukturze nie może być zwrócona poprawnie" ); break;
        case EBADF: perror( "! (problem w READDIR lub CLOSEDIR) argument funkcji readdir nie odnosi się do otwartego strumienia ścieżki" ); break;
        case EINTR: perror( "! (problem w CLOSEDIR) pojawia się wtedy gry funkcja jest przerwana przez sygnał" ); break;
        };
        errno = 0;
        printf( "\n" );
    }
    return errno_kopia;
}

main( int argc, char ** argv ) {
    ios_base::sync_with_stdio( 0 ); // (2)
    if( argc > 1 )
         listujPlikiPosortowane( argv[ 1 ] );
    else
         listujPlikiPosortowane( "." );
   
    cout << "\nj.w. ale w linii, bez liczenia:\n";
    listujPlikiPosortowane( ".", 1, 50, false );
}
