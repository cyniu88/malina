#include "menu_tree.h"

menu_tree::menu_tree (const std::string &path, LCD_c *mainLCD_PTR):database_path(path),w_serial( "([Ss]\\d{1,3}[Ee]\\d{1,3})")
{
    mainLCD=mainLCD_PTR;

    i_stack.push(0);
    i=0;
    get_list( database_path  );
}

bool menu_tree::is_file(  ) const
{
    return movie_database_vector[i].is_file;
}
//std::string menu_tree::return_path( int i) const
//{

//    return movie_database_vector[i].path;
//}

//void menu_tree::get_main_list()
//{
//    get_list( database_path );
//}
void menu_tree::next(  )
{   
    ++i;
    if (get_vector_size() == i ){
        i=0;
    }
}

void menu_tree::previous(  )
{
    --i;
    if ( i <0 )
    {
        i=get_vector_size()-1;
    }
}

int menu_tree::get_vector_size () const
{
    return  movie_database_vector.size();
}
void menu_tree::vector_clear ()
{       
    movie_database_vector.clear();
}
int menu_tree::get_i()
{
    if (i_stack.size() >1 ) {
        int i = i_stack.top();
        i_stack.pop();
        return i;
    }
    else
        return 0;
}

void menu_tree::enter_dir()
{
    i_stack.push(i);  // wpisuje na stos  kolejna wersje licznika i
    
    if (movie_database_vector[i].is_file == true ) {

    }
    else {
        get_list (movie_database_vector[i].path)  ;
        i=0;
    }
}
void menu_tree::enter_dir(const std::string& path)
{
    get_list (path)  ;
}

void menu_tree::back_dir()
{
    if ( tree_stack.size() >1 ) {
        tree_stack.pop();
        std::string path = tree_stack.top();
        tree_stack.pop();
        i=get_i();
        get_list (path)  ;
        return;
    }
    i=get_i();
    get_list (  database_path);
    return;
}

std::string menu_tree::show_list(     )
{
    if (movie_database_vector[i].is_file == true ) {
        mainLCD->printString(true,0,0,movie_database_vector[i].files_name.substr(0,16));

        return movie_database_vector[i].files_name;
    }
    else {
        mainLCD->printString(true,0,0,movie_database_vector[i].files_name+" ->");
    }

    return movie_database_vector[i].path;
}
bool comper (const movie_database & a , const movie_database& b);
//{
//    return a.files_name < b.files_name ;
//}

void menu_tree::get_list(const std::string& path  ) {
    tree_stack.push(path);
    vector_clear();    // czyscimy vector
    std::string  path2 =path;
    std::string v_path ,tmp_string ;
    if(sciezka = opendir( path.c_str() ))  {


        while(( plik = readdir( sciezka ) ) )
        {
            path2 =path;
            if (  static_cast<int>(plik->d_type) == 4 /*&&   strcmp( plik->d_name, ".." ) && strcmp( plik->d_name, "." )*/  )
            {
                if (!strcmp( plik->d_name, ".." ) || !strcmp( plik->d_name, "." ))
                {continue;}
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
            temp.files_name.assign(plik->d_name);
            movie_database_vector.push_back(temp);

        }  // end while
        sort(movie_database_vector.begin(),movie_database_vector.end(), comper);
        closedir( sciezka );
    }
}
