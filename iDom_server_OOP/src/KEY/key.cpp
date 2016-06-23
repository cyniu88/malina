#include "key.h"




KEY::KEY(std::string name) : key_name{name}
{
std::cout << "konstruktor KEY" << std::endl;
}

KEY::~KEY()
{
    std::cout << "destruktor KEY" << std::endl;
}

std::string KEY::getName()
{
    return key_name;
}

map_key::map_key(std::map <std::string , std::unique_ptr <KEY>  > *key_map) : key_map (key_map)
{
    std::cout << "konstruktor map_key"<<std::endl;
}
map_key::~map_key( )
{
    std::cout << "destruktor map_key"<<std::endl;
}


void pilot::setup(){
    /*
    std::unique_ptr <KEY> power_off ( new KEY("POWER_OFF"));
    key_map->insert(std::make_pair(power_off->getName(), std::move(power_off)) );
    std::unique_ptr <KEY> volume_up ( new KEY("VOLUME_UP"));
    key_map->insert(std::make_pair(volume_up->getName(), std::move(volume_up)) );

    std::unique_ptr <KEY> key_0 ( new KEY("KEY_0"));
    key_map->insert(std::make_pair(key_0->getName(), std::move(key_0)) );
    std::unique_ptr <KEY> key_1 ( new KEY("KEY_1"));
    key_map->insert(std::make_pair(key_1->getName(), std::move(key_1)) );
    */
    std::unique_ptr <KEY> KEY_POWER 	( new KEY( "KEY_POWER"));
    std::unique_ptr <KEY> KEY_0   		( new KEY( "KEY_0"));
    std::unique_ptr <KEY> KEY_1  		( new KEY( "KEY_1"));
    std::unique_ptr <KEY> KEY_2   		( new KEY( "KEY_2"));
    std::unique_ptr <KEY> KEY_3   		( new KEY( "KEY_3"));
    std::unique_ptr <KEY> KEY_4   		( new KEY( "KEY_4"));
    std::unique_ptr <KEY> KEY_5   		( new KEY( "KEY_5"));
    std::unique_ptr <KEY> KEY_6   		( new KEY( "KEY_6"));
    std::unique_ptr <KEY> KEY_7   		( new KEY( "KEY_7"));
    std::unique_ptr <KEY> KEY_8   		( new KEY( "KEY_8"));
    std::unique_ptr <KEY> KEY_9   		( new KEY( "KEY_9"));
    std::unique_ptr <KEY> KEY_AUDIO		( new KEY( "KEY_AUDIO"))	;
    std::unique_ptr <KEY> KEY_CHANNELDOWN( new KEY( "KEY_CHANNELDOWN"));
    std::unique_ptr <KEY> KEY_CHANNELUP	( new KEY( "KEY_CHANNELUP"));
    std::unique_ptr <KEY> KEY_DOWN    	( new KEY( "KEY_DOWN"));
    std::unique_ptr <KEY> KEY_EPG     	( new KEY( "KEY_EPG"));
    std::unique_ptr <KEY> KEY_EXIT    	( new KEY( "KEY_EXIT"));
    std::unique_ptr <KEY> KEY_FAVORITES	( new KEY( "KEY_FAVORITES"));
    std::unique_ptr <KEY> KEY_INFO    	( new KEY( "KEY_INFO"));
    std::unique_ptr <KEY> KEY_LANGUAGE	( new KEY( "KEY_LANGUAGE"));
    std::unique_ptr <KEY> KEY_MENU    	( new KEY( "KEY_MENU"));
    std::unique_ptr <KEY> KEY_MUTE    	( new KEY( "KEY_MUTE"));
    std::unique_ptr <KEY> KEY_OK      	( new KEY( "KEY_OK"));
    std::unique_ptr <KEY> KEY_RADIO   	( new KEY( "KEY_RADIO"));
    std::unique_ptr <KEY> KEY_REFRESH 	( new KEY( "KEY_REFRESH"));
    std::unique_ptr <KEY> KEY_SAT     	( new KEY( "KEY_SAT"));
    std::unique_ptr <KEY> KEY_SUBTITLE	( new KEY( "KEY_SUBTITLE"));
    std::unique_ptr <KEY> KEY_TEXT    	( new KEY( "KEY_TEXT"));
    std::unique_ptr <KEY> KEY_TV      	( new KEY( "KEY_TV"));
    std::unique_ptr <KEY> KEY_UP      	( new KEY( "KEY_UP"));
    std::unique_ptr <KEY> KEY_VOLUMEDOWN	( new KEY( "KEY_VOLUMEDOWN"));
    std::unique_ptr <KEY> KEY_VOLUMEUP	( new KEY( "KEY_VOLUMEUP"));

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
    key_map->insert(std::make_pair(KEY_AUDIO->getName(), 	 std::move(KEY_AUDIO)));
    key_map->insert(std::make_pair(KEY_CHANNELDOWN->getName(), 	 std::move(KEY_CHANNELDOWN)));
    key_map->insert(std::make_pair(KEY_CHANNELUP->getName(), 	 std::move(KEY_CHANNELUP)));
    key_map->insert(std::make_pair(KEY_DOWN->getName(), 	 std::move(KEY_DOWN)));
    key_map->insert(std::make_pair(KEY_EPG->getName(), 	 std::move(KEY_EPG)));
    key_map->insert(std::make_pair(KEY_EXIT->getName(), 	 std::move(KEY_EXIT)));
    key_map->insert(std::make_pair(KEY_FAVORITES->getName(), 	 std::move(KEY_FAVORITES)));
    key_map->insert(std::make_pair(KEY_INFO->getName(), 	 std::move(KEY_INFO)));
    key_map->insert(std::make_pair(KEY_LANGUAGE->getName(), 	 std::move(KEY_LANGUAGE)));
    key_map->insert(std::make_pair(KEY_MENU->getName(), 	 std::move(KEY_MENU)));
    key_map->insert(std::make_pair(KEY_MUTE->getName(), 	 std::move(KEY_MUTE)));
    key_map->insert(std::make_pair(KEY_OK->getName(), 	 std::move(KEY_OK)));
    key_map->insert(std::make_pair(KEY_RADIO->getName(), 	 std::move(KEY_RADIO)));
    key_map->insert(std::make_pair(KEY_REFRESH->getName(), 	 std::move(KEY_REFRESH)));
    key_map->insert(std::make_pair(KEY_SAT->getName(), 	 std::move(KEY_SAT)));
    key_map->insert(std::make_pair(KEY_SUBTITLE->getName(), 	 std::move(KEY_SUBTITLE)));
    key_map->insert(std::make_pair(KEY_TEXT->getName(), 	 std::move(KEY_TEXT)));
    key_map->insert(std::make_pair(KEY_TV->getName(), 	 std::move(KEY_TV)));
    key_map->insert(std::make_pair(KEY_UP->getName(), 	 std::move(KEY_UP)));
    key_map->insert(std::make_pair(KEY_VOLUMEDOWN->getName(), 	 std::move(KEY_VOLUMEDOWN)));
    key_map->insert(std::make_pair(KEY_VOLUMEUP->getName(), 	 std::move(KEY_VOLUMEUP)));

}

void map_key::addToMap (std::unique_ptr <KEY> keyPtr){
    key_map->insert(std::make_pair(keyPtr->getName(), std::move(keyPtr)) );
}


pilot::pilot(std::map <std::string , std::unique_ptr <KEY>  > *key_map) : key_map (key_map)
{
    std::cout << "konstruktor pilot"<<std::endl;
}
pilot::~pilot( )
{
    std::cout << "destruktor pilot"<<std::endl;
}
