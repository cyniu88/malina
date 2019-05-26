#include "key.h"
#include "../iDom_server_OOP.h"

KEY::KEY(PILOT_KEY value, const std::string& name) : _value{value},key_name{name}
{
    //std::cout << "konstruktor KEY" << std::endl;
}

KEY::KEY(const KEY &k): _value{k._value},key_name{k.key_name}
{

}

KEY::~KEY()
{
    //printf("destruktor KEY\n");
}

std::string KEY::getName() const
{
    return key_name;
}

PILOT_KEY KEY::getValue() const
{
    return _value;
}

void pilot::setup(){
    std::unique_ptr <KEY> KEY_POWER 		(new SuperKEY(PILOT_KEY::KEY_POWER, "KEY_POWER","wcisnieto POWER"));
    std::unique_ptr <KEY> KEY_AUDIO		(new SuperKEY(PILOT_KEY::KEY_AUDIO, "KEY_AUDIO","wcisnieto PAUSE"));
    std::unique_ptr <KEY> KEY_EPG     		(new SuperKEY(PILOT_KEY::KEY_EPG, "KEY_EPG","przegladanie katalogu z filmami"));
    std::unique_ptr <KEY> KEY_OK      		(new SuperKEY(PILOT_KEY::KEY_OK, "KEY_OK", "wcisnieto OK"));
    std::unique_ptr <KEY> KEY_RADIO   		(new SuperKEY(PILOT_KEY::KEY_RADIO, "KEY_RADIO","sterowanie projektorem"));
    std::unique_ptr <KEY> KEY_TV      		(new SuperKEY(PILOT_KEY::KEY_TV, "KEY_TV", "wcisnieto PLAY"));
    std::unique_ptr <KEY> KEY_0   		(new KEY(PILOT_KEY::KEY_0, "KEY_0"));
    std::unique_ptr <KEY> KEY_1  		(new KEY(PILOT_KEY::KEY_1, "KEY_1"));
    std::unique_ptr <KEY> KEY_2   		(new KEY(PILOT_KEY::KEY_2, "KEY_2"));
    std::unique_ptr <KEY> KEY_3   		(new KEY(PILOT_KEY::KEY_3, "KEY_3"));
    std::unique_ptr <KEY> KEY_4   		(new KEY(PILOT_KEY::KEY_4, "KEY_4"));
    std::unique_ptr <KEY> KEY_5   		(new KEY(PILOT_KEY::KEY_5, "KEY_5"));
    std::unique_ptr <KEY> KEY_6   		(new KEY(PILOT_KEY::KEY_6, "KEY_6"));
    std::unique_ptr <KEY> KEY_7   		(new KEY(PILOT_KEY::KEY_7, "KEY_7"));
    std::unique_ptr <KEY> KEY_8   		(new KEY(PILOT_KEY::KEY_8, "KEY_8"));
    std::unique_ptr <KEY> KEY_9   		(new KEY(PILOT_KEY::KEY_9, "KEY_9"));
    std::unique_ptr <KEY> KEY_CHANNELDOWN	(new KEY(PILOT_KEY::KEY_CHANNELDOWN, "KEY_CHANNELDOWN"));
    std::unique_ptr <KEY> KEY_CHANNELUP		(new KEY(PILOT_KEY::KEY_CHANNELUP, "KEY_CHANNELUP"));
    std::unique_ptr <KEY> KEY_DOWN    		(new KEY(PILOT_KEY::KEY_DOWN, "KEY_DOWN"));
    std::unique_ptr <KEY> KEY_EXIT    		(new KEY(PILOT_KEY::KEY_EXIT, "KEY_EXIT"));
    std::unique_ptr <KEY> KEY_FAVORITES		(new KEY(PILOT_KEY::KEY_FAVORITES, "KEY_FAVORITES"));
    std::unique_ptr <KEY> KEY_INFO    		(new KEY(PILOT_KEY::KEY_INFO, "KEY_INFO"));
    std::unique_ptr <KEY> KEY_LANGUAGE		(new KEY(PILOT_KEY::KEY_LANGUAGE, "KEY_LANGUAGE"));
    std::unique_ptr <KEY> KEY_MENU    		(new KEY(PILOT_KEY::KEY_MENU, "KEY_MENU"));
    std::unique_ptr <KEY> KEY_MUTE    		(new KEY(PILOT_KEY::KEY_MUTE, "KEY_MUTE"));
    std::unique_ptr <KEY> KEY_REFRESH 		(new KEY(PILOT_KEY::KEY_REFRESH, "KEY_REFRESH"));
    std::unique_ptr <KEY> KEY_SAT     		(new KEY(PILOT_KEY::KEY_SAT, "KEY_SAT"));
    std::unique_ptr <KEY> KEY_SUBTITLE		(new KEY(PILOT_KEY::KEY_SUBTITLE, "KEY_SUBTITLE"));
    std::unique_ptr <KEY> KEY_TEXT    		(new KEY(PILOT_KEY::KEY_TEXT, "KEY_TEXT"));
    std::unique_ptr <KEY> KEY_UP      		(new KEY(PILOT_KEY::KEY_UP, "KEY_UP"));
    std::unique_ptr <KEY> KEY_VOLUMEDOWN	(new KEY(PILOT_KEY::KEY_VOLUMEDOWN, "KEY_VOLUMEDOWN"));
    std::unique_ptr <KEY> KEY_VOLUMEUP          (new KEY(PILOT_KEY::KEY_VOLUMEUP, "KEY_VOLUMEUP"));

    key_map->insert(std::make_pair(KEY_POWER->getName(), 	 std::move(KEY_POWER)));
    key_map->insert(std::make_pair(KEY_0->getName(), 	 std::move(KEY_0)));
    key_map->insert(std::make_pair(KEY_1->getName(), 	 std::move(KEY_1)));
    key_map->insert(std::make_pair(KEY_2->getName(), 	 std::move(KEY_2)));
    key_map->insert(std::make_pair(KEY_3->getName(), 	 std::move(KEY_3)));
    key_map->insert(std::make_pair(KEY_4->getName(), 	 std::move(KEY_4)));
    key_map->insert(std::make_pair(KEY_5->getName(), 	 std::move(KEY_5)));
    key_map->insert(std::make_pair(KEY_6->getName(), 	 std::move(KEY_6)));
    key_map->insert(std::make_pair(KEY_7->getName(), 	 std::move(KEY_7)));
    key_map->insert(std::make_pair(KEY_8->getName(), 	 std::move(KEY_8)));
    key_map->insert(std::make_pair(KEY_9->getName(), 	 std::move(KEY_9)));
    key_map->insert(std::make_pair(KEY_AUDIO->getName(),         std::move(KEY_AUDIO)));
    key_map->insert(std::make_pair(KEY_CHANNELDOWN->getName(),   std::move(KEY_CHANNELDOWN)));
    key_map->insert(std::make_pair(KEY_CHANNELUP->getName(), 	 std::move(KEY_CHANNELUP)));
    key_map->insert(std::make_pair(KEY_DOWN->getName(),          std::move(KEY_DOWN)));
    key_map->insert(std::make_pair(KEY_EPG->getName(),           std::move(KEY_EPG)));
    key_map->insert(std::make_pair(KEY_EXIT->getName(),          std::move(KEY_EXIT)));
    key_map->insert(std::make_pair(KEY_FAVORITES->getName(), 	 std::move(KEY_FAVORITES)));
    key_map->insert(std::make_pair(KEY_INFO->getName(),          std::move(KEY_INFO)));
    key_map->insert(std::make_pair(KEY_LANGUAGE->getName(), 	 std::move(KEY_LANGUAGE)));
    key_map->insert(std::make_pair(KEY_MENU->getName(),          std::move(KEY_MENU)));
    key_map->insert(std::make_pair(KEY_MUTE->getName(),          std::move(KEY_MUTE)));
    key_map->insert(std::make_pair(KEY_OK->getName(),            std::move(KEY_OK)));
    key_map->insert(std::make_pair(KEY_RADIO->getName(),         std::move(KEY_RADIO)));
    key_map->insert(std::make_pair(KEY_REFRESH->getName(),       std::move(KEY_REFRESH)));
    key_map->insert(std::make_pair(KEY_SAT->getName(),           std::move(KEY_SAT)));
    key_map->insert(std::make_pair(KEY_SUBTITLE->getName(), 	 std::move(KEY_SUBTITLE)));
    key_map->insert(std::make_pair(KEY_TEXT->getName(),          std::move(KEY_TEXT)));
    key_map->insert(std::make_pair(KEY_TV->getName(),            std::move(KEY_TV)));
    key_map->insert(std::make_pair(KEY_UP->getName(),            std::move(KEY_UP)));
    key_map->insert(std::make_pair(KEY_VOLUMEDOWN->getName(), 	 std::move(KEY_VOLUMEDOWN)));
    key_map->insert(std::make_pair(KEY_VOLUMEUP->getName(), 	 std::move(KEY_VOLUMEUP)));
}

pilot::pilot(std::map <std::string , std::unique_ptr <KEY> > *key_map) : key_map (key_map)
{
   // puts("konstruktor pilot");
}
pilot::~pilot()
{
    //delete key_map->find("KEY_UP");
   // puts("destruktor pilot\n");
}

PILOT_KEY SuperKEY::getValue() const
{
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< " " << LogName << std::endl;
    log_file_mutex.mutex_unlock();
    return _value;
}

SuperKEY::SuperKEY (PILOT_KEY v, const std::string& n, const std::string& LogName) : KEY(v,n) , LogName(LogName)
{
    //puts("konstruktos SuperKEY =)");
}
SuperKEY::~SuperKEY() {
   // puts("destruktor SuperKEY \n");
}
