#include "mpd_cli.h"
//#include "../iDom_server/src/iDom_server.h"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32;06m"
#define YELLOW "\x1b[33;06m"
#define RED "\x1b[33;06m"
extern int debug_level;

bool check_title_song_to = false;

void error_callback(MpdObj *mi,int errorid, char *msg, void *userdata)
{
    printf(RED"Error "RESET""GREEN"%i:"RESET" '%s'\n", errorid, msg);
}

void status_changed(MpdObj *mi, ChangedStatusType what,  thread_data *my_data)
{
    if(what&MPD_CST_SONGID)
    {
        mpd_Song *song = mpd_playlist_get_current_song(mi);
        if(song)
        {
            printf(GREEN"Song:"RESET" %s - %s\n", song->artist, song->title);
        }
    }


    if(what&MPD_CST_REPEAT){
        printf(GREEN"Repeat:"RESET" %s\n", mpd_player_get_repeat(mi)? "On":"Off");
    }
    if(what&MPD_CST_RANDOM){
        printf(GREEN"Random:"RESET" %s\n", mpd_player_get_random(mi)? "On":"Off");
    }
    if(what&MPD_CST_VOLUME){
        printf(GREEN"Volume:"RESET" %03i%%\n",
               mpd_status_get_volume(mi));

        my_data->mainLCD->printVolume(mpd_status_get_volume(mi));
        try
        {
            my_data->ptr_MPD_info->volume= mpd_status_get_volume(mi);
        }
        catch (...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "problem z wpisaniem volume "<<   std::endl;
            log_file_mutex.mutex_unlock();
        }
    }
    if(what&MPD_CST_CROSSFADE){
        printf(GREEN"X-Fade:"RESET" %i sec.\n",
               mpd_status_get_crossfade(mi));
    }
    if(what&MPD_CST_UPDATING)
    {
        if(mpd_status_db_is_updating(mi))
        {
            printf(GREEN"Started updating DB"RESET"\n");
        }
        else
        {
            printf(GREEN"Updating DB finished"RESET"\n");
        }
    }
    if(what&MPD_CST_DATABASE)
    {
        printf(GREEN"Databased changed"RESET"\n");
    }
    if(what&MPD_CST_PLAYLIST)
    {
        printf(GREEN"Playlist changed2"RESET"\n");
        if (check_title_song_to==true)
        {
            mpd_Song *song = mpd_playlist_get_current_song(mi);
            // std::cout <<" SONG: " << song->artist<<" "<< song->title << std::endl;
            printf(GREEN"aktualnie gramy:"RESET" %s - %s\n", song->artist, song->title);

            try
            {
                my_data->ptr_MPD_info->title = std::string( song->title);
            }
            catch (...)
            {
                log_file_mutex.mutex_lock();
                log_file_cout << ERROR << "problem z wpisaniem tytulu "<<   std::endl;
                log_file_mutex.mutex_unlock();
            }



            if (song->name != NULL){
                _msg =  song->name;

                try
                {
                    my_data->ptr_MPD_info->radio = _msg;
                }
                catch (...)
                {
                    log_file_mutex.mutex_lock();
                    log_file_cout << ERROR << "problem z wpisaniem radia "<<   std::endl;
                    log_file_mutex
                            .mutex_unlock();
                }
                my_data->mainLCD->printRadioName(true,0,0,_msg);
                my_data->mainLCD->set_lcd_STATE(5);
                std::string temp_str="";
                temp_str = send_to_arduino(my_data,"temperature:2;");
                temp_str.erase(temp_str.size()-2,temp_str.size());
                my_data->mainLCD->printString(false,0,1,"temp:"+temp_str+" c");
            }


            if (song->title != NULL ){
                _msg =  song->title;
                if (_msg.size() < 7 )
                {
                    _msg =  song->name;
                    _msg += " -     brak nazwy                ";
                }

            }
            else
            {

                _msg += " -     brak nazwy      ";
            }



            my_data->mainLCD->printSongName(_msg);
        }
    }
    if(what&MPD_CST_STATE)
    {
        printf(GREEN"State:"RESET);
        switch(mpd_player_get_state(mi))
        {
        case MPD_PLAYER_PLAY:
            printf("Playing\n");
            check_title_song_to=true;
            my_data->mainLCD->play_Y_N=true;
            my_data->ptr_MPD_info->isPlay=true;
            digitalWrite(GPIO_SPIK, LOW);
            my_data->mainLCD->set_lcd_STATE(1);
            my_data->mainLCD->song_printstr();
            break;
        case MPD_PLAYER_PAUSE:
            printf("Paused\n");
            my_data->mainLCD->set_lcd_STATE( -1);
            my_data->mainLCD->printString(true ,0,1,"    PAUSE");
            break;
        case MPD_PLAYER_STOP:
            printf("Stopped\n");
            check_title_song_to=false;
            my_data->mainLCD->play_Y_N=false;
            my_data->ptr_MPD_info->isPlay=false;
            my_data->ptr_MPD_info->title="* * * *";
            digitalWrite(GPIO_SPIK,HIGH);
            my_data->mainLCD->noBacklight();
            break;
        default:
            break;
        }
    }
    /* not yet implemented signals */
    if(what&MPD_CST_AUDIO){
        printf(GREEN"Audio Changed"RESET"\n");
    }
    if(what&MPD_CST_TOTAL_TIME){
        printf(GREEN"Total song time changed:"RESET" %02i:%02i\n",
               mpd_status_get_total_song_time(mi)/60,
               mpd_status_get_total_song_time(mi)%60);
    }
    if(what&MPD_CST_ELAPSED_TIME){
        /*  printf(GREEN"Time elapsed changed:"RESET" %02i:%02i\n",
mpd_status_get_elapsed_song_time(mi)/60,
mpd_status_get_elapsed_song_time(mi)%60);
*/  }
    if(what&MPD_CST_PERMISSION){
        printf(YELLOW"Permission:"RESET" Changed\n");
    }
}

