#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <time.h>
#include <netdb.h>
#include <chrono>

// przerobka  adresu na ip . //////////////////////////////////
bool useful_F::go_while = true;

std::string useful_F::conv_dns (std::string temp){

    int i;
    struct hostent * he;
    struct in_addr ** addr_list;
    std::string s_ip;

    if(( he = gethostbyname( temp.c_str() ) ) == NULL )
    {
        herror( "gethostbyname" );
        return "- 1";
    }
    // print information about this host:
    printf( "Official name is: %s\n", he->h_name );
    printf( "IP addresses: " );
    addr_list =( struct in_addr ** ) he->h_addr_list;

    for( i = 0; addr_list[ i ] != NULL; i++ )
    {
        printf( "%s ", inet_ntoa( * addr_list[ i ] ) );
        s_ip += inet_ntoa( * addr_list[ i ] );
    }
    printf( "\n" );
    return s_ip;
}

////////////////////////////////////////// not used now
///
///
/*
void ChangeEndianness(int32_t * value,int MAX_MSG_LEN_INT)
{
    if (__BYTE_ORDER == __BIG_ENDIAN)
    {

    }
    else if( __BYTE_ORDER == __LITTLE_ENDIAN)
    { //cout << "little-endian" << endl
        for (int i =0 ; i < MAX_MSG_LEN_INT ; ++i )
        {
            int32_t result = 0;
            result |= (value[i] & 0x000000FF) << 24;
            result |= (value[i] & 0x0000FF00) << 8;
            result |= (value[i] & 0x00FF0000) >> 8;
            result |= (value[i] & 0xFF000000) >> 24;

            value[i] = result;
        }
    }
}// ChangeEndianness

void binary(int val)
{    for (int i = 31; i >= 0; i--)
    {
        if ((i + 1) % 8 == 0)
            std::cout << ' ';
        std::cout << ((val >> i) % 2);
    }
     std::cout << std::endl;
}
*/
std::string useful_F::send_to_arduino_clock (thread_data *my_data_logic, std::string msg){

    while (go_while)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        C_connection::mutex_who.lock();

        if (my_data_logic->pointer.ptr_who[0] == iDomConst::FREE)
        {
            C_connection::mutex_buf.lock();
            my_data_logic->pointer.ptr_who[0]=iDomConst::CLOCK;
            my_data_logic->pointer.ptr_who[1]= pthread_self();
            buffer=msg;
            C_connection::mutex_buf.unlock();
            C_connection::mutex_who.unlock();
            break;
        }
        C_connection::mutex_who.unlock();
    }

    while (go_while)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        C_connection::mutex_who.lock();

        if (my_data_logic->pointer.ptr_who[0] == pthread_self())
        {
            C_connection::mutex_buf.lock();
            my_data_logic->pointer.ptr_who[0]=iDomConst::FREE;
            my_data_logic->pointer.ptr_who[1]= 0;
            msg=buffer;
            C_connection::mutex_buf.unlock();
            C_connection::mutex_who.unlock();
            break;
        }
        C_connection::mutex_who.unlock();
    }
    return msg;
} //end send_to_arduino_clock

std::string useful_F::send_to_arduino (thread_data *my_data_logic, std::string msg){

    while (go_while)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        C_connection::mutex_who.lock();
        if (my_data_logic->pointer.ptr_who[0] == iDomConst::FREE)
        {
            C_connection::mutex_buf.lock();
            my_data_logic->pointer.ptr_who[0]=iDomConst::RS232;
            my_data_logic->pointer.ptr_who[1]= pthread_self();
            buffer=msg;
            C_connection::mutex_buf.unlock();
            C_connection::mutex_who.unlock();
            break;
        }
        C_connection::mutex_who.unlock();
    }

    while (go_while)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        C_connection::mutex_who.lock();
        if (my_data_logic->pointer.ptr_who[0] == pthread_self())
        {
            C_connection::mutex_buf.lock();
            my_data_logic->pointer.ptr_who[0]=iDomConst::FREE;
            my_data_logic->pointer.ptr_who[1]= 0;
            msg=buffer;
            C_connection::mutex_buf.unlock();
            C_connection::mutex_who.unlock();
            break;
        }
        C_connection::mutex_who.unlock();
    }

    return msg;
} //end send_to_arduino

std::string  useful_F::sek_to_uptime(long long secy )
{
    const int min = 60; //s
    const int houry = 3600; //s
    const int day = 86400; //s
    int number_day, number_hour, number_min, number_sec;
    int temp1, temp2, temp3;
    number_day = secy / day;
    std::string text = "\n" +std::to_string( number_day )+ " day " ;
    temp1 = secy % day;
    number_hour = temp1 / houry;
    text += std::to_string( number_hour ) +" hours "  ;
    temp2 = temp1 % houry;
    number_min = temp2 / min;
    text += std::to_string(number_min)+" minutes "  ;
    temp3 = temp2 % min;
    number_sec = temp3;
    text += std::to_string( number_sec )+ " seconds " ;
    return text;
}

