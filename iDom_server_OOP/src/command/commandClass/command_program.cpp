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
        return "what?\n" + help();
    }
    if (v[1] == "version")
    {
        std::stringstream r;
        const char * PROG_INFO = "iDomServer: " __DATE__ ", " __TIME__;
        r << "wersja " <<PROG_INFO <<" "<< GIT_BRANCH <<" " << GIT_COMMIT_HASH << std::endl;
        return r.str();
    }
    if (v[1] == "stop")
    {
        my_data->main_iDomTools->close_iDomServer();
    }
    if (v.size() < 3 )
    {
        return "add more paramiters";
    }
    if(v[1] == "reload" && v[2] == "soft"){
        my_data->main_iDomTools->reloadSoft_iDomServer();
    }
    else if(v[1] == "reload" && v[2] == "hard")
    {
        my_data->main_iDomTools->reloadHard_iDomServer();
    }
    else if(v[1] == "clear" && v[2] == "ram")
    {
        useful_F::runLinuxCommand("sync; echo 3 > /proc/sys/vm/drop_caches");
        ret = "ram has beed freed";
    }
    else if(v[1] == "debuge" && v[2] == "variable")
    {
        std::stringstream r;
        r << "my_data->alarmTime.fromVolume \t" << my_data->alarmTime.fromVolume << std::endl;
        r << "my_data->alarmTime.radioID \t" << my_data->alarmTime.radioID << std::endl;
        r << "my_data->alarmTime.state \t" << stateToString( my_data->alarmTime.state) << std::endl;
        r << "my_data->alarmTime.time \t" << my_data->alarmTime.time.getString() << std::endl;
        r << "my_data->alarmTime.toVolume \t" << my_data->alarmTime.toVolume << std::endl;
        r << std::endl;
        r << "my_data->encriptionKey \t" << my_data->encriptionKey << std::endl;
        r << std::endl;
        r << "my_data->server_settings->_rs232.BaudRate \t"
          << my_data->server_settings->_rs232.BaudRate << std::endl;
        r << "my_data->server_settings->_camera.cameraLedOFF \t"
          << my_data->server_settings->_camera.cameraLedOFF << std::endl;
        r << "my_data->server_settings->_camera.cameraLedON \t"
          << my_data->server_settings->_camera.cameraLedON << std::endl;
        r << "my_data->server_settings->_camera.cameraURL \t"
          << my_data->server_settings->_camera.cameraURL << std::endl;
        r << "my_data->server_settings->_server.encrypted \t"
          << my_data->server_settings->_server.encrypted << std::endl;
        r << "my_data->server_settings->_fb_viber.facebookAccessToken \t"
          << my_data->server_settings->_fb_viber.facebookAccessToken << std::endl;
        r << "my_data->server_settings->_server.ftpServer.URL \t"
          << my_data->server_settings->_server.ftpServer.URL << std::endl;
        r << "my_data->server_settings->_server.ftpServer.user \t"
          << my_data->server_settings->_server.ftpServer.user << std::endl;
        r << "my_data->server_settings->_server.ID_server \t"
          << my_data->server_settings->_server.ID_server << std::endl;
        r << "my_data->server_settings->_server.lightningApiURL \t"
          << my_data->server_settings->_server.lightningApiURL << std::endl;
        r << "my_data->server_settings->_server.MENU_PATH \t"
          << my_data->server_settings->_server.MENU_PATH << std::endl;
        r << "my_data->server_settings->_server.MOVIES_DB_PATH \t"
          << my_data->server_settings->_server.MOVIES_DB_PATH << std::endl;
        r << "my_data->server_settings->_server.MPD_IP \t"
          << my_data->server_settings->_server.MPD_IP << std::endl;
        r << "my_data->server_settings->_server.omxplayerFile \t"
          << my_data->server_settings->_server.omxplayerFile << std::endl;
        r << "my_data->server_settings->_server.PORT \t"
          << my_data->server_settings->_server.PORT << std::endl;
        r << "my_data->server_settings->_rs232.portRS232 \t"
          << my_data->server_settings->_rs232.portRS232<< std::endl;
        r << "my_data->server_settings->_rs232.portRS232_clock \t"
          << my_data->server_settings->_rs232.portRS232_clock << std::endl;
        r << "my_data->server_settings->_server.radio433MHzConfigFile \t"
          << my_data->server_settings->_server.radio433MHzConfigFile << std::endl;
        r << "my_data->server_settings->_rflink.RFLinkBaudRate \t"
          << my_data->server_settings->_rflink.RFLinkBaudRate<< std::endl;
        r << "my_data->server_settings->_rflink.RFLinkPort \t"
          << my_data->server_settings->_rflink.RFLinkPort<< std::endl;
        r << "my_data->server_settings->_server.saveFilePath \t"
          << my_data->server_settings->_server.saveFilePath<< std::endl;
        r << "my_data->server_settings->_server.SERVER_IP \t"
          << my_data->server_settings->_server.SERVER_IP << std::endl;
        r << "my_data->server_settings->_runThread.CRON \t"
          << my_data->server_settings->_runThread.CRON << std::endl;
        r << "my_data->server_settings->_runThread.DUMMY \t"
          << my_data->server_settings->_runThread.DUMMY << std::endl;
        r << "my_data->server_settings->_runThread.IRDA \t"
          << my_data->server_settings->_runThread.IRDA << std::endl;
        r << "my_data->server_settings->_runThread.MPD \t"
          << my_data->server_settings->_runThread.MPD << std::endl;
        r << "my_data->server_settings->_runThread.RS232 \t"
          << my_data->server_settings->_runThread.RS232 << std::endl;
        r << "my_data->server_settings->_server.TS_KEY \t"
          << my_data->server_settings->_server.TS_KEY<< std::endl;
        r << "my_data->server_settings->_fb_viber.viberAvatar \t"
          << my_data->server_settings->_fb_viber.viberAvatar<< std::endl;
        r << "my_data->server_settings->_fb_viber.viberReceiver.at(0) \t"
          << my_data->server_settings->_fb_viber.viberReceiver.at(0)<< std::endl;
        r << "my_data->server_settings->_fb_viber.viberSender \t"
          << my_data->server_settings->_fb_viber.viberSender<< std::endl;
        r << "my_data->server_settings->_fb_viber.viberToken \t"
          << my_data->server_settings->_fb_viber.viberToken<< std::endl;
        r << "my_data->server_settings->_server.v_delay \t"
          << my_data->server_settings->_server.v_delay<< std::endl;
        r << std::endl;
        r << "my_data->server_settings->sleeper \t"
          << my_data->sleeper<< std::endl;
        r << std::endl;
        r << "my_data->iDomProgramState \t"
          << static_cast<int>(my_data->iDomProgramState) << std::endl;
        r << std::endl;
        r << "my_data->serverStarted \t"  << my_data->serverStarted << std::endl;
        r << std::endl;
        r << "my_data->main_iDomStatus \t"
          << my_data->main_iDomStatus->getAllObjectsStateString()<< std::endl;
        r << std::endl;
        r << "my_data->idom_all_state.houseState \t"
          <<stateToString(my_data->idom_all_state.houseState)<< std::endl;
        r << std::endl;
        r << "my_data->now_time \t" << my_data->now_time<< std::endl;
        r << "my_data->start - time \t" << my_data->start<< std::endl;
        r << std::endl;
        r << "my_data->pointer.ptr_buf \t" << my_data->pointer.ptr_buf<< std::endl;
        r << "my_data->pointer.ptr_who \t" << my_data->pointer.ptr_who<< std::endl;

        r << std::endl;
        r << "my_data->ptr_MPD_info->artist \t" << my_data->ptr_MPD_info->artist<< std::endl;
        r << "my_data->ptr_MPD_info->currentSongID \t" << my_data->ptr_MPD_info->currentSongID<< std::endl;
        r << "my_data->ptr_MPD_info->isPlay \t" << my_data->ptr_MPD_info->isPlay<< std::endl;
        r << "my_data->ptr_MPD_info->radio \t" << my_data->ptr_MPD_info->radio<< std::endl;
        r << "my_data->ptr_MPD_info->songList.at(0) \t" << my_data->ptr_MPD_info->songList.at(0)<< std::endl;
        r << "my_data->ptr_MPD_info->title \t" << my_data->ptr_MPD_info->title<< std::endl;
        r << "my_data->ptr_MPD_info->volume \t" << my_data->ptr_MPD_info->volume<< std::endl;

        r << std::endl;
        r << "my_data->main_RFLink->okTime \t" << my_data->main_RFLink->m_okTime<< std::endl;
        r << "my_data->main_RFLink->pingTime \t" << my_data->main_RFLink->m_pingTime<< std::endl;

        r << std::endl;
        r << "my_data->mainLCD-> \t" << my_data->mainLCD->getData() << std::endl;

        r << "my_data->mqttHandler->_connected: " << my_data->mqttHandler->_connected << std::endl;
        r << "my_data->mqttHandler->_subscribed: " << my_data->mqttHandler->_subscribed << std::endl;
        r << "buffer size:" << std::endl;

        r << "mqtt bffer: " << my_data->mqttHandler->getReceiveQueueSize() << std::endl;
        r << "rflink map size: " << my_data->main_RFLink->m_rflinkMAP.size() << std::endl;

        r << "alarm map size: " << my_data->iDomAlarm.alarmSize() << std::endl;

        //r << my_data->main_iDomTools->dump() << std::endl;
        //r << my_data->main_iDomTools-> << std::endl;

        r << iDom_API::getDump() << std::endl;

        r << "END.";
        ret = r.str();
    }
    else if(v[1] == "raspberry")
    {
        int i = useful_F::runLinuxCommand(v[2].c_str());
        ret = "command done with exitcode: " + std::to_string(i);
    }
    else
    {
        ret = " what? - "+ v[1];
    }
    return ret;
}

std::string command_program::help() const
{
    std::stringstream help;
    help << "program version - show iDom server info " << std::endl;
    help << "program stop - close iDom server"<< std::endl;
    help << "program reload soft - reload iDom server" << std::endl;
    help << "program reload hard - reload iDom server" << std::endl;
    help << "program clear ram   - reload iDom server" << std::endl;
    help << "program debuge variable - show value iDom server variable" << std::endl;
    help << "program raspberry <command> - put command to raspberry " << std::endl;
    return help.str();
}
