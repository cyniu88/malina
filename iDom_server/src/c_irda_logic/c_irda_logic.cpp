#include "c_irda_logic.h"
#include "../iDom_server/src/iDom_server.h"


c_irda_logic::c_irda_logic(thread_data *my_data)
{
    //std::cout <<"SCIEZKA TO " << my_data->server_settings->MOVIES_DB_PATH << std::endl;
    my_data_logic = my_data;
    who='!';
    // pinMode(LED7, OUTPUT); // LED  na wyjscie  GPIO
    //digitalWrite(LED7,OFF);
}

void c_irda_logic::_add(char X)
{

    if (who=='!')
    {
        if (X!='M'&& X!='r'&& X!='E' && X!='s')
        {
            char_queue._add(X);
        }

        else if (X=='r')
        {
            who = 'r';
            char_queue._add('A'); // projektor wlaczony wiec pauzuje radio
            usleep(500);


        }
        else if (X=='s')
        {

            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"TEMPERATURA :)");
            std::string temp_str="";
            temp_str = send_to_arduino(my_data_logic,"temperature:2;");
            temp_str.erase(temp_str.size()-2,temp_str.size());
            my_data_logic->mainLCD->printString(false,0,1,"temp: "+temp_str+" c");
            who='!';


        }
        else if (X=='E')
        {
            who = 'E';
            my_data_logic->main_tree->show_list(); //printuje pierwszy element
            my_data_logic->mainLCD->set_print_song_state(100);
            //my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));
        }
        else {
            who = 'M';
            my_data_logic->main_MENU->show_list();
            my_data_logic->mainLCD->set_print_song_state(100);
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////// obsluga projektora
    else if (who=='r')
    {
        std::cout << "jestem w projektorze  " << std::endl;
        if ( X=='e')
        {
            who = '!';
            //   std::cout << "koniec sterowania  projektorem" << std::endl;
        }
        else if (X=='+')
        {
            system("echo -n + > /tmp/cmd");  // podglasniamy
        }
        else if (X=='-')
        {
            system("echo -n - > /tmp/cmd");  // wyciszamy
        }
        else if (X=='O')
        {
            system("echo -n p > /tmp/cmd");  //pauza play
        }
        else if (X=='P')
        {
            system("echo -n q > /tmp/cmd &");  // zamykanie omxplayera
        }
        else if (X=='D')
        {
            system("echo -n $'\x1b\x5b\x43' > /tmp/cmd");  // do przodu
        }
        else if (X=='U')
        {
            system("echo -n $'\x1b\x5b\x44' > /tmp/cmd");  // do tylu

        }
        else if (X=='^')
        {
            system("echo -n o > /tmp/cmd");  // do przodu
        }
        else if (X=='/')
        {
            system("echo -n i > /tmp/cmd");  // do tylu

        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////// przegladanie katalogow
    else if (who=='E')
    {

        if ( X=='e')
        {
            my_data_logic->mainLCD->set_print_song_state(0);
            my_data_logic->mainLCD->set_lcd_STATE(2);
            who = '!';  // koniec przegladania katalogow

        }
        else if (X=='+')
        {
            my_data_logic->main_tree->next();  // naspteny katalog
        }
        else if (X=='-')
        {
            my_data_logic->main_tree->previous(); //poprzedni katalog
        }
        else if (X=='O')
        {
            // whodze w katalog lub odtwarzma plik

            if (my_data_logic->main_tree->is_file() == false)
            {
                my_data_logic->main_tree->enter_dir();
                my_data_logic->main_tree->show_list();
               // my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));
            }
            else
            {
                std::cout << " URUCHAMIAM PLIK! " <<my_data_logic->main_tree->show_list() <<std::endl;


                std::string command = "/home/pi/programowanie/PYTON/iDom_movie.py ";
                command+=my_data_logic->main_tree->show_list();
                system(command.c_str());
                std::cout << " WYSTARTOWALEM!!";
                my_data_logic->mainLCD->set_lcd_STATE(-1);
                my_data_logic->mainLCD->printString(true,0,0,"odtwarzam film");
                my_data_logic->mainLCD->printString(false,0,1,my_data_logic->main_tree->show_list());
                //char_queue._add('A');
            }
        }
        else if (X=='U')
        {
            my_data_logic->main_tree->back_dir();

        }
        my_data_logic->main_tree->show_list();
       // my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));

    }
        /////////////////////////////////////////////////////////////////////////////////////////////  oblsuga menu

        else if (who=='M')
        {

        if ( X=='e')
        {
            my_data_logic->mainLCD->set_print_song_state(0);
            my_data_logic->mainLCD->set_lcd_STATE(2);
            who = '!';  // koniec przegladania katalogow

        }
        else if (X=='+')
        {
            my_data_logic->main_MENU->next();  // naspteny katalog
        }
        else if (X=='-')
        {
            my_data_logic->main_MENU->previous(); //poprzedni katalog
        }
        else if (X=='O')
        {
            // whodze w katalog lub odtwarzma plik

            if (my_data_logic->main_MENU->is_file() == false)
            {
                my_data_logic->main_MENU->enter_dir();
                my_data_logic->main_MENU->show_list();
               // my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));
            }
            else
            {
                std::cout << " URUCHAMIAM PLIK! " <<my_data_logic->main_MENU->show_list() <<std::endl;

            }
        }
        else if (X=='U')
        {
            my_data_logic->main_MENU->back_dir();

        }
        my_data_logic->main_MENU->show_list();
       // my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));


        }
    }


char c_irda_logic::_get( )
{  char temp;


    if (irda_queue.size() > 0){
        temp = irda_queue.front();

        irda_queue.pop();
    }
    else{

        return 'a';
    }
    return temp;
}

int c_irda_logic::_size()
{
    return irda_queue.size();
}
