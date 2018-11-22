#include "command_program.h"
#include "../../functions/functions.h"
#include "../../433MHz/RFLink/rflinkhandler.h"

command_program::command_program(const std::string &name):command(name)
{
}

std::string command_program::execute(std::vector<std::string> &v, thread_data *my_data)
{
    std::string ret = help();
    if (v.size() <2 )
    {
        return "what?\n" + help();;
    }
    if (v[1] == "stop")
    {
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "STOP");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::CLOSE;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    if (v.size() < 3 )
    {
        return "add more paramiters";
    }
    if(v[1] == "reload" && v[2] == "soft"){
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "RELO");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::RELOAD;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    else if(v[1] == "reload" && v[2] == "hard")
    {
        std::string s ="close server";
        useful_F::send_to_arduino_clock(my_data, "UPAD");
        iDomTOOLS::MPD_stop();
        my_data->iDomProgramState = iDomStateEnum::HARD_RELOAD;
        my_data->main_iDomTools->saveState_iDom();
        throw s;
    }
    else if(v[1] == "clear" && v[2] == "ram")
    {
        system("sync; echo 3 > /proc/sys/vm/drop_caches");
        ret = "ram has beed freed";
    }
    else if(v[1] == "debuge" && v[2] == "variable")
    {
        std::stringstream r;
        r << "my_data->alarmTime.fromVolume \t" << my_data->alarmTime.fromVolume <<std::endl;
        r << "my_data->alarmTime.radioID \t" << my_data->alarmTime.radioID <<std::endl;
        r << "my_data->alarmTime.state \t" << stateToString( my_data->alarmTime.state) <<std::endl;
        r << "my_data->alarmTime.time \t" << my_data->alarmTime.time.getString() <<std::endl;
        r << "my_data->alarmTime.toVolume \t" << my_data->alarmTime.toVolume <<std::endl;
        r << std::endl;
        r << "my_data->encriptionKey \t" << my_data->encriptionKey <<std::endl;
        r << std::endl;
        r << "my_data->server_settings->BaudRate \t" << my_data->server_settings->BaudRate <<std::endl;
        r << "my_data->server_settings->cameraLedOFF \t" << my_data->server_settings->cameraLedOFF <<std::endl;
        r << "my_data->server_settings->cameraLedON \t" << my_data->server_settings->cameraLedON <<std::endl;
        r << "my_data->server_settings->cameraURL \t" << my_data->server_settings->cameraURL <<std::endl;
        r << "my_data->server_settings->encrypted \t" << my_data->server_settings->encrypted <<std::endl;
        r << "my_data->server_settings->facebookAccessToken \t" << my_data->server_settings->facebookAccessToken <<std::endl;
        r << "my_data->server_settings->ftpServer.URL \t" << my_data->server_settings->ftpServer.URL <<std::endl;
        r << "my_data->server_settings->ftpServer.user \t" << my_data->server_settings->ftpServer.user <<std::endl;
        r << "my_data->server_settings->ID_server \t" << my_data->server_settings->ID_server <<std::endl;
        r << "my_data->server_settings->lightningApiURL \t" << my_data->server_settings->lightningApiURL <<std::endl;
        r << "my_data->server_settings->MENU_PATH \t" << my_data->server_settings->MENU_PATH<<std::endl;
        r << "my_data->server_settings->MOVIES_DB_PATH \t" << my_data->server_settings->MOVIES_DB_PATH<<std::endl;
        r << "my_data->server_settings->MPD_IP \t" << my_data->server_settings->MPD_IP<<std::endl;
        r << "my_data->server_settings->omxplayerFile \t" << my_data->server_settings->omxplayerFile<<std::endl;
        r << "my_data->server_settings->PORT \t" << my_data->server_settings->PORT<<std::endl;
        r << "my_data->server_settings->portRS232 \t" << my_data->server_settings->portRS232<<std::endl;
        r << "my_data->server_settings->portRS232_clock \t" << my_data->server_settings->portRS232_clock<<std::endl;
        r << "my_data->server_settings->radio433MHzConfigFile \t" << my_data->server_settings->radio433MHzConfigFile<<std::endl;
        r << "my_data->server_settings->RFLinkBaudRate \t" << my_data->server_settings->RFLinkBaudRate<<std::endl;
        r << "my_data->server_settings->RFLinkPort \t" << my_data->server_settings->RFLinkPort<<std::endl;
        r << "my_data->server_settings->saveFilePath \t" << my_data->server_settings->saveFilePath<<std::endl;
        r << "my_data->server_settings->SERVER_IP \t" << my_data->server_settings->SERVER_IP<<std::endl;
        r << "my_data->server_settings->THREAD_CRON \t" << my_data->server_settings->THREAD_CRON<<std::endl;
        r << "my_data->server_settings->THREAD_DUMMY \t" << my_data->server_settings->THREAD_DUMMY<<std::endl;
        r << "my_data->server_settings->THREAD_IRDA \t" << my_data->server_settings->THREAD_IRDA<<std::endl;
        r << "my_data->server_settings->THREAD_MPD \t" << my_data->server_settings->THREAD_MPD<<std::endl;
        r << "my_data->server_settings->THREAD_RS232 \t" << my_data->server_settings->THREAD_RS232<<std::endl;
        r << "my_data->server_settings->TS_KEY \t" << my_data->server_settings->TS_KEY<<std::endl;
        r << "my_data->server_settings->viberAvatar \t" << my_data->server_settings->viberAvatar<<std::endl;
        r << "my_data->server_settings->viberReceiver.at(0) \t" << my_data->server_settings->viberReceiver.at(0)<<std::endl;
        r << "my_data->server_settings->viberSender \t" << my_data->server_settings->viberSender<<std::endl;
        r << "my_data->server_settings->viberToken \t" << my_data->server_settings->viberToken<<std::endl;
        r << "my_data->server_settings->v_delay \t" << my_data->server_settings->v_delay<<std::endl;
        r << std::endl;
        r << "my_data->server_settings->sleeper \t" << my_data->sleeper<<std::endl;
        r << std::endl;
        r << "my_data->iDomProgramState \t" << static_cast<int>(my_data->iDomProgramState)<<std::endl;
        r << std::endl;
        r << "my_data->main_iDomStatus \t" << my_data->main_iDomStatus->getAllObjectsStateString()<<std::endl;
        r << std::endl;
        r << "my_data->idom_all_state.houseState \t" <<stateToString(my_data->idom_all_state.houseState)<<std::endl;
        r << std::endl;
        r << "my_data->now_time \t" << my_data->now_time<<std::endl;
        r << "my_data->start - time \t" << my_data->start<<std::endl;
        r << std::endl;
        r << "my_data->pointer.ptr_buf \t" << my_data->pointer.ptr_buf<<std::endl;
        r << "my_data->pointer.ptr_who \t" << my_data->pointer.ptr_who<<std::endl;

        r << std::endl;
        r << "my_data->ptr_MPD_info->artist \t" << my_data->ptr_MPD_info->artist<<std::endl;
        r << "my_data->ptr_MPD_info->currentSongID \t" << my_data->ptr_MPD_info->currentSongID<<std::endl;
        r << "my_data->ptr_MPD_info->isPlay \t" << my_data->ptr_MPD_info->isPlay<<std::endl;
        r << "my_data->ptr_MPD_info->radio \t" << my_data->ptr_MPD_info->radio<<std::endl;
        r << "my_data->ptr_MPD_info->songList.at(0) \t" << my_data->ptr_MPD_info->songList.at(0)<<std::endl;
        r << "my_data->ptr_MPD_info->title \t" << my_data->ptr_MPD_info->title<<std::endl;
        r << "my_data->ptr_MPD_info->volume \t" << my_data->ptr_MPD_info->volume<<std::endl;

        r << std::endl;
        r << "my_data->main_RFLink->okTime \t" << my_data->main_RFLink->okTime<<std::endl;
        r << "my_data->main_RFLink->pingTime \t" << my_data->main_RFLink->pingTime<<std::endl;

        r << std::endl;
        r << "my_data->mainLCD-> \t" << my_data->mainLCD->getData() <<std::endl;

        r << "END.";
        ret = r.str();
    }
    else if(v[1] == "raspberry")
    {
        system(v[2].c_str());
        ret = "ram has beed freed";
    }
    else
    {
        ret = " what? - "+ v[1];
    }
    return ret;
}

std::string command_program::help()
{
    std::stringstream help;
    help << "program stop - close iDom server"<< std::endl;
    help << "program reload soft - reload iDom server" << std::endl;
    help << "program reload hard - reload iDom server" << std::endl;
    help << "program clear ram   - reload iDom server" << std::endl;
    help << "program debuge variable - show value iDom server variable" << std::endl;
    help << "program raspberry <command> - put command to raspberry " << std::endl;
    return help.str();
}