////// watek sleeper
void useful_F::sleeper_mpd (thread_data  *my_data)
{
    blockQueue char_queue; // kolejka polecen
    for (; my_data->sleeper >0 ; my_data->sleeper-- ){
        //sleep (60);
        std::this_thread::sleep_for( std::chrono::seconds(60) );
    }

    char_queue._add('P');
    send_to_arduino(my_data,"LED_STOP:22;");
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "zaczynam procedure konca watku SLEEP_MPD" <<  std::endl;
    log_file_mutex.mutex_unlock();
    try {
        for (int i =0 ; i< iDomConst::MAX_CONNECTION;++i)
        {
            if (my_data->main_THREAD_arr[i].thread_ID == std::this_thread::get_id())
            {
                my_data->main_THREAD_arr[i].thread.detach();
                my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";
                my_data->main_THREAD_arr[i].thread_ID =  std::thread::id();
                break;
            }
        }
    }
    catch (std::system_error &e){
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR<< "zlapano wyjatek w  watku SLEEP_MPD: " << e.what()<< std::endl;
        log_file_mutex.mutex_unlock();
    }

    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< "koniec  watku SLEEP_MPD" <<  std::endl;
    log_file_mutex.mutex_unlock();

}
void useful_F::tokenizer ( std::vector <std::string> &command, std::string separator, std::string &text){
    std::string temp;

    for(char n: text) { // the initializer may be an array
        bool is_sep = false;
        for(char m: separator){
            
            if (n==m)
            {
                is_sep = true;
            }
        }
        
        if (is_sep== false){

            temp+=n;
        }
        else
        {
            if (!temp.empty())
            {
                command.push_back( temp);
                temp="";
            }
        }
    }
    if (!temp.empty()){
        command.push_back( temp);
    }
}

std::string useful_F::RSHash(int offset  )
{
    offset = 9; // because warning
    time_t act_time;
    struct tm * act_date;
    time(&act_time);
    act_date = localtime(&act_time);
    char buffer[10];
    strftime(buffer,10,"%M%H%w",act_date);
    std::string str(buffer);
    //std::cout<<str<<std::endl;
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < str.length(); i++)
    {
        hash = hash * a + str[i];
        a    = a * b;
    }
    return std::to_string((hash & 0x7FFFFFFF));
}

void useful_F::write_to_mkfifo(  std::string msg)
{
    int fd = open("/mnt/ramdisk/cmd", O_WRONLY);
    write(fd, msg.c_str(), msg.size());
    close(fd);
}


std::string useful_F::read_from_mkfifo()
{
    char buf[10];
    /* open, read, and display the message from the FIFO */
    int fd = open("/mnt/ramdisk/cmd", O_RDONLY);
    read(fd, buf, 10);
    close(fd);
    return (std::string(buf));
}

//wysylanie pliku
std::string useful_F::l_send_file(std::string path, std::string find  , bool reverse )
{
    std::string str_buf;
    if(find.empty()==true)
    {
        std::fstream log_file;
        log_file.open(path.c_str(),std::ios::in);
        if( log_file.good() == false )
        {
            return " can not open file !";
        }
        else
        {
            str_buf.erase();
            while( !log_file.eof() )
            {
                str_buf += log_file.get();
            }
            str_buf.erase(str_buf.length()-1, str_buf.length());

            log_file.close();
        }
    }
    else
    {
        std::fstream log_file;
        log_file.open(path.c_str(),std::ios::in);
        if( log_file.good() == false )
        {
            return " can not open file !";
        }
        else
        {
            str_buf.erase();
            std::string str_temp;
            while( std::getline(log_file,str_temp) )
            {
                if (reverse){
                    if(std::string::npos!=str_temp.find(find)){
                        str_buf+=str_temp +"\n";
                    }
                    else{
                        if(str_buf.size()<3){
                            str_buf+="    ";
                        }
                    }
                }
                else{
                    if(std::string::npos == str_temp.find(find)){
                        str_buf+=str_temp+"\n";
                    }
                }
                if(str_buf.size()<3){
                    str_buf+="    ";
                }
            }
        }
        log_file.close();
    }
    return str_buf;
}

std::vector<std::string> useful_F::split(const std::string& s, char separator ){
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    return output;
}
thread_data* useful_F::myStaticData = NULL;
void useful_F::setStaticData(thread_data *my_dataPtr)
{
    myStaticData = my_dataPtr;
}

volatile unsigned int  useful_F::lastInterruptTime = 0;
std::mutex useful_F::mut;

void useful_F::button_interrupt( )
{
    static int counter = 0;
    counter++;
    //TODO delete counter it is not needed
    std::lock_guard <std::mutex > lock(useful_F::mut);
    volatile unsigned int m = millis();
    volatile auto a = m - useful_F::lastInterruptTime;
    if (a > 50){
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "przerwanie przycisku " <<counter<< std::endl;
        log_file_mutex.mutex_unlock();
        useful_F::myStaticData->myEventHandler.run("interrupt")->addEvent("przerwanie z przycisku");
        printf ("przerwanie %d - %d = %d\n", m, useful_F::lastInterruptTime, a);
        printf("counter %d \n status is %d\n",counter,digitalRead(iDomConst::BUTTON_PIN));

        if (digitalRead(iDomConst::BUTTON_PIN)==HIGH){
            //iDomTOOLS::playMPD(useful_F::myStaticData);
            unsigned int menuCounter = 0;
            while (digitalRead(iDomConst::BUTTON_PIN)==HIGH){
                menuCounter++;
                if(menuCounter==6){
                    useful_F::myStaticData->mainLCD->set_lcd_STATE(100);
                    useful_F::myStaticData->mainLCD->printString(true,0,0,"MUSIC");
                    puts("MUSIC");
                }
                if (menuCounter==60000000){
                    useful_F::myStaticData->mainLCD->set_lcd_STATE(100);
                    useful_F::myStaticData->mainLCD->printString(true,0,0,"LED");
                    puts("LED");
                }
                if (menuCounter==120000000){
                    useful_F::myStaticData->mainLCD->set_lcd_STATE(100);
                    useful_F::myStaticData->mainLCD->printString(true,0,0,"SERVER");
                    puts("SERVER");
                }
            }
        }
//        else{
//            useful_F::myStaticData->mainLCD->set_lcd_STATE(100);
//            useful_F::myStaticData->mainLCD->printString(true,0,0,"DONE");
//            puts("DONE");
//        }
        useful_F::lastInterruptTime = millis();
    }
}