void  *main_mpd_cli(void *data )
{

    blockQueue char_queue; // kolejka polecen
    thread_data  *my_data;
    my_data = (thread_data*)data;

    ////////////////////////////// LCD PART ///////////////////////
    // LCD_c m_lcd(0x27,16,2);
    my_data->mainLCD->set_print_song_state(0);
    ///////////////////////////////////////////////////////////////////
    //int fdstdin = 0;
    int   iport = 6600, button_counter =0;
    char *hostname = getenv("MPD_HOST");
    char *port = getenv("MPD_PORT");
    char *password = getenv("MPD_PASSWORD");
    MpdObj *obj = NULL;
    /* Make the input non blocking */
    // fdstdin = open("/dev/stdin", O_NONBLOCK|O_RDONLY);
    /* set correct hostname */
    std::cout << " adres hosta to " << hostname << std::endl;
    if(!hostname) {


        std::cout << " ip mpd to " <<  my_data->server_settings->MPD_IP << " ! \n";
        hostname = (char*)my_data->server_settings->MPD_IP.c_str();
    }
    if(port){
        iport = std::stoi(port);
    }
    /* Create mpd object */
    obj = mpd_new(hostname, iport,password);

    /* Connect signals */
    mpd_signal_connect_error(obj,(ErrorCallback)error_callback, NULL);
    mpd_signal_connect_status_changed(obj,(StatusChangedCallback)status_changed , my_data);
    /* Set timeout */
    mpd_set_connection_timeout(obj, 10);

    int work;
    work = mpd_connect(obj);

    while (work){

        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "nie udalo sie polaczyc z MPD "<<   std::endl;
        log_file_mutex.mutex_unlock();
        system("service mpd stop");
        sleep(1);
        system("service mpd start");
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "restart MPD "<<   std::endl;

        log_file_cout << INFO << "nawiazuje nowe polaczenie z MPD "<<   std::endl;
        log_file_mutex.mutex_unlock();
        work = mpd_connect(obj);
    }

    std::cout << " stop "<<std::endl;

    if(!work)
    {
        char buffer;
        // mpd_send_password(obj);
        //memset(buffer, '\0', 20);
        do{

            if(char_queue._size() > 0)
            {
                //digitalWrite(LED7,1);
                buffer = char_queue._get();
                switch(buffer)
                {
                case '\n':
                    break;
                case 'D':
                    mpd_player_next(obj);
                    break;
                case 'U':
                    mpd_player_prev(obj);
                    break;
                case 't':
                    mpd_player_play(obj);
                    break;
                case 'A':
                    mpd_player_pause(obj);
                    break;
                case 'P':
                    mpd_player_stop(obj);
                    break;
                case 'q':

                    printf("Quitting....\n");
                    break;
                case 'R':
                    mpd_player_set_repeat(obj, !mpd_player_get_repeat(obj));
                    break;
                case 's':
                    mpd_player_set_random(obj, !mpd_player_get_random(obj));
                    break;
                case '1':
                {
                    MpdData *data = mpd_playlist_get_changes(obj,-1);
                    if(data)
                    {
                        printf(GREEN"Playlist:"RESET"\n");
                        do{

                            if(data->type == MPD_DATA_TYPE_SONG)
                            {
                                printf(GREEN"%i"RESET": %s - %s\n", data->song->id,
                                       data->song->artist,
                                       data->song->title);
                            }
                            data = mpd_data_get_next(data);
                        }while(data);
                    }
                    break;
                }
                case 'p':
                    /*if(char_queue._size() > 0)
{
buffer = char_queue._get();
int id = atoi(buffer);
printf(GREEN"Playing:"RESET" %i\n", id);
mpd_player_play_id(obj,id);
}
break;*/

                case '+':
                    mpd_status_set_volume(obj, mpd_status_get_volume(obj)+1);
                    break;
                case '-':
                    mpd_status_set_volume(obj, mpd_status_get_volume(obj)-1);
                    break;
                case '2':
                    debug_level = (debug_level > 0)?0:3;
                    printf(YELLOW"Debug:"RESET" %s\n", (debug_level >0)? "Enabled":"Disabled");
                    break;
                case 'h':
                    printf("\th:\t\tHelp\n"\
                           "\td:\t\tToggle debug on/off\n"\
                           "\t+:\t\tIncrease volume\n"\
                           "\t-:\t\tDecrease volume\n"\
                           "\ta <pass>:\t Authentificate with pass\n"\
                           "\tp <id>:\t Play song with id\n"\
                           "\tl:\t\tList the playlist\n"\
                           "\ts:\t\tToggle shuffle mode\n"\
                           "\tr:\t\tToggle repeat\n"\
                           "\tq:\t\tQuit\n"\
                           "\tv:\t\tStop\n"\
                           "\tc:\t\tPause\n"\
                           "\tx:\t\tPlay\n"\
                           "\tz:\t\tPrevious\n"\
                           "\tb:\t\tNext\n");
                    break;
                default:
                    printf("buffer: %c\n", buffer);


                }
                //  digitalWrite(LED7,0);  // gasze sygnal odbioru _msgomosci

            }
            if (!mpd_check_connected(obj))
            {
                log_file_mutex.mutex_lock();
                log_file_cout << WARNING << "utracono polacznie z  MPD "<<   std::endl;
                log_file_cout << INFO << "restart MPD" << std::endl;
                log_file_mutex.mutex_unlock();
                system ("service mpd restart");
                mpd_connect(obj);

            }
            //std::cout << " \n\nprzed if \n\n";
            mpd_status_update(obj);


            my_data->mainLCD->checkState();

            /////////////////////////////////////////////////////////////////////////////////


            if ( digitalRead(BUTTON_PIN) == HIGH )
            {
                std::cout << " wcisnieta pin !" << std::endl;
                if (check_title_song_to == true && button_counter ==4)
                {
                    char_queue._add('P');
                    std::cout << " \n\ngasze !" << std::endl;
                }
                else if (check_title_song_to == false && button_counter ==4)
                {
                    char_queue._add('t');
                    std::cout << " \n\n\n zapalam !" << std::endl;
                }
                else if (check_title_song_to == false && button_counter ==10)
                {

                    std::cout << " \n\n\n koniec programu z przyciska !" << std::endl;
                    go_while = false;
                }

                std::cout << " \n\n\n licznik guzika wskazuje "<< button_counter << std::endl;
                ++button_counter;
            }
            else button_counter =0;



            //m_lcd.checkState();
        }while(!usleep(500000) &&  go_while);
        mpd_player_stop(obj);
        sleep (3);

    }
    else{
        std::cout << " NIE UDALO SIE POłączyć "<<std::endl;

    }
    mpd_free(obj);
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << " koniec watku klient MPD  "<<   std::endl;
    log_file_mutex.mutex_unlock();
    //close(fdstdin);


    return 0;
}