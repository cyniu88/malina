#include "files_tree.h"


files_tree::files_tree ( std::string path)
{
    database_path = path;
	tree_stack.push(database_path);
    get_list( database_path  );

}
bool files_tree::is_file( int i)
{
    return movie_database_vector[i].is_file;
}
std::string files_tree::return_path( int i)
{
    std::cout << " return " << movie_database_vector[i].path <<std::endl;
    return movie_database_vector[i].path;
}

void files_tree::get_main_list()
{
    get_list( database_path );
}
void files_tree::list_tree( int i)
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
int files_tree::get_vector_size ()
{
    return  movie_database_vector.size();
}
void files_tree::vector_clear ()
{       
         movie_database_vector.clear();
}


void files_tree::enter_dir(std::string path,int i)
{

 
    if (movie_database_vector[i].is_file == true ) {
        std::cout << "to nie KATALOG!"<< std::endl;

    }
    else {

        std::cout << "sciezka: " << movie_database_vector[i].path << " i i: " << i << std::endl;
        std::cout << " WHODZE W TO!" << std::endl;
		tree_stack.push(path);
		 std::cout << " w kolejce jest elementow : "<< tree_stack.size()<< std::endl;
		 std::cout << " a ostatni to : "<< tree_stack.top()<< std::endl;
        get_list (path)  ;

    }

}
void files_tree::enter_dir(std::string path)
{	tree_stack.push(path);
      get_list (path)  ;
}

std::string files_tree::back_dir()
{
		if ( tree_stack.size() >1 ) {
		tree_stack.pop();
		std::string path = tree_stack.top();
		tree_stack.pop();
        std::cout << "                                           robie back do folderu: " << path << std::endl;
		 std::cout << " w kolejce jest TERAZ elementow : "<< tree_stack.size()<< std::endl;
        return path;
		}
    
     return database_path;
}

void files_tree::show_list(   ) {
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


void files_tree::get_list( std::string path  ) {
  
    vector_clear();    // czyscimy vector
    char g;
   std::string  path2 =path;
   std::string v_path ,tmp_string ;
    std::cout << " SCIEZKA TO " << path << std::endl;
    //std::cin >> g;

    if(sciezka = opendir( path.c_str() ))  {


        while(( plik = readdir( sciezka ) ) )
        {
				path2 =path;
         
			//	std::cout << " SCIEZKA TO " << path << std::endl;
            if (  static_cast<int>(plik->d_type) == 4 /*&&   strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." )*/  )
            {

                temp.is_file=false;
             

            }
           else //if ( (int)plik->d_type == 8 && strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." ))
            {
               temp.is_file=true;
                 
           }
          
		    v_path= path2;
			
			v_path+="/";
		
			tmp_string.assign(plik->d_name);
		    v_path+=tmp_string;
		   temp.path =v_path;
		   
           // std::cout << "wpisuje w strukture " << plik->d_name << std::endl;
           
			//std::cout << "\n";
			 
			
           // std::cout << "wpisuje w vector " << temp.path << " i jest to " << temp.is_file << std::endl;
            movie_database_vector.push_back(temp);

        }  // end while

        closedir( sciezka );
    }
    else
        std::cout << " wywoujc funkcj opendir(%s) pojawi si bd otwarcia strumienia dla danej cieki, moe nie istnieje, lub podano ciek pust\n"<<  path << std::endl;

}

