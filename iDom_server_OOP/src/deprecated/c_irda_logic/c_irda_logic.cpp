#include "c_irda_logic.h"
#include "../thread_functions/iDom_thread.h"
#include "../functions/functions.h"

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
    context->mainLCD->set_print_song_state(100);
    context->mainLCD->printString(true,0,0,std::to_string(context->sleeper) + " minut");

    switch (X){
    case PILOT_KEY::KEY_EXIT:
    {
        context->sleeper = 0;
        context->mainLCD->set_print_song_state(0);
        who = PILOT_STATE::MPD;
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        ++context->sleeper;
        context->mainLCD->printString(true,0,0,std::to_string(context->sleeper) + " minut");
        break;
    }
    case PILOT_KEY::KEY_DOWN:
    {
        --context->sleeper;
        context->mainLCD->printString(true,0,0,std::to_string(context->sleeper) + " minut");
        break;
    }
    case PILOT_KEY::KEY_CHANNELUP:
    {
        context->sleeper += 10;
        context->mainLCD->printString(true,0,0,std::to_string(context->sleeper) + " minut");
        break;
    }
    case PILOT_KEY::KEY_CHANNELDOWN:
    {
        context->sleeper -= 10;
        context->mainLCD->printString(true,0,0,std::to_string(context->sleeper) + " minut");
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        iDOM_THREAD::start_thread("Sleeper MPD",useful_F::sleeper_mpd,context);
        context->mainLCD->printString(true,1,0,"SLEEPer START");
        context->mainLCD->set_print_song_state(0);
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
    context->mainLCD->set_print_song_state(100);
    context->mainLCD->printString(false,2,1,"  PROJEKTOR   ");

    switch (X)
    {
    case PILOT_KEY::KEY_EXIT:
    {
        context->mainLCD->set_print_song_state(0);
        who = PILOT_STATE::MPD;

        if(context->ptr_MPD_info->isPlay == true){
            iDomTOOLS::MPD_play(context);
        }
        else {
            iDomTOOLS::turnOffSpeakers();
            context->mainLCD->set_print_song_state(0); // off display
            context->mainLCD->set_lcd_STATE(0);
        }
        break;
    }
    case PILOT_KEY::DUMMY:
    {
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "+");
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "-");
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        context->mainLCD->set_print_song_state(1000);
        context->mainLCD->printString(false,0,0,"ODTWARZAM VIDEO");
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "p");
        break;
    }
    case PILOT_KEY::KEY_POWER:
    {
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "q");
        break;
    }
    case PILOT_KEY::KEY_DOWN:
    {
        useful_F::runLinuxCommand("echo -n $'\x1b\x5b\x43' > /mnt/ramdisk/cmd"); // do przodu
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        useful_F::runLinuxCommand("echo -n $'\x1b\x5b\x44' > /mnt/ramdisk/cmd"); // do tylu
        break;
    }
    case PILOT_KEY::KEY_CHANNELUP:
    {
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "o");
        break;
    }
    case PILOT_KEY::KEY_CHANNELDOWN:
    {
        useful_F_libs::write_to_mkfifo(context->server_settings->_server.omxplayerFile, "i");
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
        context->mainLCD->set_print_song_state(0);
        context->mainLCD->set_lcd_STATE(2);
        who = PILOT_STATE::MPD; // koniec przegladania katalogow
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        context->main_tree->next(); // naspteny katalog
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        context->main_tree->previous(); //poprzedni katalog
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        // whodze w katalog lub odtwarzma plik
        if (context->main_tree->is_file() == false)
        {
            context->main_tree->enter_dir();
            context->main_tree->show_list();
        }
        else
        {
            std::cout << " URUCHAMIAM PLIK! " <<context->main_tree->show_list() <<std::endl;

            std::string command = "/home/pi/programowanie/iDom_server_OOP/script/PYTHON/iDom_movie.py ";
            command.append(context->main_tree->show_list());
            useful_F::runLinuxCommand(command);
            std::cout << " WYSTARTOWALEM!!";
            context->mainLCD->set_lcd_STATE(-1);
            context->mainLCD->printString(true,0,0,"odtwarzam film");
            context->mainLCD->printString(false,0,1,context->main_tree->show_list());
            who = PILOT_STATE::PROJECTOR;

            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "odtwarzam film "<< context->ptr_MPD_info->isPlay << std::endl;
            log_file_mutex.mutex_unlock();

            if (context->ptr_MPD_info->isPlay == true){
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
        context->main_tree->back_dir();
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w moveLogic"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
    context->main_tree->show_list();
}

void c_irda_logic::menuLogic(PILOT_KEY X)
{
    switch (X)
    {
    case PILOT_KEY::KEY_EXIT:
    {
        context->mainLCD->set_print_song_state(0);
        context->mainLCD->set_lcd_STATE(2);
        who = PILOT_STATE::MPD; // koniec przegladania katalogow
        break;
    }
    case PILOT_KEY::KEY_VOLUMEUP:
    {
        context->main_MENU->next(); // naspteny katalog
        break;
    }
    case PILOT_KEY::KEY_VOLUMEDOWN:
    {
        context->main_MENU->previous(); //poprzedni katalog
        break;
    }
    case PILOT_KEY::KEY_OK:
    {
        // whodze w katalog lub odtwarzma plik
        if (context->main_MENU->is_file() == false)
        {
            context->main_MENU->enter_dir();
            context->main_MENU->show_list();
        }
        else
        {
            // menu start
            if (context->main_MENU->show_list() == "5.SLEEPer"){
                std::cout << " POBUDKA!!!!" << std::endl;
                who = PILOT_STATE::SLEEPER;
            }
            else if (context->main_MENU->show_list() == "2.TEMPERATURA"){
                std::cout << " temperatura !!!!" << std::endl;
                who = PILOT_STATE::MPD;
                _add(PILOT_KEY::KEY_SAT);
            }
            else if (context->main_MENU->show_list() == "4.PLIKI"){
                std::cout << " do filmow" << std::endl;
                who = PILOT_STATE::MPD;
                _add(PILOT_KEY::KEY_EPG);
                _add(PILOT_KEY::KEY_VOLUMEUP);
            }
        }
        break;
    }
    case PILOT_KEY::KEY_UP:
    {
        context->main_MENU->back_dir();
        break;
    }
    default:
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "nieznany case w menuLogic"<< std::endl;
        log_file_mutex.mutex_unlock();
        break;
    }
    context->main_MENU->show_list();
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
        context->mainLCD->set_lcd_STATE(10);
        context->mainLCD->printString(true,0,0,"GASZE LEDy");
        std::string temp_str = "";
        temp_str.erase();
        temp_str.append(context->main_iDomTools->ledOFF());
        context->mainLCD->printString(false,0,1,temp_str);
        who = PILOT_STATE::MPD;
        break;
    }
    case PILOT_KEY::KEY_LANGUAGE:
    {
        context->mainLCD->set_lcd_STATE(10);
        context->mainLCD->printString(true,0,0,"ZAPALAM LEDy");
        std::string temp_str = context->ptr_pilot_led->colorLED[context->ptr_pilot_led->counter].getColorName();
        context->main_iDomTools->ledOn(context->ptr_pilot_led->colorLED[context->ptr_pilot_led->counter]);

        if (++context->ptr_pilot_led->counter > context->ptr_pilot_led->colorLED.size()-1 )
        {
            context->ptr_pilot_led->counter = 0;
        }

        context->mainLCD->printString(false,0,1,temp_str);
        who = PILOT_STATE::MPD;
        break;
    }
    case PILOT_KEY::KEY_SAT:
    {
        context->mainLCD->set_lcd_STATE(10);
        context->mainLCD->printString(true,0,0,"SMOG: " + context->main_iDomTools->getSmog() + " mg/m^3");
        std::string temp_str = "I:";
        std::vector<std::string> temper = context->main_iDomTools->getTemperature();
        //temp_str += context_logic->main_iDomTools->getTemperatureString();// send_to_arduino(context_logic,"temperature:2;");
        temp_str.append(temper.at(0));
        temp_str.append(" O:"+ temper.at(1));
        context->mainLCD->printString(false,0,1,temp_str + " c");
        who = PILOT_STATE::MPD;
        break;
    }
    case PILOT_KEY::KEY_EPG:
        who = PILOT_STATE::MOVIE;
        context->main_tree->show_list(); //printuje pierwszy element
        context->mainLCD->set_print_song_state(100);
        break;
    case PILOT_KEY::KEY_MENU:
        who = PILOT_STATE::MENU;
        context->main_MENU->show_list();
        context->mainLCD->set_print_song_state(100);
        break;
    case PILOT_KEY::KEY_FAVORITES:
        context->main_iDomTools->turnOnOffPrinter();
        break;
    case PILOT_KEY::KEY_TEXT:
        context->main_iDomTools->turnOnOff433MHzSwitch("listwa");
        break;
    case PILOT_KEY::KEY_REFRESH:
        context->main_iDomTools->startKodi_Thread();
        break;
    default:
        irdaMPD(X);
    }
}

c_irda_logic::c_irda_logic(thread_context *context):context(context)
{
    m_className.append(typeid(this).name());
    who = PILOT_STATE::MPD;
    iDom_API::addToMap(m_className,this);
}

c_irda_logic::~c_irda_logic()
{
    iDom_API::removeFromMap(m_className);
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

std::string c_irda_logic::dump() const
{
    std::stringstream ret;
    ret << " who: " << static_cast<int>(this->who) << std::endl;
    return ret.str();
}
