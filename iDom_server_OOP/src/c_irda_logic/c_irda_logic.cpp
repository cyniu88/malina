#include "c_irda_logic.h"
#include "../iDom_server_OOP.h"
#include   <unistd.h>


void c_irda_logic::irdaMPD(PILOT_KEY X)
{
    switch (X){
    case PILOT_KEY::KEY_POWER:
        mpd_queue._add(MPD_COMMAND::STOP);
        break;
    case PILOT_KEY::KEY_TV:
        mpd_queue._add(MPD_COMMAND::PLAY);
        break;
    case PILOT_KEY::KEY_VOLUMEDOWN:
        mpd_queue._add(MPD_COMMAND::VOLDOWN);
        break;
    case PILOT_KEY::KEY_VOLUMEUP:
        mpd_queue._add(MPD_COMMAND::VOLUP);
        break;
    case PILOT_KEY::KEY_AUDIO:
        mpd_queue._add(MPD_COMMAND::PAUSE);
        break;
    case PILOT_KEY::KEY_UP:
        mpd_queue._add(MPD_COMMAND::NEXT);
        break;
    case PILOT_KEY::KEY_DOWN:
        mpd_queue._add(MPD_COMMAND::PREV);
        break;
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "nieznana komenda MPD z pilota "<< std::endl;
        log_file_mutex.mutex_unlock();
    }
}

c_irda_logic::c_irda_logic(thread_data *my_data)
{
    my_data_logic = my_data;
    who=PILOT_STATE::MPD;
}

