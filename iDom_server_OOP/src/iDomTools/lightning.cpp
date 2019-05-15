#include "lightning.h"

LIGHTNING::LIGHTNING()
{
    puts("LIGHTNING::LIGHTNING()");
    iDom_API::m_className.append(typeid (this).name());
    iDom_API::addToMap(iDom_API::m_className,this);
}

LIGHTNING::~LIGHTNING()
{
    puts("LIGHTNING::~LIGHTNING()");
    iDom_API::removeFromMap(iDom_API::m_className);
}

CARDINAL_DIRECTIONS::ALARM_INFO LIGHTNING::lightningAlert(nlohmann::json jj)
{
    CARDINAL_DIRECTIONS::ALARM_INFO data;
    nlohmann::json i;

#ifdef BT_TEST
    std::cout <<"\n\n data all " << i.dump(4) <<" size:"<< i.size() <<std::endl;
#endif
    if (jj.find("response") != jj.end())
    {
        i = jj.at("response").get<nlohmann::json>();
    }
    else
    {
        std::cout << " zly JSON " <<std::endl;
        return data;
    }

    auto _size = i.size();
    if (_size == 0)
    {
        data.riseAlarm = false;
        return data;
    }
    STATISTIC<int> ageAver(_size);
    STATISTIC<double> distanceKmAver(_size);
    STATISTIC<int> bearingAver(_size);
    for (auto j : i){
#ifdef BT_TEST
   //     std::cout <<"\n distance " << j.at("relativeTo").at("bearingENG").get<std::string>() << std::endl;
   //     std::cout <<"distance " << j.at("relativeTo").at("distanceKM").get<double>() << std::endl;
   //     std::cout <<"timestamp " << j.at("age").get<int>() << std::endl;
#endif
        ageAver.push_back(j.at("age").get<int>());
        distanceKmAver.push_back(j.at("relativeTo").at("distanceKM").get<double>());
        bearingAver.push_back(static_cast<int>(CARDINAL_DIRECTIONS::stringToCardinalDirectionsEnum(
                                                   j.at("relativeTo").at("bearingENG").get<std::string>()))
                              );
    }
    data.bearingENG = static_cast<CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM>(bearingAver.mode());
    data.distance = distanceKmAver.average();
    data.timestamp = ageAver.median();

    data.data.str(std::string());
    data.data << "ilość uderzeń: "<< _size << "\\n";
    data.data << "średni czas upłynięty od ostatniego uderzenia pioruna: "<< data.timestamp << " sek \\n";
    data.data << "średnia odległość ostatniego uderzenia pieruna: "<< data.distance <<" km \\n ";
    data.data << "kierunek uderzeń piorunów: " << CARDINAL_DIRECTIONS::cardinalDirectionsEnumToHuman(data.bearingENG);

    if(i.size() > 0){
        // std::cout << "jest size: " << i.size()<<std::endl;
        data.riseAlarm = true;
    }

    return data;
}

bool LIGHTNING::checkLightningAlert(CARDINAL_DIRECTIONS::ALARM_INFO *info)
{
#ifdef BT_TEST
    std::cout << "LIGHTNING::checkLightningAlert() bool "<< info->riseAlarm <<" local "
              << m_alarmState << std::endl
              << " distance " << info->distance << std::endl;
#endif
    if(info->riseAlarm == false && m_alarmState == false){
#ifdef BT_TEST
        std::cout << "(info->riseAlarm == false || alarmState == false)"<<std::endl;
#endif
        return false;
    }
    if(info->riseAlarm == false && m_alarmState == true){
#ifdef BT_TEST
        std::cout << "(info->riseAlarm == false || alarmState == true)"<<std::endl;
#endif
        m_alarmState = false;
        return false;
    }
    if(info->riseAlarm == true && m_alarmState == false){

#ifdef BT_TEST
        std::cout << "(info->riseAlarm == true || alarmState == false)"<<std::endl;
#endif
        m_alarmState = true;
        m_lightningTime = Clock::getTime();
        m_oldDistance = info->distance;
        std::cout << " w true m_oldDistance: "<< m_oldDistance <<std::endl;
        return true;
    }

#ifdef BT_TEST
    std::cout << "checkLightningAlert() - dystans"<<std::endl;
    std::cout << "Dystans: " <<info->distance << " m_oldDistance: "<< m_oldDistance <<std::endl;
#endif
    if(m_oldDistance > info->distance)
    {
        m_oldDistance = info->distance;
        return true;
    }
    else
    {
        m_oldDistance = info->distance;
    }
    ////////////////clear
    //oldDistance = 0.0;
    return false;
}

std::string LIGHTNING::dump() const
{
    std::stringstream ret;

    ret << m_className << " m_alarmState: " << this->m_alarmState << std::endl;
    ret << m_className << " m_lightningTime: " << this->m_lightningTime.getString() << std::endl;
    ret << m_className << " m_oldDistance: " << this->m_oldDistance << std::endl;

    return ret.str();
}

