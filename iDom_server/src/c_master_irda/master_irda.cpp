
#include "master_irda.h"
#include <time.h>
#include <wiringPi.h>
master_irda::master_irda(thread_data *my_data)
{
    buttonTimer = millis();
    buttonMENU = 0;
    if (wiringPiSetup () == -1)
        exit (1) ;
    //Initiate LIRC. Exit on failure
    if(lirc_init("lirc",1)==-1)
        exit(EXIT_FAILURE);
    std::cout << " jestem po uruchominiu lirc \n";

    //// tu chyba sa glupoty  zle


}
void master_irda::setup ()
{


}

void master_irda::run()
{
    //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
    if(lirc_readconfig(NULL,&config,NULL)==0)
    {
       // std::cout << " jestem w iflirc_readconfig(NULL,&config,NULL)==0 \n ";
        //Do stuff while LIRC socket is open  0=open  -1=closed.
        while(lirc_nextcode(&code)==0)
        {
            std::cout << " w while \n";
            //If code = NULL, meaning nothing was returned from LIRC socket,
            //then skip lines below and start while loop again.
            if(code==NULL) {
                // std::cout << " kontynuuje\n";
                continue;}
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

                    if(strstr (code,"KEY_POWER")){
                        //printf("MATCH on KEY_POWER\n");
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto POWER" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('v');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_1")){
                        irda_queue._add('1');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_2")){
                        irda_queue._add('2');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_3")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('3');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_4")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('4');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_5")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('5');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_6")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('6');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_7")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('7');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_8")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('8');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_9")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('9');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_0")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('0');
                        buttonTimer = millis();
                    }
                    ////////////////////////////////////
                    else if(strstr (code,"KEY_INFO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " operacja na diodzie" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('i');
                        buttonTimer = millis();
                    }
                    /////////////////////////////////////////////////
                    else if(strstr (code,"KEY_RADIO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto PLAY" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('x');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_AUDIO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto PAUSE" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('c');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_VOLUMEUP")){
                        //printf("MATCH on KEY_VOLUMEUP\n");
                        // system("mpc -h 192.168.1.144 volume +1");
                        irda_queue._add('+');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_VOLUMEDOWN")){
                        //printf("MATCH on KEY_VOLUMEDOWN\n");
                        // system("mpc -h 192.168.1.144 volume -1");
                        irda_queue._add('-');
                        buttonTimer = millis();
                    }
                    /////////////////////////////////////////////

                    else if(strstr (code,"KEY_DOWN")){
                        //////printf("MATCH on KEY_DOWN\n");
                        //system("mpc -h 192.168.1.144 next");
                        irda_queue._add('b');
                        buttonTimer = millis();
                    }

                    else if(strstr (code,"KEY_UP")){
                        //////printf("MATCH on KEY_UP\n");
                        //system("mpc -h 192.168.1.144 prev");
                        irda_queue._add('z');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_OK")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('o');
                        buttonMENU=0;
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_MENU")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('m');
                        buttonMENU=1;// wlacz timeout dla menu
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_EXIT")){
                        //////printf("MATCH on KEY_3\n");
                        irda_queue._add('e');
                        buttonMENU = 0;
                        buttonTimer = millis();
                    }
                }
            }
            //Need to free up code before the next loop
            free(code);
        }
        //Frees the data structures associated with config.
        lirc_freeconfig(config);
    }
    //lirc_deinit() closes the connection to lircd and does some internal clean-up stuff.
    lirc_deinit();
    exit(EXIT_SUCCESS);

}
