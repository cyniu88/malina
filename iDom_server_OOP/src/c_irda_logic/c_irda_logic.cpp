#include "c_irda_logic.h"
#include "../iDom_server_OOP.h"
#include "../thread_functions/iDom_thread.h"

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
        log_file_cout << CRITICAL << "nieznana komenda MPD z pilota "<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
}

void c_irda_logic::sleeperLogic(PILOT_KEY X)
{
    my_data->mainLCD->set_print_song_state(100);
    my_data->mainLCD->printString(true,0,0,std::to_string(my_data->sleeper)+" minut");

    switch (X){
    case PILOT_KEY::KEY_EXIT:
    {
        my_data->sleeper=0;
        my_data->mainLCD->set_print_song_state(0);
        who = PILOT_STATE::MPD;
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        ++my_data->sleeper;
        my_data->mainLCD->printString(true,0,0,std::to_string(my_data->sleeper)+" minut");
        break;
    }
    case PILOT_KEY::KEY_DOWN:
    {
        --my_data->sleeper;
        my_data->mainLCD->printString(true,0,0,std::to_string(my_data->sleeper)+" minut");
        break;
    }
    case PILOT_KEY::KEY_CHANNELUP:
    {
        my_data->sleeper+=10;
        my_data->mainLCD->printString(true,0,0,std::to_string(my_data->sleeper)+" minut");
        break;
    }
    case PILOT_KEY::KEY_CHANNELDOWN:
    {
        my_data->sleeper-=10;
        my_data->mainLCD->printString(true,0,0,std::to_string(my_data->sleeper)+" minut");
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        iDOM_THREAD::start_thread("Sleeper MPD",useful_F::sleeper_mpd,my_data);
        my_data->mainLCD->printString(true,1,0,"SLEEPer START");
        my_data->mainLCD->set_print_song_state(0);
        who = PILOT_STATE::MPD;
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w sleeperLogic"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
}

void c_irda_logic::projectorLogic(PILOT_KEY X)
{
    my_data->mainLCD->set_print_song_state(100);
    my_data->mainLCD->printString(false,2,1,"  PROJEKTOR   ");

    switch (X)
    {
    case PILOT_KEY::KEY_EXIT:
    {
        my_data->mainLCD->set_print_song_state(0);
        who = PILOT_STATE::MPD;

        if(my_data->ptr_MPD_info->isPlay == true){
            iDomTOOLS::MPD_play(my_data);
        }
        else {
            iDomTOOLS::turnOffSpeakers();
            my_data->mainLCD->set_print_song_state(0); // off display
            my_data->mainLCD->set_lcd_STATE(0);
        }
        break;
    }
    case PILOT_KEY::DUMMY:
    {
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "+");
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "-");
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        my_data->mainLCD->set_print_song_state(1000);
        my_data->mainLCD->printString(false,0,0,"ODTWARZAM VIDEO");
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "p");
        break;
    }
    case PILOT_KEY::KEY_POWER:
    {
        //system("echo -n q > /mnt/ramdisk/cmd &"); // zamykanie omxplayera
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "q");
        break;
    }
    case PILOT_KEY::KEY_DOWN:
    {
        system("echo -n $'\x1b\x5b\x43' > /mnt/ramdisk/cmd"); // do przodu
        //write_to_mkfifo("$'\x1b\x5b\x43'");
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        system("echo -n $'\x1b\x5b\x44' > /mnt/ramdisk/cmd"); // do tylu
        // write_to_mkfifo("$'\x1b\x5b\x44'");
        break;
    }
    case PILOT_KEY::KEY_CHANNELUP:
    {
        //system("echo -n o > /mnt/ramdisk/cmd"); // do przodu
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "o");
        break;
    }
    case PILOT_KEY::KEY_CHANNELDOWN:
    {
        //system("echo -n i > /mnt/ramdisk/cmd"); // do tylu
        useful_F_libs::write_to_mkfifo(my_data->server_settings->omxplayerFile, "i");
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w projector"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
}

