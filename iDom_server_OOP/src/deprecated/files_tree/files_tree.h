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
//#include <boost/regex.hpp>
#include <regex>
#include <algorithm>
#include "../LCD_c/lcd_c.h"

struct movie_database
{
    bool is_file; // true - file false - directory
    std::string path;
    std::string files_name;
};


class files_tree {
private:
    LCD_c *mainLCD;
    std::string database_path;
    std::vector <movie_database> movie_database_vector;
    int i; // licznik
    struct dirent * plik;
    DIR * sciezka;
    movie_database temp;

    std::regex w_serial;
    std::smatch result;

    std::stack <std::string> tree_stack; // kolejka trzyma kolejke katalogow jakie odwiedzilismy dzieki temu da sie wracac
    std::stack <int> i_stack;

public:
    files_tree(const std::string& path, LCD_c *mainLCD_PTR);
    void get_list(std::string path );// pobiera liste plikow i katalogow w katalogu glóm
    //void get_main_list ();
    std::string show_list ( );
    int get_i(); 				//pobgiera i ze stosu do cofania sie po katalogach
    //std::string return_path (int i) const;
    int get_vector_size () const;
    void next( );
    void previous();
    bool is_file( ) const;

    void back_dir();

    void enter_dir(const std::string& path );
    void enter_dir();
    void vector_clear();
};

#endif // FILES_TREE_H
