#include <sstream>

#include "mpd_cli.h"
#include "../TASKER/tasker.h"
#include "../thread_functions/iDom_thread.h"

using namespace std::chrono_literals;

extern int debug_level;
bool check_title_song_to = false;

void error_callback(MpdObj *mi, int errorid, char *msg, void *userdata)
{
    printf("Error "
           ""
           "%i:"
           " '%s'\n",
           errorid, msg);
}

void status_changed(MpdObj *mi, ChangedStatusType what, thread_data *my_data)
{
    if (what & MPD_CST_SONGID)
    {
        mpd_Song *song = mpd_playlist_get_current_song(mi);
        if (song)
        {
            try
            {
                printf("Song:"
                       " %s - %s\n",
                       song->artist, song->title);
                std::stringstream msg;
                msg << " ";
                msg << (song->title);
                my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/songID", msg.str());
            }
            catch (...)
            {
                puts("error cyniu1");
            }
        }
    }

    if (what & MPD_CST_REPEAT)
    {
        printf("Repeat:"
               " %s\n",
               mpd_player_get_repeat(mi) ? "On" : "Off");
    }
    if (what & MPD_CST_RANDOM)
    {
        printf("Random:"
               " %s\n",
               mpd_player_get_random(mi) ? "On" : "Off");
    }
    if (what & MPD_CST_VOLUME)
    {
        printf("Volume:"
               " %03i%%\n",
               mpd_status_get_volume(mi));

        try
        {
            my_data->ptr_MPD_info->volume = mpd_status_get_volume(mi);
            my_data->main_key_menu_handler->quickPrint("Volume:", std::to_string(my_data->ptr_MPD_info->volume));
            my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/volume",
                                          std::to_string(my_data->ptr_MPD_info->volume));
        }
        catch (...)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "problem z wpisaniem volume " << std::endl;
            log_file_mutex.mutex_unlock();
        }
    }
    if (what & MPD_CST_CROSSFADE)
    {
        printf("X-Fade:"
               " %i sec.\n",
               mpd_status_get_crossfade(mi));
    }
    if (what & MPD_CST_UPDATING)
    {
        if (mpd_status_db_is_updating(mi))
        {
            printf("Started updating DB"
                   "\n");
        }
        else
        {
            printf("Updating DB finished"
                   "\n");
        }
    }
    if (what & MPD_CST_DATABASE)
    {
        printf("Databased changed"
               "\n");
    }
    if (what & MPD_CST_PLAYLIST)
    {
        if (check_title_song_to == true)
        {
            mpd_Song *song = mpd_playlist_get_current_song(mi);
            printf("aktualnie gramy:"
                   " %s - %s\n",
                   song->artist, song->title);

            std::stringstream msg;
            msg << " ";
            msg << song->title;
            my_data->mqttHandler->publish(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/songID", msg.str());

            my_data->ptr_MPD_info->title = msg.str();

            msg.clear();

            msg << " ";
            msg << song->artist;
            my_data->ptr_MPD_info->artist = msg.str();

            if (song->name != NULL)
            {
                _msg = song->name;

                my_data->ptr_MPD_info->radio = _msg;

                my_data->main_key_menu_handler->quickPrint("Radio:", _msg);

                updatePlayList(mi, my_data);
            }

            if (song->title != NULL)
            {
                _msg = song->title;
                if (_msg.size() < 7)
                {
                    _msg = song->name;
                    _msg += " -brak nazwy";
                }
            }
            else if (song->artist != NULL)
            {
                _msg = song->artist;
            }
            else
            {
                _msg += " -brak nazwy";
            }
            my_data->ptr_MPD_info->title = _msg;
            my_data->main_key_menu_handler->quickPrint(_msg, "");
        }
    }
    if (what & MPD_CST_STATE)
    {
        printf("State:");
        switch (mpd_player_get_state(mi))
        {
        case MPD_PLAYER_PLAY:
            printf("Playing\n");
            my_data->mqttHandler->publishRetained(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/status", "PLAY");
            check_title_song_to = true;
            // digitalWrite(iDomConst::GPIO_SPIK, LOW);
            my_data->main_iDomTools->turnOnSpeakers();
            updatePlayList(mi, my_data);
            my_data->myEventHandler.run("mpd")->addEvent("MPD playing");
            my_data->main_iDomStatus->setObjectState("music", STATE::PLAY);
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            break;
        case MPD_PLAYER_PAUSE:
            printf("Paused\n");
            my_data->mqttHandler->publishRetained(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/status", "PAUSE");
            my_data->main_key_menu_handler->quickPrint("MPD", "PAUZA");
            my_data->myEventHandler.run("mpd")->addEvent("MPD pause");
            my_data->main_iDomStatus->setObjectState("music", STATE::PAUSE);
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            break;
        case MPD_PLAYER_STOP:
            printf("Stopped\n");
            my_data->mqttHandler->publishRetained(my_data->server_settings->_mqtt_broker.topicPublish + "/mpd/status", "STOP");

            if (my_data->ptr_MPD_info->isPlay == true)
            {
                my_data->main_iDomTools->ledClear();
            }
            check_title_song_to = false;
            my_data->ptr_MPD_info->isPlay = false;
            my_data->ptr_MPD_info->title = "* * * *";
            my_data->main_iDomTools->turnOffSpeakers();
            // digitalWrite(iDomConst::GPIO_SPIK,HIGH);
            my_data->main_key_menu_handler->recKeyEvent(KEY_PAD::OFF_LCD);
            sleep(1);
            my_data->myEventHandler.run("mpd")->addEvent("MPD stopped");
            my_data->main_iDomStatus->setObjectState("music", STATE::STOP);
            my_data->main_iDomTools->saveState_iDom(my_data->serverStarted);
            break;
        default:
            break;
        }
    }
    /* not yet implemented signals */
    if (what & MPD_CST_AUDIO)
    {
        printf("Audio Changed"
               "\n");
    }
    if (what & MPD_CST_TOTAL_TIME)
    {
        printf("Total song time changed:"
               " %02i:%02i\n",
               mpd_status_get_total_song_time(mi) / 60,
               mpd_status_get_total_song_time(mi) % 60);
    }
    if (what & MPD_CST_ELAPSED_TIME)
    {
        /* printf(GREEN"Time elapsed changed:"RESET" %02i:%02i\n",
mpd_status_get_elapsed_song_time(mi)/60,
mpd_status_get_elapsed_song_time(mi)%60);
*/ }
        if (what & MPD_CST_PERMISSION)
        {
            printf("Permission:"
                   " Changed\n");
        }
}

void main_mpd_cli(thread_data *my_data, const std::string &threadName)
{
        blockQueue mpdQueue; // kolejka polecen

        ///////////////////////////////////////////////////////////////////
        int iport = 6600;
        char *hostname = getenv("MPD_HOST");
        char *port = getenv("MPD_PORT");
        char *password = getenv("MPD_PASSWORD");
        MpdObj *obj = NULL;

        // std::cout << " adres hosta to " << hostname << std::endl;
        if (hostname == NULL)
        {
            // std::cout << " ip mpd to " << my_data->server_settings->MPD_IP << " ! \n";
            hostname = my_data->server_settings->_server.MPD_IP.data();
        }
        if (port)
        {
            iport = std::stoi(port);
        }
        /* Create mpd object */
        obj = mpd_new(hostname, iport, password);
        /* Connect signals */
        mpd_signal_connect_error(obj, (ErrorCallback)error_callback, NULL);
        mpd_signal_connect_status_changed(obj, (StatusChangedCallback)status_changed, my_data);
        /* Set timeout */
        mpd_set_connection_timeout(obj, 10);

        int work;
        work = mpd_connect(obj);

        while (work != MPD_OK)
        {
            log_file_mutex.mutex_lock();
            log_file_cout << ERROR << "nie udalo sie polaczyc z MPD " << std::endl;
            log_file_mutex.mutex_unlock();
            system("service mpd stop");
            sleep(1);
            system("service mpd start");
            log_file_mutex.mutex_lock();
            log_file_cout << INFO << "restart MPD " << std::endl;
            log_file_cout << INFO << "nawiazuje nowe polaczenie z MPD " << std::endl;
            log_file_mutex.mutex_unlock();
            my_data->myEventHandler.run("mpd")->addEvent("restart MPD");
            work = mpd_connect(obj);
        }

        std::cout << " stop " << std::endl;

        if (work == MPD_OK)
        {
            MPD_COMMAND buffer_tmp;
            do
            {
                if (mpdQueue._size() > 0)
                {
                    // digitalWrite(LED7,1);
                    buffer_tmp = mpdQueue._get();
                    switch (buffer_tmp)
                    {
                    case MPD_COMMAND::NEXT:
                        mpd_player_next(obj);
                        break;
                    case MPD_COMMAND::PREV:
                        mpd_player_prev(obj);
                        break;
                    case MPD_COMMAND::PLAY:
                        mpd_player_play(obj);
                        break;
                    case MPD_COMMAND::PAUSE:
                        mpd_player_pause(obj);
                        break;
                    case MPD_COMMAND::STOP:
                        mpd_player_pause(obj);
                        mpd_player_stop(obj);
                        break;
                    case MPD_COMMAND::REPEATE:
                        mpd_player_set_repeat(obj, !mpd_player_get_repeat(obj));
                        break;
                    case MPD_COMMAND::RANDOM:
                        mpd_player_set_random(obj, !mpd_player_get_random(obj));
                        break;
                    case MPD_COMMAND::VOLUP:
                        mpd_status_set_volume(obj, mpd_status_get_volume(obj) + 1);
                        break;
                    case MPD_COMMAND::VOLDOWN:
                        mpd_status_set_volume(obj, mpd_status_get_volume(obj) - 1);
                        break;
                    case MPD_COMMAND::VOLSET:
                        mpd_status_set_volume(obj, my_data->ptr_MPD_info->volume);
                        break;
                    case MPD_COMMAND::DEBUG:
                        debug_level = (debug_level > 0) ? 0 : 3;
                        printf("Debug:"
                               " %s\n",
                               (debug_level > 0) ? "Enabled" : "Disabled");
                        break;
                    case MPD_COMMAND::PLAY_ID:
                        mpd_player_play_id(obj, my_data->ptr_MPD_info->currentSongID);
                        break;
                    default:
                        printf("buffer: %c\n", buffer_tmp);
                    }
                }
                if (!mpd_check_connected(obj))
                {
                    log_file_mutex.mutex_lock();
                    log_file_cout << WARNING << "utracono polacznie z MPD " << std::endl;
                    log_file_cout << INFO << "restart MPD" << std::endl;
                    log_file_mutex.mutex_unlock();
                    std::this_thread::sleep_for(1s);
                    system("service mpd restart");
                    mpd_connect(obj);
                }

                mpd_status_update(obj);

                std::this_thread::sleep_for(0.5s);

                my_data->main_key_menu_handler->scrollText();
            } while (useful_F::go_while);

            // mpd_player_stop(obj); //wylaczanie mpd na koniec
        }
        else
        {
            std::cout << " NIE UDALO SIE POłączyć " << std::endl;
        }
        mpd_free(obj);
        iDOM_THREAD::stop_thread(threadName, my_data);
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << " koniec " << threadName << std::endl;
        log_file_mutex.mutex_unlock();
}

void updatePlayList(MpdObj *mi, thread_data *my_data)
{
        {
            MpdData *data = mpd_playlist_get_changes(mi, -1);
            // my_data->ptr_MPD_info->songList ="";
            if (data)
            {
                // printf("Playlist:""\n");
                std::string buffor;
                my_data->ptr_MPD_info->songList.erase(my_data->ptr_MPD_info->songList.begin(), my_data->ptr_MPD_info->songList.end());
                do
                {
                    if (data->type == MPD_DATA_TYPE_SONG)
                    {
                        // printf("%i"": %s - %s\n", data->song->id, data->song->artist, data->song->title);
                        buffor = std::to_string(data->song->id) + " ";
                        if (data->song->name != NULL)
                        {
                            buffor += std::string(data->song->name) + " ";
                        }
                        //                    if (data->song->artist != NULL){
                        //                        buffor += std::string(data->song->artist)+" ";
                        //                    }
                        //                    if  ( data->song->title != NULL ){
                        //                        buffor +=std::string(data->song->title)+" ";
                        //                    }
                        my_data->ptr_MPD_info->songList.push_back(buffor);
                    }
                    data = mpd_data_get_next(data);
                } while (data);
            }
        }
}
