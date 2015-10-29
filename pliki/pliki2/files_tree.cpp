#include "files_tree.h"


files_tree::files_tree ( char * path)
{
    database_path = path;
    get_list( database_path  );

}
bool files_tree::is_file( int i)
{
    return movie_database_vector[i].is_file;
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
{        i =0;
         movie_database_vector.clear();
}


void files_tree::enter_dir(int i)
{
    if (movie_database_vector[i].is_file == true ) {
        std::cout << "to nie KATALOG!"<< std::endl;

    }
    else {

        //std::cout << "sciezka: " << movie_database_vector[i].path << " i i: " << i << std::endl;
        std::cout << " WHODZE W TO!" << std::endl;
        get_list (movie_database_vector[i].path)  ;

    }

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


void files_tree::get_list( char * path  ) {

    vector_clear();    // czyscimy vector
    char g;

    char  v_path[150] ;
    std::cout << " SCIEZKA TO " << path << std::endl;
    std::cin >> g;

    if(sciezka = opendir( path ))  {


        while(( plik = readdir( sciezka ) ) )
        {
            // cout <<"nazwa:"<< plik->d_name <<  endl;
            // cout <<"typ: " << (int)plik->d_type << endl;

            if (  (int)plik->d_type == 4 &&   strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." )  )
            {

                temp.is_file=false;
              //  strcpy(temp.path,sciezka_katalogu);

            }
            else if ( (int)plik->d_type == 8 && strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." ))
            {
                temp.is_file=true;
                //=sciezka_pliku;
                //strcpy(temp.path,sciezka_pliku);
            }
            else
            {continue;}
            strcpy(v_path, path);
            strcat (v_path,"/");
            strcat (v_path,plik->d_name);
            std::cout << "wpisuje w strukture " << plik->d_name << std::endl;
            strcpy(temp.path,v_path);
            std::cout << "wpisuje w vector " << temp.path << " i jest to " << temp.is_file << std::endl;
            movie_database_vector.push_back(temp);

        }  // end while

        closedir( sciezka );
    }
    else
        printf( "! wywoujc funkcj opendir(%s) pojawi si bd otwarcia strumienia dla danej cieki, moe nie istnieje, lub podano ciek pust\n",  path );

}


