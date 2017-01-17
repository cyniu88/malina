#include "c_irda_logic.h"
#include "../iDom_server_OOP.h"
#include   <unistd.h>


c_irda_logic::c_irda_logic(thread_data *my_data)
{
    my_data_logic = my_data;
    who='!';
}

void c_irda_logic::_add(char X)
{

    if (who=='!')
    {
        if (X!='M'&& X!='r'&& X!='E' && X!='s' && X!='c' && X!='S' && X!='L')
        {
            char_queue._add(X);
        }

        else if (X=='r')
        {
            who = 'r';
        }
        else if (X=='S')
        {
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"GASZE LEDy");
            std::string temp_str="";
            temp_str.erase();
            temp_str += my_data_logic->main_iDomTools->ledOFF();
            my_data_logic->mainLCD->printString(false,0,1,temp_str);

            who='!';
        }
        else if (X=='L')
        {
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"ZAPALAM LEDy");
            std::string temp_str="";
            temp_str += my_data_logic->main_iDomTools->ledOn(my_data_logic->ptr_pilot_led->colorLED[my_data_logic->ptr_pilot_led->counter]);

            if (++my_data_logic->ptr_pilot_led->counter >  my_data_logic->ptr_pilot_led->colorLED.size()-1 )
            {
                my_data_logic->ptr_pilot_led->counter=0;
            }

            my_data_logic->mainLCD->printString(false,0,1,temp_str);
            who='!';
        }
        else if (X=='s')
        {
            CRON temp_cron(my_data_logic);
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"SMOG: "+temp_cron.smog()+" mg/m^3");
            std::string temp_str = "I:";
            temp_str += my_data_logic->main_iDomTools->getTemperatureString();// send_to_arduino(my_data_logic,"temperature:2;");

            temp_str.insert(temp_str.find_last_of(':')," O:");

            temp_str.erase(temp_str.find_last_of(':'),1);
            temp_str.erase(temp_str.size()-2,temp_str.size());
            my_data_logic->mainLCD->printString(false,0,1,temp_str+" c");
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
    //////////////////////////////////////  automatyczne wylaczanie muzyki /////////////////////

    else if (who=='c')
    { my_data_logic->mainLCD->set_print_song_state(100);

        my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        if ( X=='e')
        {
            my_data_logic->sleeper=0;
            my_data_logic->mainLCD->set_print_song_state(0);
            who = '!';
            //   std::cout << "koniec sterowania  projektorem" << std::endl;
        }
        else if (X=='U')
        {
            ++my_data_logic->sleeper;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X=='D')
        {
            --my_data_logic->sleeper;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X=='^')
        {
            my_data_logic->sleeper+=10;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X=='/')
        {
            my_data_logic->sleeper-=10;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X=='O')
        {
            for (int con_counter=0; con_counter< MAX_CONNECTION; ++con_counter)
            {
                if (   my_data_logic->main_THREAD_arr[con_counter].thread.joinable() == false )   // jesli pozycja jest wolna (0)  to wstaw tam  jesli jest zjęta wyslij sygnal i sprawdz czy waŧek żyje ///

                {
                    if ( con_counter!=MAX_CONNECTION -1)
                    {
                        my_data_logic->main_THREAD_arr[con_counter].thread = std::thread(sleeper_mpd,my_data_logic);
                        my_data_logic->main_THREAD_arr[con_counter].thread_name="Sleeper  MPD ";
                        my_data_logic->main_THREAD_arr[con_counter].thread_ID = my_data_logic->main_THREAD_arr[con_counter].thread.get_id();
                        my_data_logic->main_THREAD_arr[con_counter].thread.detach();
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO << "watek SLEEPER_MPD wystartowal  "<< my_data_logic->main_THREAD_arr[con_counter].thread_ID << std::endl;
                        log_file_mutex.mutex_unlock();
                        // my_data_logic->sleeper=0;
                        my_data_logic->mainLCD->printString(true,1,0,"SLEEPer START");
                        my_data_logic->mainLCD->set_print_song_state(0);
                        who = '!';
                        break;
                    }
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////// obsluga projektora
    else if (who=='r')
    {
        //std::cout << "jestem w projektorze  " << std::endl;
        my_data_logic->mainLCD->set_print_song_state(100);
        my_data_logic->mainLCD->printString(false,2,1,"  PROJEKTOR   ");
        if ( X=='e')
        {   my_data_logic->mainLCD->set_print_song_state(0);
            who = '!';
            if(my_data_logic->ptr_MPD_info->isPlay == true){
                char_queue._add('t');
            }
            else {
                my_data_logic->main_iDomTools->turnOffSpeakers();
                my_data_logic->mainLCD->set_print_song_state(0); // off display
                my_data_logic->mainLCD->set_lcd_STATE(0);
            }
            //   std::cout << "koniec sterowania  projektorem" << std::endl;
        }

        else if (X=='+')
        {
            //system("echo -n + > /mnt/ramdisk/cmd");  // podglasniamy
            write_to_mkfifo("+");
        }
        else if (X=='-')
        {
            //system("echo -n - > /mnt/ramdisk/cmd");  // wyciszamy
            write_to_mkfifo("-");
        }
        else if (X=='O')
        {
            my_data_logic->mainLCD->set_print_song_state(1000);
            my_data_logic->mainLCD->printString(false,0,0,"ODTWARZAM VIDEO");
            //system("echo -n p > /mnt/ramdisk/cmd");  //pauza play
            write_to_mkfifo("p");
        }
        else if (X=='P')
        {
            //system("echo -n q > /mnt/ramdisk/cmd &");  // zamykanie omxplayera
            write_to_mkfifo("q");
        }
        else if (X=='D')
        {
            system("echo -n $'\x1b\x5b\x43' > /mnt/ramdisk/cmd");  // do przodu
            //write_to_mkfifo("$'\x1b\x5b\x43'");
        }
        else if (X=='U')
        {
            system("echo -n $'\x1b\x5b\x44' > /mnt/ramdisk/cmd");  // do tylu
            //   write_to_mkfifo("$'\x1b\x5b\x44'");
        }
        else if (X=='^')
        {
            //system("echo -n o > /mnt/ramdisk/cmd");  // do przodu
            write_to_mkfifo("o");
        }
        else if (X=='/')
        {
            //system("echo -n i > /mnt/ramdisk/cmd");  // do tylu
            write_to_mkfifo("i");

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
                who='r';
                //char_queue._add('A');
                log_file_mutex.mutex_lock();
                log_file_cout << INFO << "odtwarzam film "<< my_data_logic->ptr_MPD_info->isPlay << std::endl;
                log_file_mutex.mutex_unlock();
                if (my_data_logic->ptr_MPD_info->isPlay == true){
                    char_queue._add('A'); // projektor wlaczony wiec pauzuje radio
                }
                else{
                    my_data_logic->main_iDomTools->turnOnSpeakers();
                    puts("wlaczam glosnik do filmu");
                }
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
            }
            else
            {
                // menu start
                if (my_data_logic->main_MENU->show_list() == "5.SLEEPer" ){
                    std::cout << " POBUDKA!!!!" << std::endl;
                    who='c';
                }
                else if (my_data_logic->main_MENU->show_list() == "2.TEMPERATURA" ){
                    std::cout << " temperatura !!!!" << std::endl;
                    who='!';
                    _add('s');
                }
                else if (my_data_logic->main_MENU->show_list() == "4.PLIKI" ){
                    std::cout << " do filmow" << std::endl;
                    who='!';
                    _add('E');
                    _add('+');
                }
            }
        }
        else if (X=='U')
        {
            my_data_logic->main_MENU->back_dir();
        }
        my_data_logic->main_MENU->show_list();
    }
}


char c_irda_logic::_get( )
{
    char temp;
    if (irda_queue.empty() == false){
        temp = irda_queue.front();
        irda_queue.pop();
    }
    else{
        return 'a';
    }
    return temp;
}

int c_irda_logic::_size() const
{
    return irda_queue.size();
}
