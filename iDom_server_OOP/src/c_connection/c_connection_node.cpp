#include <iostream>
#include "c_connection.h"

void C_connection::setEncriptionKey(const std::string& key)
{
    m_encriptionKey = key;
}

void C_connection::setEncrypted(bool flag)
{
    m_encrypted = flag;
}

void C_connection::crypto(std::string &toEncrypt, std::string key, bool encrypted)
{
    if (!encrypted){
          return;
      }
    unsigned int keySize = key.size()-1;
#ifdef BT_TEST
    std::cout << "key: " << key << " size: " << key.size() << std::endl;
#endif
    for (unsigned int i = 0; i < toEncrypt.size (); i++)
    {
        if (keySize == 0)
            keySize = key.size()-1;
        else
            --keySize;
        toEncrypt[i] ^= key[keySize];
    }
}

void C_connection::onStartConnection()
{

}

void C_connection::onStopConnection()
{
    my_data->main_iDomTools->cameraLedOFF(my_data->server_settings->_camera.cameraLedOFF);
}

void C_connection::cryptoLog(std::string &toEncrypt)
{
    crypto(toEncrypt,m_encriptionKey,m_encrypted);
}
