#include "command_program.h"
#include "../../functions/functions.h"
#include "../../433MHz/RFLink/rflinkhandler.h"
#include "../../iDomTools/idomtools_interface.h"

command_program::command_program(const std::string &name):command(name){}

std::string command_program::execute(std::vector<std::string> &v, thread_data *context)
{
    std::string ret = help();
    if (v.size() <2 )
    {
        return "what?\n" + help();
    }
    if (v[1] == "version")
    {
        std::stringstream r;
        const char* PROG_INFO = "iDomServer: " __DATE__ ", " __TIME__;
        r << "wersja " << PROG_INFO << " " << GIT_BRANCH << " " << GIT_COMMIT_HASH << std::endl;
        return r.str();
    }
    if (v[1] == "stop")
    {
        context->main_iDomTools->close_iDomServer();
        return "CLOSE";
    }
    if (v.size() < 3 )
    {
        std::cout << "cyniu size: " << v.size() << std::endl;
        return "add more paramiters";
    }
    if(v[1] == "reload" && v[2] == "soft"){
        context->main_iDomTools->reloadSoft_iDomServer();
        return "CLOSE";
    }
    else if(v[1] == "reload" && v[2] == "hard")
    {
        context->main_iDomTools->reloadHard_iDomServer();
        return "CLOSE";
    }
    else if(v[1] == "clear" && v[2] == "ram")
    {
        useful_F::runLinuxCommand("sync; echo 3 > /proc/sys/vm/drop_caches");
        ret = "ram has beed freed";
    }
    else if(v[1] == "debuge" && v[2] == "variable")
    {
        std::stringstream r;
        r << std::boolalpha;
        r << "context->alarmTime.fromVolume \t" << context->alarmTime.fromVolume << std::endl;
        r << "context->alarmTime.radioID \t" << context->alarmTime.radioID << std::endl;
        r << "context->alarmTime.state \t" << context->alarmTime.state << std::endl;
        r << "context->alarmTime.time \t" << context->alarmTime.time.getString() << std::endl;
        r << "context->alarmTime.toVolume \t" << context->alarmTime.toVolume << std::endl;
        r << "command: " << std::endl;
        for (const auto& k : context->alarmTime.commands)
        {
            r << k << std::endl;
        }
        r << std::endl;
        r << "context->encriptionKey \t" << context->encriptionKey << std::endl;
        r << std::endl;
        r << "context->server_settings->_rs232.BaudRate \t"
          << context->server_settings->_rs232.BaudRate << std::endl;
        r << "context->server_settings->_server.encrypted \t"
          << context->server_settings->_server.encrypted << std::endl;
        r << "context->server_settings->_server.ftpServer.URL \t"
          << context->server_settings->_server.ftpServer.URL << std::endl;
        r << "context->server_settings->_server.ftpServer.user \t"
          << context->server_settings->_server.ftpServer.user << std::endl;
        r << "context->server_settings->_server.ID_server \t"
          << context->server_settings->_server.ID_server << std::endl;
        r << "context->server_settings->_server.lightningApiURL \t"
          << context->server_settings->_server.lightningApiURL << std::endl;
        r << "context->server_settings->_server.MPD_IP \t"
          << context->server_settings->_server.MPD_IP << std::endl;
        r << "context->server_settings->_server.PORT \t"
          << context->server_settings->_server.PORT << std::endl;
        r << "context->server_settings->_rs232.portRS232 \t"
          << context->server_settings->_rs232.portRS232<< std::endl;
        r << "context->server_settings->_server.radio433MHzConfigFile \t"
          << context->server_settings->_server.radio433MHzConfigFile << std::endl;
        r << "context->server_settings->_rflink.RFLinkBaudRate \t"
          << context->server_settings->_rflink.RFLinkBaudRate<< std::endl;
        r << "context->server_settings->_rflink.RFLinkPort \t"
          << context->server_settings->_rflink.RFLinkPort<< std::endl;
        r << "context->server_settings->_server.saveFilePath \t"
          << context->server_settings->_server.saveFilePath<< std::endl;
        r << "context->server_settings->_server.SERVER_IP \t"
          << context->server_settings->_server.SERVER_IP << std::endl;
        r << "context->server_settings->_runThread.CRON \t"
          << context->server_settings->_runThread.CRON << std::endl;
        r << "context->server_settings->_runThread.DUMMY \t"
          << context->server_settings->_runThread.DUMMY << std::endl;
        r << "context->server_settings->_runThread.MPD \t"
          << context->server_settings->_runThread.MPD << std::endl;
        r << "context->server_settings->_runThread.RS232 \t"
          << context->server_settings->_runThread.RS232 << std::endl;
        r << "context->server_settings->_runThread.RFLink \t"
          << context->server_settings->_runThread.RFLink << std::endl;
        r << "context->server_settings->_runThread.MQTT \t"
          << context->server_settings->_runThread.MQTT<< std::endl;
        r << "context->server_settings->_server.TS_KEY \t"
          << context->server_settings->_server.TS_KEY<< std::endl;
        r << "context->server_settings->_fb_viber.viberAvatar \t"
          << context->server_settings->_fb_viber.viberAvatar<< std::endl;
        r << "context->server_settings->_fb_viber.viberReceiver.at(0) \t"
          << context->server_settings->_fb_viber.viberReceiver.at(0) << std::endl;
        r << "context->server_settings->_fb_viber.viberSender \t"
          << context->server_settings->_fb_viber.viberSender << std::endl;
        r << "context->server_settings->_fb_viber.viberToken \t"
          << context->server_settings->_fb_viber.viberToken << std::endl;
        r << "context->server_settings->_server.v_delay \t"
          << context->server_settings->_server.v_delay << std::endl;
        r << "context->server_settings->_command\t"
          << context->server_settings->_command.dump(4) << std::endl;
        r << std::endl;
        r << "context->server_settings->sleeper \t"
          << context->sleeper<< std::endl;
        r << std::endl;
        r << "context->iDomProgramState \t"
          << static_cast<int>(context->iDomProgramState) << std::endl;
        r << std::endl;
        r << "context->serverStarted \t"  << context->serverStarted << std::endl;
        r << std::endl;
        r << "context->main_iDomStatus \t"
          << context->main_iDomStatus->getAllObjectsStateString()<< std::endl;
        r << std::endl;
        r << "context->idom_all_state.houseState \t"
          << context->idom_all_state.houseState << std::endl;
        r << std::endl;
        r << "context->now_time \t" << context->now_time<< std::endl;
        r << "context->start - time \t" << context->start<< std::endl;
        r << std::endl;

        r << "context->ptr_MPD_info->artist \t" << context->ptr_MPD_info->artist<< std::endl;
        r << "context->ptr_MPD_info->currentSongID \t" << context->ptr_MPD_info->currentSongID<< std::endl;
        r << "context->ptr_MPD_info->isPlay \t" << context->ptr_MPD_info->isPlay<< std::endl;
        r << "context->ptr_MPD_info->radio \t" << context->ptr_MPD_info->radio<< std::endl;
        r << "context->ptr_MPD_info->songList.at(0) \t" << context->ptr_MPD_info->songList.at(0)<< std::endl;
        r << "context->ptr_MPD_info->title \t" << context->ptr_MPD_info->title<< std::endl;
        r << "context->ptr_MPD_info->volume \t" << context->ptr_MPD_info->volume<< std::endl;

        r << std::endl;
        r << "context->main_RFLink->okTime \t" << context->main_RFLink->m_okTime<< std::endl;
        r << "context->main_RFLink->pingTime \t" << context->main_RFLink->m_pingTime<< std::endl;

        r << std::endl;

        r << "context->mqttHandler->_connected: " << context->mqttHandler->_connected << std::endl;
        r << "context->mqttHandler->_subscribed: " << context->mqttHandler->_subscribed << std::endl;
        r << "buffer size:" << std::endl;

        r << "mqtt bffer: " << context->mqttHandler->getReceiveQueueSize() << std::endl;
        r << "rflink map size: " << context->main_RFLink->m_rflinkMAP.size() << std::endl;

        r << "alarm map size: " << context->iDomAlarm.alarmSize() << std::endl;

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
