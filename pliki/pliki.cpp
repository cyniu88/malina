#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
struct movie_database
{
  bool is_file; // true - file  false - directory
  char path[150];
};

class movies_tree {
private:
char * database_path;
std::vector < movie_database > movie_database_vector;  
  size_t i ;    // licznik 
    struct dirent * plik;
    DIR * sciezka;
    movie_database temp;
public:
movies_tree(char* path);
void get_list( char* path );  // pobiera liste plikow i katalogow  w katalogu glóm
void get_main_list ();
void show_list ();
int get_vector_size ();
void list_tree( int i);
bool is_file(int i);
//next();
//back();
//exit();
void enter_dir(int i);
void vector_clear();
};
movies_tree::movies_tree ( char * path)
{
database_path = path;
get_list( database_path  );
i =0 ;
}
bool movies_tree::is_file( int i)
{
  return movie_database_vector[i].is_file;
}

void movies_tree::get_main_list()
{
get_list( database_path );
}
void movies_tree::list_tree( int i)
{	
if (!(get_vector_size() > i) ){
	
			return;
		}
	if (movie_database_vector[i].is_file == true ) {
		std::cout << "wypisuje sciezke pliku " << movie_database_vector[i].path << " jest to " << movie_database_vector[i].is_file << std::endl;
		 
		}
		else {
		   std::cout << "wypisuje sciezke katalogu " << movie_database_vector[i].path<< " jest to " << movie_database_vector[i].is_file << std::endl;
		}
		
}
int movies_tree::get_vector_size ()
{
		return  movie_database_vector.size();
}
void movies_tree::vector_clear ()
{        i =0;
		 movie_database_vector.clear();
}


void movies_tree::enter_dir(int i)
{
	if (movie_database_vector[i].is_file == true ) {
		std::cout << "to nie KATALOG!"<< std::endl;
		 
		}
		else {
		
			std::cout << "sciezka: " << movie_database_vector[i].path << " i i: " << i << std::endl;
		    get_list (movie_database_vector[i].path)  ;
			
		}

		}

void movies_tree::show_list(   ) {
   for(  size_t i = 0; i < movie_database_vector.size(); i++ )
   {
		if (movie_database_vector[i].is_file == true ) {
		   std::cout << "wypisuje sciezke pliku " << movie_database_vector[i].path << std::endl;
		 
		}
		else {
		   std::cout << "wypisuje sciezke katalogu " << movie_database_vector[i].path << std::endl;
		   
		}
	}
	 
}


void movies_tree::get_list( char * path  ) {

    vector_clear();    // czyscimy vector 

    if(( sciezka = opendir( path ) ) ) {
	
	
        while(( plik = readdir( sciezka ) ) )
            {
			// cout <<"nazwa:"<< plik->d_name <<  endl;
			// cout <<"typ: " << (int)plik->d_type << endl;
			 
			 char  sciezka_pliku[150] ;
			  strcpy(sciezka_pliku,path);
			  strcat (sciezka_pliku,"/");
			  strcat (sciezka_pliku,plik->d_name);
			 
			 
			 if (  (int)plik->d_type == 4 &&   strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." )  )
			 {
			// cout  << " to "<< sciezka_pliku <<  endl;
			 char  sciezka_katalogu[150] ;
			 strcpy(sciezka_katalogu, path);
			 strcat (sciezka_katalogu,"/");
			 strcat (sciezka_katalogu,plik->d_name);
			 cout<< "katalog: " << plik->d_name  << " \t\t" << sciezka_katalogu << endl;
			    temp.is_file=false;
				strcpy(temp.path,sciezka_katalogu);
				
			 }
			 else if ( strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." ))
			 {
			  // cout<< "plik: " << plik->d_name << " \t\t" << sciezka_pliku << endl;  
			    temp.is_file=true;
				//=sciezka_pliku;
				strcpy(temp.path,sciezka_pliku);
			 }
			 else
			 {continue;}
			   std::cout << "wpisuje w vector " << temp.path << " i jest to " << temp.is_file << std::endl;
				movie_database_vector.push_back(temp);
			 
			}  // end while
       i=0;
        closedir( sciezka );
    }
    else
         printf( "! wywoujc funkcj opendir(%s) pojawi si bd otwarcia strumienia dla danej cieki, moe nie istnieje, lub podano ciek pust\n",  path );
   
}

main( int argc, char ** argv ) {
char choise;
bool go_while=true;
movies_tree main_tree( argv[1]);
//main_tree.show_list();
while (go_while) {
if ( choise =='b'){
main_tree.get_main_list();
}
std::cout << " rozmiar vecotra : " << main_tree.get_vector_size() << std::endl;
for( size_t i = 0; i < main_tree.get_vector_size(); i++ )
   {
   main_tree.list_tree( i );
   std::cout << "iteracja: " << i << std::endl;
   std::cin >> choise;
   if (choise =='n')
		{
		continue;
		}
		else if ( choise =='t' &&  main_tree.is_file(i) == true)
		{
		  std::cout << " URUCHAMIAM PLIK! " << std::endl;
		  break;
		}
		else if ( choise =='t' &&  main_tree.is_file(i)== false) 
		{
			main_tree.enter_dir( i );
			std::cout << " rozmiar vecotra : " << main_tree.get_vector_size() << std::endl;
			continue;
		}
		 else if ( choise =='b')
		  {
			main_tree.vector_clear();	
		   break;
		  }
		   else if ( choise =='e')
		  {
			go_while=false;
		    break;
		  }
        } //end for
	}  // end while
return 0;
}
/* 
movie_database temp;
char choise;
bool go_while = true;
while (go_while)
{
   if( argc > 1 )
         listujPliki( argv[ 1 ],&movie_database_vector );
    else
         listujPliki( "." ,&movie_database_vector);
   
   std::cout << " teraz wypisze vektor " << movie_database_vector.size() << std::endl;
   
   for( size_t i = 0; i < movie_database_vector.size(); i++ )
   {
		if (movie_database_vector[i].is_file == true ) {
		std::cout << "wypisuje sciezke pliku " << movie_database_vector[i].path << std::endl;
		std::cout << "uruchomic? " << std::endl;
		std::cin >> choise;
		}
		else {
		   std::cout << "wypisuje sciezke katalogu " << movie_database_vector[i].path << std::endl;
		   std::cout << "wejsc? " << std::endl;
		std::cin >> choise;
		}
		
		if (choise =='n')
		{
		continue;
		}
		else if ( choise =='t' &&  movie_database_vector[i].is_file == true)
		{
		  std::cout << " URUCHAMIAM PLIK! " << std::endl;
		  break;
		}
		  else if ( choise =='t' &&  movie_database_vector[i].is_file == false) 
		  {
			listujPliki( movie_database_vector[i].path ,&movie_database_vector);
		  }
		  else if ( choise =='b')
		  {
			movie_database_vector.clear();	
		   break;
		  }
		   else if ( choise =='e')
		  {
			go_while=false;
		    break;
		  }
   } //end for
   
   } // end while
   return 0;
} */
