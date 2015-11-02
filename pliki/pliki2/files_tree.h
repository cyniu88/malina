#ifndef FILES_TREE_H
#define FILES_TREE_H

#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
using namespace std;
struct movie_database
{
  bool is_file; // true - file  false - directory
 std::string path;
};

class files_tree {
private:
std::string database_path;
std::vector < movie_database > movie_database_vector;
  size_t i ;    // licznik
    struct dirent * plik;
    DIR * sciezka;
    movie_database temp;
	
std::stack <std::string> tree_stack;  // kolejka trzyma kolejke katalogow jakie odwiedzilismy  dzieki temu da sie wracac 


public:
files_tree(std::string path);
void get_list(std::string path );  // pobiera liste plikow i katalogow  w katalogu glóm
void get_main_list ();
void show_list ();
std::string return_path (int i);
int get_vector_size ();
void list_tree( int i);
bool is_file(int i);
//next();
std::string back_dir();
//exit();
void enter_dir(std::string path,int i);
void enter_dir(std::string path);
void vector_clear();
};

#endif // FILES_TREE_H
