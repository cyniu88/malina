#include "mpd_cli.h"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32;06m"
#define YELLOW "\x1b[33;06m"
#define RED "\x1b[33;06m"
extern int debug_level;
void error_callback(MpdObj *mi,int errorid, char *msg, void *userdata)
{
        printf(RED"Error "RESET""GREEN"%i:"RESET" '%s'\n", errorid, msg);
}

void status_changed(MpdObj *mi, ChangedStatusType what)
{
        if(what&MPD_CST_SONGID)
        {
                mpd_Song *song = mpd_playlist_get_current_song(mi);
                if(song)
                {
                        printf(GREEN"Song:"RESET" %s - %s\n", song->artist, song->title);
                }
        }

        if(what&MPD_CST_STATE)
        {
                printf(GREEN"State:"RESET);
                switch(mpd_player_get_state(mi))
                {
                        case MPD_PLAYER_PLAY:
                                printf("Playing\n");
                                break;
                        case MPD_PLAYER_PAUSE:
                                printf("Paused\n");
                                break;
                        case MPD_PLAYER_STOP:
                                printf("Stopped\n");
                                break;
                        default:
                                break;
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
                mpd_Song *song = mpd_playlist_get_current_song(mi);

                   // std::cout <<" SONG: " << song->artist<<" "<< song->title << std::endl;
                printf(GREEN"aktualnie gramy:"RESET" %s - %s\n", song->artist, song->title);
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
                /*              printf(GREEN"Time elapsed changed:"RESET" %02i:%02i\n",
                                mpd_status_get_elapsed_song_time(mi)/60,
                                mpd_status_get_elapsed_song_time(mi)%60);
                                */      }
                if(what&MPD_CST_PERMISSION){
                        printf(YELLOW"Permission:"RESET" Changed\n");
                }
}

void  *main_mpd_cli(void *data )
{       blockQueue char_queue; // kolejka polecen
        thread_data  *my_data;
        my_data = (thread_data*)data;

        //int fdstdin = 0;
        int run = 1, iport = 6600;
        char *hostname = getenv("MPD_HOST");
        char *port = getenv("MPD_PORT");
        char *password = getenv("MPD_PASSWORD");
        MpdObj *obj = NULL;
        /* Make the input non blocking */
       // fdstdin = open("/dev/stdin", O_NONBLOCK|O_RDONLY);
        /* set correct hostname */
        std::cout << " adres hosta to " << hostname << std::endl;
        if(!hostname) {
                hostname = "localhost";

              std::cout << " ip mpd to " <<  my_data->server_settings->MPD_IP << " ! \n";
                hostname = (char*)my_data->server_settings->MPD_IP.c_str();
        }
        if(port){
                iport = atoi(port);
        }
        /* Create mpd object */
        obj = mpd_new(hostname, iport,password);

        /* Connect signals */
        mpd_signal_connect_error(obj,(ErrorCallback)error_callback, NULL);
        mpd_signal_connect_status_changed(obj,(StatusChangedCallback)status_changed, NULL);
        /* Set timeout */
        mpd_set_connection_timeout(obj, 10);

        if(!mpd_connect(obj))
        {
                char buffer;
                mpd_send_password(obj);
                //memset(buffer, '\0', 20);
                do{

                      if(char_queue._size() > 0)
                        {
                          digitalWrite(LED7,1);
                            buffer = char_queue._get();
                                switch(buffer)
                                {
                                        case '\n':
                                                break;
                                        case 'b':
                                                mpd_player_next(obj);
                                                break;
                                        case 'z':
                                                mpd_player_prev(obj);
                                                break;
                                        case 'x':
                                                mpd_player_play(obj);
                                                break;
                                        case 'c':
                                                mpd_player_pause(obj);
                                                break;
                                        case 'v':
                                                mpd_player_stop(obj);
                                                break;
                                        case 'q':
                                                run = 0;
                                                printf("Quitting....\n");
                                                break;
                                        case 'r':
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
                                /*    if(char_queue._size() > 0)
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
                                digitalWrite(LED7,0);  // gasze sygnal odbioru wiadomosci

                        }
   if (!mpd_check_connected(obj))
    {
        printf("not connected\n");
        mpd_connect(obj);
        
    }
//std::cout << " \n\nprzed if \n\n";
                      mpd_status_update(obj);
//std::cout << " \n\npo if \n\n";
                      //  std::cout << " jestem w while \n";
                }while(!usleep(100000) &&  go_while);
        }
        mpd_free(obj);
            log_file_mutex.mutex_lock();
            log_file_cout << INFO << " koniec watku klient MPD  "<<   std::endl;
            log_file_mutex.mutex_unlock();
       //close(fdstdin);


}

