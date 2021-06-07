#include <iostream>
#include "menu_kodi.h"
#include "menu_root.h"

/*
up     [{"jsonrpc":"2.0","method":"Input.Up","params":[],"id":2361}]
left   [{"jsonrpc":"2.0","method":"Input.Left","params":[],"id":2460}]
right  [{"jsonrpc":"2.0","method":"Input.Right","params":[],"id":2621}]
down   [{"jsonrpc":"2.0","method":"Input.Down","params":[],"id":2621}]
enter  [{"jsonrpc":"2.0","method":"Input.Select","params":[],"id":2923}]
back   [{"jsonrpc":"2.0","method":"Input.Back","params":[],"id":3749}]
play   [{"jsonrpc":"2.0","method":"Player.PlayPause","params":[1,"toggle"],"id":3999}]
stop   [{"jsonrpc":"2.0","method":"Player.Stop","params":[1],"id":11536}]
home   [{"jsonrpc":"2.0","method":"Input.Home","params":[],"id":4405}]
set volume [{"jsonrpc":"2.0","method":"Application.SetVolume","params":[80],"id":8033}]
get volumr [{"jsonrpc":"2.0","method":"Application.GetProperties","params":[["volume"]],"id":492}]
quit [{"jsonrpc":"2.0","method":"Application.Quit","params":[],"id":6}]
 */

std::string MENU_KODI::sendCommandToKodi(const std::string &command, const std::string &param)
{
    std::string readBuffer;
    std::string data = R"({"jsonrpc":"2.0","method":")";
    data.append(command);
    data.append(R"(","params":[)");
    data.append(param);
    data.append(R"(],"id":)");
    data.append(std::to_string(++kodiMsgId));
    data.append("}");

    std::string address = kodiHost;

   // CURLcode ret;
    CURL *hnd;
    struct curl_slist *slist1;
    std::string jsonstr = data;

    slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_URL, address.c_str());
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, jsonstr.c_str());
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.38.0");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist1);
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(hnd, CURLOPT_PORT, 8080);
    //curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_perform(hnd);

    curl_easy_cleanup(hnd);
    hnd = NULL;
    curl_slist_free_all(slist1);
    slist1 = NULL;
    return readBuffer;
}

MENU_KODI::MENU_KODI(thread_data *my_data, LCD_c* lcdPTR, MENU_STATE_MACHINE* msm, STATE lcdLED): MENU_STATE_BASE (my_data, lcdPTR, msm, lcdLED)
{
    std::cout << "MENU_KODI::MENU_KODI()" << std::endl;
}

MENU_KODI::MENU_KODI(const MENU_KODI &base):MENU_STATE_BASE(base)
{
    // std::cout << "MENU_KODI::MENU_KODI() kopiujacy" << std::endl;
}

MENU_KODI::MENU_KODI(MENU_KODI &&base):MENU_STATE_BASE(std::move(base))
{
    //  std::cout << "MENU_KODI::MENU_KODI() przenoszacy" << std::endl;
}

MENU_KODI &MENU_KODI::operator=(const MENU_KODI &base)
{
    //  std::cout << "MENU_KODI::operator = kopiujacy" << std::endl;
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_KODI &MENU_KODI::operator=(MENU_KODI &&base)
{
    //  std::cout << "MENU_KODI::operator = przenoszacy" << std::endl;
    if(&base not_eq this){
        my_dataPTR = base.my_dataPTR;
        lcdPTR = base.lcdPTR;
        stateMachinePTR = base.stateMachinePTR;
    }
    return * this;
}

MENU_KODI::~MENU_KODI()
{

}

void MENU_KODI::entry()
{
    my_dataPTR->main_iDomTools->startKodi_Thread();
}

void MENU_KODI::exit()
{

}

void MENU_KODI::keyPadOk()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Select","") << std::endl;
}

void MENU_KODI::keyPadUp()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Up","") << std::endl;
}

void MENU_KODI::keyPadDown()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Down","") << std::endl;
}

void MENU_KODI::keyPadLeft()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Left","") << std::endl;
}

void MENU_KODI::keyPadRight()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Right","") << std::endl;
}

void MENU_KODI::keyPadRes()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Back","") << std::endl;
}

void MENU_KODI::keyPadEpg()
{
    sendCommandToKodi("Player.Stop", "");
}

void MENU_KODI::keyPadPower()
{
    std::cout << "odebrano: " << sendCommandToKodi("Application.Quit","") << std::endl;
}

void MENU_KODI::keyPadMenu()
{
    std::cout << "odebrano: " << sendCommandToKodi("Input.Home","") << std::endl;
}

std::string MENU_KODI::getStateName()
{
    return typeid (this).name();
}

void MENU_KODI::timeout(std::function<void ()> function)
{
    changeStateTo<MENU_ROOT>();
}