void c_irda_logic::movieLogic(PILOT_KEY X)
{
    switch (X)
    {
    case PILOT_KEY::KEY_EXIT:
    {
        my_data->mainLCD->set_print_song_state(0);
        my_data->mainLCD->set_lcd_STATE(2);
        who = PILOT_STATE::MPD; // koniec przegladania katalogow
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        my_data->main_tree->next(); // naspteny katalog
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        my_data->main_tree->previous(); //poprzedni katalog
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        // whodze w katalog lub odtwarzma plik

        if (my_data->main_tree->is_file() == false)
        {
            my_data->main_tree->enter_dir();
            my_data->main_tree->show_list();
        }
        else
        {
            std::cout << " URUCHAMIAM PLIK! " <<my_data->main_tree->show_list() <<std::endl;

            std::string command = "/home/pi/programowanie/iDom_server_OOP/script/PYTHON/iDom_movie.py ";
            command+=my_data->main_tree->show_list();
            system(command.c_str());
            std::cout << " WYSTARTOWALEM!!";
            my_data->mainLCD->set_lcd_STATE(-1);
            my_data->mainLCD->printString(true,0,0,"odtwarzam film");
            my_data->mainLCD->printString(false,0,1,my_data->main_tree->show_list());
            who = PILOT_STATE::PROJECTOR;

            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "odtwarzam film "<< my_data->ptr_MPD_info->isPlay << std::endl;
            log_file_mutex.mutex_unlock();

            if (my_data->ptr_MPD_info->isPlay == true){
                iDomTOOLS::MPD_pause(); // projektor wlaczony wiec pauzuje radio
            }
            else{
                iDomTOOLS::turnOnSpeakers();
                puts("wlaczam glosnik do filmu");
            }
        }
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        my_data->main_tree->back_dir();
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w moveLogic"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }

    my_data->main_tree->show_list();
}

void c_irda_logic::menuLogic(PILOT_KEY X)
{
    switch (X)
    {
    case PILOT_KEY::KEY_EXIT:
    {
        my_data->mainLCD->set_print_song_state(0);
        my_data->mainLCD->set_lcd_STATE(2);
        who = PILOT_STATE::MPD; // koniec przegladania katalogow
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        my_data->main_MENU->next(); // naspteny katalog
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        my_data->main_MENU->previous(); //poprzedni katalog
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        // whodze w katalog lub odtwarzma plik

        if (my_data->main_MENU->is_file() == false)
        {
            my_data->main_MENU->enter_dir();
            my_data->main_MENU->show_list();
        }
        else
        {
            // menu start
            if (my_data->main_MENU->show_list() == "5.SLEEPer"){
                std::cout << " POBUDKA!!!!" << std::endl;
                who=PILOT_STATE::SLEEPER;
            }
            else if (my_data->main_MENU->show_list() == "2.TEMPERATURA"){
                std::cout << " temperatura !!!!" << std::endl;
                who=PILOT_STATE::MPD;
                _add(PILOT_KEY::KEY_SAT);
            }
            else if (my_data->main_MENU->show_list() == "4.PLIKI"){
                std::cout << " do filmow" << std::endl;
                who=PILOT_STATE::MPD;
                _add(PILOT_KEY::KEY_EPG);
                _add(PILOT_KEY::KEY_VOLUMEUP);
            }
        }
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        my_data->main_MENU->back_dir();
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w menuLogic"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;

    }
    my_data->main_MENU->show_list();
}

void c_irda_logic::mainPilotHandler(PILOT_KEY X)
{
    switch (X){

    case PILOT_KEY::KEY_RADIO:
    {
        who = PILOT_STATE::PROJECTOR;
        _add(PILOT_KEY::DUMMY);
        break;
    }

    case PILOT_KEY::KEY_SUBTITLE:
    {
        my_data->mainLCD->set_lcd_STATE(10);
        my_data->mainLCD->printString(true,0,0,"GASZE LEDy");
        std::string temp_str="";
        temp_str.erase();
        temp_str += my_data->main_iDomTools->ledOFF();
        my_data->mainLCD->printString(false,0,1,temp_str);
        who=PILOT_STATE::MPD;
        break;
    }

    case PILOT_KEY::KEY_LANGUAGE:
    {
        my_data->mainLCD->set_lcd_STATE(10);
        my_data->mainLCD->printString(true,0,0,"ZAPALAM LEDy");
        std::string temp_str = my_data->ptr_pilot_led->colorLED[my_data->ptr_pilot_led->counter].getColorName();
        my_data->main_iDomTools->ledOn(my_data->ptr_pilot_led->colorLED[my_data->ptr_pilot_led->counter]);

        if (++my_data->ptr_pilot_led->counter > my_data->ptr_pilot_led->colorLED.size()-1 )
        {
            my_data->ptr_pilot_led->counter=0;
        }

        my_data->mainLCD->printString(false,0,1,temp_str);
        who = PILOT_STATE::MPD;
        break;
    }

    case PILOT_KEY::KEY_SAT:
    {
        my_data->mainLCD->set_lcd_STATE(10);
        my_data->mainLCD->printString(true,0,0,"SMOG: "+my_data->main_iDomTools->getSmog()+" mg/m^3");
        std::string temp_str = "I:";
        std::vector<std::string> temper = my_data->main_iDomTools->getTemperature();
        //temp_str += my_data_logic->main_iDomTools->getTemperatureString();// send_to_arduino(my_data_logic,"temperature:2;");
        temp_str += temper.at(0);
        temp_str += " O:"+ temper.at(1);
        my_data->mainLCD->printString(false,0,1,temp_str+" c");
        who = PILOT_STATE::MPD;
        break;
    }

    case PILOT_KEY::KEY_EPG:
        who = PILOT_STATE::MOVIE;
        my_data->main_tree->show_list(); //printuje pierwszy element
        my_data->mainLCD->set_print_song_state(100);
        break;

    case PILOT_KEY::KEY_MENU:
        who = PILOT_STATE::MENU;
        my_data->main_MENU->show_list();
        my_data->mainLCD->set_print_song_state(100);
        break;
    case PILOT_KEY::KEY_FAVORITES:
        my_data->main_iDomTools->turnOnOffPrinter();
        break;
    case PILOT_KEY::KEY_TEXT:
        my_data->main_iDomTools->turnOnOff433MHzSwitch("listwa");
        break;
    case PILOT_KEY::KEY_REFRESH:
        my_data->main_iDomTools->startKodi_Thread();
        break;
    default:
        irdaMPD(X);
    }
}

c_irda_logic::c_irda_logic(thread_data *my_data):my_data(my_data)
{
    // my_data = my_data;
    who = PILOT_STATE::MPD;
}

void c_irda_logic::_add(PILOT_KEY X)
{
    switch (who){
    case PILOT_STATE::MPD:
        mainPilotHandler(X);
        break;
    case PILOT_STATE::SLEEPER:
        sleeperLogic(X);
        break;
    case PILOT_STATE::PROJECTOR:
        projectorLogic(X);
        break;
    case PILOT_STATE::MOVIE:
        movieLogic(X);
        break;
    case PILOT_STATE::MENU:
        menuLogic(X);
        break;
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany pilotState"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
}

//PILOT_KEY c_irda_logic::_get( )
//{
//    PILOT_KEY temp = PILOT_KEY::DUMMY;
//    if (irda_queue.empty() == false){
//        temp = irda_queue.front();
//        irda_queue.pop();
//    }
//    return temp;
//}

//int c_irda_logic::_size() const
//{
//    return irda_queue.size();
//}