void c_irda_logic::_add(PILOT_KEY X)
{

    if (who==PILOT_STATE::MPD)
    {
        if (X!=PILOT_KEY::KEY_MENU && X!=PILOT_KEY::KEY_RADIO && X!=PILOT_KEY::KEY_EPG
                && X!=PILOT_KEY::KEY_SAT && X!=PILOT_KEY::SLEEPER && X!=PILOT_KEY::KEY_SUBTITLE
                && X!=PILOT_KEY::KEY_LANGUAGE)
        {
          irdaMPD(X);
        }

        else if (X == PILOT_KEY::KEY_RADIO)
        {
            who = PILOT_STATE::PROJECTOR;
            _add(PILOT_KEY::DUMMY);
        }
        else if (X == PILOT_KEY::KEY_SUBTITLE)
        {
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"GASZE LEDy");
            std::string temp_str="";
            temp_str.erase();
            temp_str += my_data_logic->main_iDomTools->ledOFF();
            my_data_logic->mainLCD->printString(false,0,1,temp_str);

            who=PILOT_STATE::MPD;
        }
        else if (X==PILOT_KEY::KEY_LANGUAGE)
        {
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"ZAPALAM LEDy");
            std::string temp_str = my_data_logic->ptr_pilot_led->colorLED[my_data_logic->ptr_pilot_led->counter].getColorName();
            my_data_logic->main_iDomTools->ledOn(my_data_logic->ptr_pilot_led->colorLED[my_data_logic->ptr_pilot_led->counter]);

            if (++my_data_logic->ptr_pilot_led->counter >  my_data_logic->ptr_pilot_led->colorLED.size()-1 )
            {
                my_data_logic->ptr_pilot_led->counter=0;
            }

            my_data_logic->mainLCD->printString(false,0,1,temp_str);
            who=PILOT_STATE::MPD;
        }
        else if (X== PILOT_KEY::KEY_SAT)
        {
            my_data_logic->mainLCD->set_lcd_STATE(10);
            my_data_logic->mainLCD->printString(true,0,0,"SMOG: "+my_data_logic->main_iDomTools->getSmog()+" mg/m^3");
            std::string temp_str = "I:";
            temp_str += my_data_logic->main_iDomTools->getTemperatureString();// send_to_arduino(my_data_logic,"temperature:2;");

            temp_str.insert(temp_str.find_last_of(':')," O:");

            temp_str.erase(temp_str.find_last_of(':'),1);
            temp_str.erase(temp_str.size()-2,temp_str.size());
            my_data_logic->mainLCD->printString(false,0,1,temp_str+" c");
            who=PILOT_STATE::MPD;
        }
        else if (X==PILOT_KEY::KEY_EPG)
        {
            who = PILOT_STATE::MOVIE;
            my_data_logic->main_tree->show_list(); //printuje pierwszy element
            my_data_logic->mainLCD->set_print_song_state(100);
            //my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));
        }
        else {
            who = PILOT_STATE::MENU;
            my_data_logic->main_MENU->show_list();
            my_data_logic->mainLCD->set_print_song_state(100);
        }
    }
    //////////////////////////////////////  automatyczne wylaczanie muzyki /////////////////////

    else if (who==PILOT_STATE::SLEEPER)
    { my_data_logic->mainLCD->set_print_song_state(100);

        my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        if ( X==PILOT_KEY::KEY_EXIT)
        {
            my_data_logic->sleeper=0;
            my_data_logic->mainLCD->set_print_song_state(0);
            who = PILOT_STATE::MPD;
            //   std::cout << "koniec sterowania  projektorem" << std::endl;
        }
        else if (X==PILOT_KEY::KEY_UP)
        {
            ++my_data_logic->sleeper;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X==PILOT_KEY::KEY_DOWN)
        {
            --my_data_logic->sleeper;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X==PILOT_KEY::KEY_CHANNELUP)
        {
            my_data_logic->sleeper+=10;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X==PILOT_KEY::KEY_CHANNELDOWN)
        {
            my_data_logic->sleeper-=10;
            my_data_logic->mainLCD->printString(true,0,0,std::to_string(my_data_logic->sleeper)+" minut");
        }
        else if (X==PILOT_KEY::KEY_OK)
        {
            for (int con_counter=0; con_counter< iDomConst::MAX_CONNECTION; ++con_counter)
            {
                if (   my_data_logic->main_THREAD_arr[con_counter].thread_socket == 0 )   // jesli pozycja jest wolna (0)  to wstaw tam  jesli jest zjęta wyslij sygnal i sprawdz czy waŧek żyje ///

                {
                    if ( con_counter!=iDomConst::MAX_CONNECTION -1)
                    {
                        my_data_logic->main_THREAD_arr[con_counter].thread = std::thread(useful_F::sleeper_mpd,my_data_logic);
                        my_data_logic->main_THREAD_arr[con_counter].thread_name="Sleeper  MPD ";
                        my_data_logic->main_THREAD_arr[con_counter].thread_ID = my_data_logic->main_THREAD_arr[con_counter].thread.get_id();
                        my_data_logic->main_THREAD_arr[con_counter].thread.detach();
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO << "watek SLEEPER_MPD wystartowal  "<< my_data_logic->main_THREAD_arr[con_counter].thread_ID << std::endl;
                        log_file_mutex.mutex_unlock();
                        // my_data_logic->sleeper=0;
                        my_data_logic->mainLCD->printString(true,1,0,"SLEEPer START");
                        my_data_logic->mainLCD->set_print_song_state(0);
                        who = PILOT_STATE::MPD;
                        break;
                    }
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////// obsluga projektora
    else if (who==PILOT_STATE::PROJECTOR)
    {
        my_data_logic->mainLCD->set_print_song_state(100);
        my_data_logic->mainLCD->printString(false,2,1,"  PROJEKTOR   ");

        if ( X==PILOT_KEY::KEY_EXIT)
        {   my_data_logic->mainLCD->set_print_song_state(0);
            who = PILOT_STATE::MPD;
            if(my_data_logic->ptr_MPD_info->isPlay == true){
                iDomTOOLS::MPD_play();
            }
            else {
                iDomTOOLS::turnOffSpeakers();
                my_data_logic->mainLCD->set_print_song_state(0); // off display
                my_data_logic->mainLCD->set_lcd_STATE(0);
            }
        }

        else if (X==PILOT_KEY::DUMMY){

        }
        else if (X==PILOT_KEY::KEY_VOLUMEUP)
        {
            useful_F::write_to_mkfifo("+");
        }
        else if (X==PILOT_KEY::KEY_VOLUMEDOWN)
        {
            useful_F::write_to_mkfifo("-");
        }
        else if (X==PILOT_KEY::KEY_OK)
        {
            my_data_logic->mainLCD->set_print_song_state(1000);
            my_data_logic->mainLCD->printString(false,0,0,"ODTWARZAM VIDEO");
            useful_F::write_to_mkfifo("p");
        }
        else if (X==PILOT_KEY::KEY_POWER)
        {
            //system("echo -n q > /mnt/ramdisk/cmd &");  // zamykanie omxplayera
            useful_F::write_to_mkfifo("q");
        }
        else if (X==PILOT_KEY::KEY_DOWN)
        {
            system("echo -n $'\x1b\x5b\x43' > /mnt/ramdisk/cmd");  // do przodu
            //write_to_mkfifo("$'\x1b\x5b\x43'");
        }
        else if (X==PILOT_KEY::KEY_UP)
        {
            system("echo -n $'\x1b\x5b\x44' > /mnt/ramdisk/cmd");  // do tylu
            //   write_to_mkfifo("$'\x1b\x5b\x44'");
        }
        else if (X==PILOT_KEY::KEY_CHANNELUP)
        {
            //system("echo -n o > /mnt/ramdisk/cmd");  // do przodu
            useful_F::write_to_mkfifo("o");
        }
        else if (X==PILOT_KEY::KEY_CHANNELDOWN)
        {
            //system("echo -n i > /mnt/ramdisk/cmd");  // do tylu
            useful_F::write_to_mkfifo("i");

        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////// przegladanie katalogow
    else if (who==PILOT_STATE::MOVIE)
    {
        if ( X==PILOT_KEY::KEY_EXIT)
        {
            my_data_logic->mainLCD->set_print_song_state(0);
            my_data_logic->mainLCD->set_lcd_STATE(2);
            who = PILOT_STATE::MPD;  // koniec przegladania katalogow

        }
        else if (X==PILOT_KEY::KEY_VOLUMEUP)
        {
            my_data_logic->main_tree->next();  // naspteny katalog
        }
        else if (X==PILOT_KEY::KEY_VOLUMEDOWN)
        {
            my_data_logic->main_tree->previous(); //poprzedni katalog
        }
        else if (X == PILOT_KEY::KEY_OK)
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
                who=PILOT_STATE::PROJECTOR;
                log_file_mutex.mutex_lock();
                log_file_cout << INFO << "odtwarzam film "<< my_data_logic->ptr_MPD_info->isPlay << std::endl;
                log_file_mutex.mutex_unlock();
                if (my_data_logic->ptr_MPD_info->isPlay == true){
                    iDomTOOLS::MPD_pause(); // projektor wlaczony wiec pauzuje radio
                }
                else{
                    iDomTOOLS::turnOnSpeakers();
                    puts("wlaczam glosnik do filmu");
                }
            }
        }
        else if (X == PILOT_KEY::KEY_UP)
        {
            my_data_logic->main_tree->back_dir();
        }
        my_data_logic->main_tree->show_list();
        // my_data_logic->mainLCD->printString(0,0,my_data_logic->main_tree->show_list().substr(16));

    }
    /////////////////////////////////////////////////////////////////////////////////////////////  oblsuga menu

    else if (who == PILOT_STATE::MENU)
    {

        if ( X == PILOT_KEY::KEY_EXIT)
        {
            my_data_logic->mainLCD->set_print_song_state(0);
            my_data_logic->mainLCD->set_lcd_STATE(2);
            who = PILOT_STATE::MPD;  // koniec przegladania katalogow

        }
        else if (X == PILOT_KEY::KEY_VOLUMEUP)
        {
            my_data_logic->main_MENU->next();  // naspteny katalog
        }
        else if (X == PILOT_KEY::KEY_VOLUMEDOWN)
        {
            my_data_logic->main_MENU->previous(); //poprzedni katalog
        }
        else if (X == PILOT_KEY::KEY_OK)
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
                    who=PILOT_STATE::SLEEPER;
                }
                else if (my_data_logic->main_MENU->show_list() == "2.TEMPERATURA" ){
                    std::cout << " temperatura !!!!" << std::endl;
                    who=PILOT_STATE::MPD;
                    _add(PILOT_KEY::KEY_SAT);
                }
                else if (my_data_logic->main_MENU->show_list() == "4.PLIKI" ){
                    std::cout << " do filmow" << std::endl;
                    who=PILOT_STATE::MPD;
                    _add(PILOT_KEY::KEY_EPG);
                    _add(PILOT_KEY::KEY_VOLUMEUP);
                }
            }
        }
        else if (X==PILOT_KEY::KEY_UP)
        {
            my_data_logic->main_MENU->back_dir();
        }
        my_data_logic->main_MENU->show_list();
    }
}


PILOT_KEY c_irda_logic::_get( )
{
    PILOT_KEY temp = PILOT_KEY::DUMMY;
    if (irda_queue.empty() == false){
        temp = irda_queue.front();
        irda_queue.pop();
    }
    return temp;
}

int c_irda_logic::_size() const
{
    return irda_queue.size();
}
