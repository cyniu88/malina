/*mapa klawiszy pilota i znaki im przypisane
KEY_POWER           P
KEY_0               0
KEY_1               1
KEY_2               2
KEY_3               3
KEY_4               4
KEY_5               5
KEY_6               6
KEY_7               7
KEY_8               8
KEY_9               9
KEY_AUDIO       	A
KEY_CHANNELDOWN     /
KEY_CHANNELUP       ^
KEY_DOWN            D
KEY_EPG             E
KEY_EXIT            e
KEY_FAVORITES       F
KEY_INFO            I
KEY_LANGUAGE        L
KEY_MENU        	M
KEY_MUTE        	m
KEY_OK              O
KEY_RADIO           r
KEY_REFRESH         R
KEY_SAT             s
KEY_SUBTITLE        S
KEY_TEXT            T
KEY_TV              t
KEY_UP              U
KEY_VOLUMEDOWN      -
KEY_VOLUMEUP        +

 */
#include "master_irda.h"
#include "../c_irda_logic/c_irda_logic.h"
master_irda::master_irda(thread_data *my_data):buttonTimer(millis()), buttonMENU(0), my_data2(my_data)
{
    if(  lirc_init(const_cast< char*>("lirc"),1)  ==  -1 )
        exit(EXIT_FAILURE);
    std::cout << " jestem po uruchominiu lirc \n";

    //// tu chyba sa glupoty  zle
}

void master_irda::run()
{
    c_irda_logic irda_queue(my_data2);
    //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
    if(lirc_readconfig(NULL,&config,NULL)==0)
    {
        // std::cout << " jestem w iflirc_readconfig(NULL,&config,NULL)==0 \n ";
        //Do stuff while LIRC socket is open  0=open  -1=closed.
        while(lirc_nextcode(&code )==0 )
        {
            std::cout << " w while \n";
            if (go_while==false)     /// TO NIE DZIALA DO POPRAWY
            {
                break;  // koncze petle zamykam wątek
            }
            //If code = NULL, meaning nothing was returned from LIRC socket,
            //then skip lines below and start while loop again.
            if(code==NULL) {
                std::cout << " kontynuuje\n";
                continue;
            }
            else
            {
                //Make sure there is a 400ms gap before detecting button presses.
                if (millis() - buttonTimer  > 400 ){

                    // time out   OK menu
                    if (millis() - buttonTimer  > 30000 && buttonMENU == 1){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " timeout menu " <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        buttonMENU=0;  // flaga na 0  nie wejdze juz do timeout
                        irda_queue._add('e');
                    }
                    //////////////////////////////////////////////////////////////
                    //Check to see if the string "KEY_1" appears anywhere within the string 'code'.


                    CodeString= code;

                    CodeString.erase(0,  ( CodeString.find("KEY"))) ;
                    CodeString.erase(  CodeString.find(" "), CodeString.size());


                    my_data2->myEventHandler.run("pilot")->addEvent("press: "+CodeString);
                    irda_queue._add( my_data2->key_map[CodeString]->getValue() );
                    buttonTimer = millis();

                      if(strstr (code,"KEY_EPG")){
                        buttonMENU=1;
                    }
                    else if(strstr (code,"KEY_OK")){
                        buttonMENU=0;
                    }
                    else if(strstr (code,"KEY_MENU")){
                        buttonMENU=1;// wlacz timeout dla menu
                    }
                    else if(strstr (code,"KEY_EXIT")){
                        buttonMENU = 0;
                    }
                }
            }
            //Need to free up code before the next loop
            free(code);
        }
        //Frees the data structures associated with config.
        lirc_freeconfig(config);
    }
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << " koniec watku master IRDA  "<<   std::endl;
    log_file_mutex.mutex_unlock();

    //lirc_deinit() closes the connection to lircd and does some internal clean-up stuff.
    lirc_deinit();
    // exit(EXIT_SUCCESS);
}
