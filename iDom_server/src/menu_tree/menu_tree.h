#ifndef MENU_TREE_H
#define MENU_TREE_H

#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <algorithm>
#include <boost/regex.hpp>
#include "../LCD_c/lcd_c.h"
#include "../files_tree/files_tree.h"
 

class menu_tree {
private:
 LCD_c *mainLCD;
std::string database_path;
std::vector < movie_database > movie_database_vector;
  int i ;    // licznik
    struct dirent * plik;
    DIR * sciezka;
    movie_database temp;

    boost::regex w_serial;
    boost::smatch result;

std::stack <std::string> tree_stack;  // kolejka trzyma kolejke katalogow jakie odwiedzilismy  dzieki temu da sie wracac 
std::stack <int> i_stack;

public:
menu_tree(std::string path, LCD_c *mainLCD_PTR);
void get_list(std::string path );  // pobiera liste plikow i katalogow  w katalogu glóm
void get_main_list ();
std::string show_list ( );
int get_i(); 				//pobgiera i  ze stosu  do cofania sie po katalogach
std::string return_path (int i);
int get_vector_size ();
void next( );
void previous();
bool is_file( );

void back_dir();

void enter_dir(std::string path );
void enter_dir();
void vector_clear();
};

#endif // MENU_TREE_H