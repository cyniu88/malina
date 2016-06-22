#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include "files_tree.h"

int main( int argc, char ** argv ) {
    char choise;
    bool go_while=true;
	std::string s(argv[1]);
    files_tree main_tree( s);
    //main_tree.show_list();
   
    while (go_while) {
     
          std::cout<< main_tree.show_list()<<std::endl;
            std::cout << " wybierz " << std::endl;
            std::cin >> choise;
            if (choise =='n')
            {    
				main_tree.next(  );
		      
            }
			else if ( choise =='p' )
            {   
				main_tree.previous( );
			 
            }
            else if ( choise =='t' )
            {
                std::cout << " URUCHAMIAM PLIK! " << std::endl;
				if (main_tree.is_file() == false)
				{
				 main_tree.enter_dir();
				}
				else
				{
					std::cout << " URUCHAMIAM PLIK! " <<main_tree.show_list() <<std::endl;
					std::string command("cat ");
					command+=main_tree.show_list();
					std::cout << "\n komenda to "<< command << "\n a wynik jej to: "<< system(command.c_str()) << std::endl;
					
				}
                continue;
            }
            
		 
            else if ( choise =='u')
            {
                	main_tree.back_dir();	 
                continue;
            }
            else if ( choise =='e')
            {
                go_while=false;
                break;
            }
       // } //end for
    }  // end while
    return 0;
}
