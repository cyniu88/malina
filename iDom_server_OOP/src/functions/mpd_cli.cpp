#include "mpd_cli.h"

extern int debug_level;
bool check_title_song_to = false;

void error_callback(MpdObj *mi,int errorid, char *msg, void *userdata)
{
    printf( "Error """"%i:"" '%s'\n", errorid, msg);
}

void status_changed(MpdObj *mi, ChangedStatusType what,  thread_data *my_data)
{
    if(what&MPD_CST_SONGID)
    {
        mpd_Song *song = mpd_playlist_get_current_song(mi);
        if(song)
        {
            printf( "Song:"" %s - %s\n", song->artist, song->title);
        }
    }

    if(what&MPD_CST_REPEAT){
        printf("Repeat:"" %s\n", mpd_player_get_repeat(mi)? "On":"Off");
    }
    if(what&MPD_CST_RANDOM){
        printf("Random:"" %s\n", mpd_player_get_random(mi)? "On":"Off");
    }
    if(what&MPD_CST_VOLUME){
        printf("Volume:"" %03i%%\n",
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
        printf("X-Fade:"" %i sec.\n",
               mpd_status_get_crossfade(mi));
    }
    if(what&MPD_CST_UPDATING)
    {
        if(mpd_status_db_is_updating(mi))
        {
            printf("Started updating DB""\n");
        }
        else
        {
            printf("Updating DB finished""\n");
        }
    }
    if(what&MPD_CST_DATABASE)
    {
        printf("Databased changed""\n");
    }
    if(what&MPD_CST_PLAYLIST)
    {
        printf("Playlist changed2""\n");
        if (check_title_song_to==true)
        {
            mpd_Song *song = mpd_playlist_get_current_song(mi);
            // std::cout <<" SONG: " << song->artist<<" "<< song->title << std::endl;
            printf("aktualnie gramy:"" %s - %s\n", song->artist, song->title);

            try
            {
                my_data->ptr_MPD_info->title = std::string( song->title);
            }
            catch (...)
            {
                my_data->myEventHandler.run("mpd")->addEvent("wrong title");
                my_data->ptr_MPD_info->title = "no data";
            }
            try
            {
                my_data->ptr_MPD_info->artist = std::string( song->artist);
            }
            catch (...)
            {
                my_data->myEventHandler.run("mpd")->addEvent("wrong artist");
                my_data->ptr_MPD_info->artist = "no data";
            }

            if (song->name != NULL){
                _msg =  song->name;

                try
                {
                    my_data->ptr_MPD_info->radio = _msg;
                }
                catch (...)
                {
                    my_data->myEventHandler.run("mpd")->addEvent("wrong radio station name");
                }
                my_data->mainLCD->printRadioName(true,0,0,_msg);
                my_data->mainLCD->set_lcd_STATE(5);
                std::string temp_str = my_data->main_iDomTools->getTemperatureString(); // send_to_arduino(my_data,"temperature:2;");
                temp_str.erase(temp_str.size()-2,temp_str.size());
                my_data->mainLCD->printString(false,0,1,"temp:"+temp_str+" c");

                updatePlayList(mi,my_data);
            }

            if (song->title != NULL ){
                _msg =  song->title;
                if (_msg.size() < 7 )
                {
                    _msg =  song->name;
                    _msg += " -     brak nazwy                ";
                }
            }
            else if (song->artist != NULL){
                _msg = song->artist;
            }
            else
            {
                _msg += " -     brak nazwy      ";
            }
            // my_data->ptr_MPD_info->title = _msg;
            my_data->mainLCD->printSongName(_msg);
        }
    }
    if(what&MPD_CST_STATE)
    {
        printf("State:");
        switch(mpd_player_get_state(mi))
        {
        case MPD_PLAYER_PLAY:
            printf("Playing\n");
            check_title_song_to=true;
            my_data->mainLCD->play_Y_N=true;
            my_data->ptr_MPD_info->isPlay=true;
            digitalWrite(iDomConst::GPIO_SPIK, LOW);
            my_data->mainLCD->set_lcd_STATE(1);
            my_data->mainLCD->song_printstr();
            updatePlayList(mi,my_data);
            my_data->myEventHandler.run("mpd")->addEvent("MPD playing");
            break;
        case MPD_PLAYER_PAUSE:
            printf("Paused\n");
            my_data->mainLCD->set_lcd_STATE( -1);
            my_data->mainLCD->printString(true ,0,1,"    PAUSE");
            my_data->myEventHandler.run("mpd")->addEvent("MPD pause");
            break;
        case MPD_PLAYER_STOP:
            printf("Stopped\n");
            if (my_data->ptr_MPD_info->isPlay ==true){
                my_data->main_iDomTools->ledClear();
            }
            check_title_song_to=false;
            my_data->mainLCD->play_Y_N=false;
            my_data->ptr_MPD_info->isPlay=false;
            my_data->ptr_MPD_info->title="* * * *";
            digitalWrite(iDomConst::GPIO_SPIK,HIGH);
            my_data->mainLCD->noBacklight();
            sleep(1);
            my_data->myEventHandler.run("mpd")->addEvent("MPD stopped");
            break;
        default:
            break;
        }
    }
    /* not yet implemented signals */
    if(what&MPD_CST_AUDIO){
        printf("Audio Changed""\n");
    }
    if(what&MPD_CST_TOTAL_TIME){
        printf("Total song time changed:"" %02i:%02i\n",
               mpd_status_get_total_song_time(mi)/60,
               mpd_status_get_total_song_time(mi)%60);
    }
    if(what&MPD_CST_ELAPSED_TIME){
        /*  printf(GREEN"Time elapsed changed:"RESET" %02i:%02i\n",
mpd_status_get_elapsed_song_time(mi)/60,
mpd_status_get_elapsed_song_time(mi)%60);
*/  }
    if(what&MPD_CST_PERMISSION){
        printf( "Permission:"" Changed\n");
    }
}

void main_mpd_cli(thread_data* my_data )
{
    blockQueue char_queue; // kolejka polecen

    ////////////////////////////// LCD PART ///////////////////////
    my_data->mainLCD->set_print_song_state(0);
    ///////////////////////////////////////////////////////////////////
    int   iport = 6600;
    char *hostname = getenv("MPD_HOST");
    char *port = getenv("MPD_PORT");
    char *password = getenv("MPD_PASSWORD");
    MpdObj *obj = NULL;

    std::cout << " adres hosta to " << hostname << std::endl;
    if(hostname == NULL) {
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
    mpd_signal_connect_status_changed(obj,(StatusChangedCallback)status_changed , my_data );
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
        my_data->myEventHandler.run("mpd")->addEvent("restart MPD");
        work = mpd_connect(obj);
    }

    std::cout << " stop "<<std::endl;

    if(!work)
    {
        char buffer;
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
                    mpd_player_pause(obj);
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
                case '%':
                    mpd_status_set_volume(obj, my_data->ptr_MPD_info->volume);
                    break;
                case '2':
                    debug_level = (debug_level > 0)?0:3;
                    printf( "Debug:"" %s\n", (debug_level >0)? "Enabled":"Disabled");
                    break;
                case 'I':
                    mpd_player_play_id(obj,my_data->ptr_MPD_info->currentSongID);
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

            mpd_status_update(obj);
            my_data->mainLCD->checkState();


            std::this_thread::sleep_for( std::chrono::milliseconds(500) );
        } while( useful_F::go_while);

        mpd_player_stop(obj); //wylaczanie mpd na koniec

    }
    else{
        std::cout << " NIE UDALO SIE POłączyć "<<std::endl;
    }
    mpd_free(obj);
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << " koniec watku klient MPD  "<<   std::endl;
    log_file_mutex.mutex_unlock();
}

void updatePlayList(MpdObj *mi,thread_data *my_data)
{
    {
        MpdData *data = mpd_playlist_get_changes(mi,-1);
        //my_data->ptr_MPD_info->songList ="";
        if(data)
        {
            //printf("Playlist:""\n");
            std::string buffor;
            my_data->ptr_MPD_info->songList.erase(my_data->ptr_MPD_info->songList.begin(),my_data->ptr_MPD_info->songList.end());
            do{
                if(data->type == MPD_DATA_TYPE_SONG)
                {
                    //printf("%i"": %s - %s\n", data->song->id, data->song->artist, data->song->title);
                    buffor = std::to_string(data->song->id)+" ";
                    if ( data->song->name != NULL){
                        buffor +=std::string(data->song->name)+" ";
                    }
                    if (data->song->artist != NULL){
                        buffor += std::string(data->song->artist)+" ";
                    }
                    if  ( data->song->title != NULL ){
                        buffor +=std::string(data->song->title)+" ";
                    }
                    my_data->ptr_MPD_info->songList.push_back(buffor);
                }
                data = mpd_data_get_next(data);
            }while(data);
        }
    }
}
