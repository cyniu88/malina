#ifndef LIGHTNING_H
#define LIGHTNING_H
#include <iostream>

#include "../../libs/Statistic/statistic.h"
#include "idomtools_useful.h"
#include "json.hpp"

class CARDINAL_DIRECTIONS{
public:
    enum class CARDINAL_DIRECTIONS_ENUM{
        /*
           N
        NNW NNE
      NW       NE
   WNW		     ENE
 W				    E
   WSW			 ESE
      SW	   SE
        SSW	 SSE
           S
    */

        N = 1,NNE,NE,ENE,E,ESE,SE,SSE,S,SSW,SW,WSW,W,WNW,NW,NNW,ERROR
    };
    static CARDINAL_DIRECTIONS_ENUM stringToCardinalDirectionsEnum(std::string s){
             if (s == "N")      return CARDINAL_DIRECTIONS_ENUM::N;
        else if (s == "NNE")    return CARDINAL_DIRECTIONS_ENUM::NNE;
        else if (s == "NE")     return CARDINAL_DIRECTIONS_ENUM::NE;
        else if (s == "ENE")    return CARDINAL_DIRECTIONS_ENUM::ENE;
        else if (s == "E")      return CARDINAL_DIRECTIONS_ENUM::E;
        else if (s == "ESE")    return CARDINAL_DIRECTIONS_ENUM::ESE;
        else if (s == "SE")     return CARDINAL_DIRECTIONS_ENUM::SE;
        else if (s == "SSE")    return CARDINAL_DIRECTIONS_ENUM::SSE;
        else if (s == "S")      return CARDINAL_DIRECTIONS_ENUM::S;
        else if (s == "SSW")    return CARDINAL_DIRECTIONS_ENUM::SSW;
        else if (s == "SW")     return CARDINAL_DIRECTIONS_ENUM::SW;
        else if (s == "WSW")    return CARDINAL_DIRECTIONS_ENUM::WSW;
        else if (s == "W")      return CARDINAL_DIRECTIONS_ENUM::W;
        else if (s == "WNW")    return CARDINAL_DIRECTIONS_ENUM::WNW;
        else if (s == "NW")     return CARDINAL_DIRECTIONS_ENUM::NW;
        else if (s == "NNW")    return CARDINAL_DIRECTIONS_ENUM::NNW;
        else                    return CARDINAL_DIRECTIONS_ENUM::ERROR;
    }

    static std::string cardinalDirectionsEnumToString(CARDINAL_DIRECTIONS_ENUM e){
        switch (e){
        case CARDINAL_DIRECTIONS_ENUM::N:
            return "N";
        case CARDINAL_DIRECTIONS_ENUM::NNE:
            return "NNE";
        case CARDINAL_DIRECTIONS_ENUM:: NE:
            return "NE";
        case CARDINAL_DIRECTIONS_ENUM::ENE:
            return "ENE";
        case CARDINAL_DIRECTIONS_ENUM::E:
            return "E";
        case CARDINAL_DIRECTIONS_ENUM::ESE:
            return "ESE";
        case CARDINAL_DIRECTIONS_ENUM::SE:
            return "SE";
        case CARDINAL_DIRECTIONS_ENUM::SSE:
            return "SSE";
        case CARDINAL_DIRECTIONS_ENUM::S:
            return "S";
        case CARDINAL_DIRECTIONS_ENUM::SSW:
            return "SSW";
        case CARDINAL_DIRECTIONS_ENUM::SW:
            return "SW";
        case CARDINAL_DIRECTIONS_ENUM::WSW:
            return "WSW";
        case CARDINAL_DIRECTIONS_ENUM::W:
            return "W";
        case CARDINAL_DIRECTIONS_ENUM::WNW:
            return "WNW";
        case CARDINAL_DIRECTIONS_ENUM::NW:
            return "NW";
        case CARDINAL_DIRECTIONS_ENUM::NNW:
            return "NNW";
        default:
            return "UNKNOWN DIRECTION";
        }
    }
    static std::string cardinalDirectionsEnumToHuman(CARDINAL_DIRECTIONS_ENUM e){
        switch (e){
        case CARDINAL_DIRECTIONS_ENUM::N:
            return "północ";
        case CARDINAL_DIRECTIONS_ENUM::NNE:
            return "północ - północny wschód";
        case CARDINAL_DIRECTIONS_ENUM:: NE:
            return "północny wschód";
        case CARDINAL_DIRECTIONS_ENUM::ENE:
            return "wschód - północny wschód";
        case CARDINAL_DIRECTIONS_ENUM::E:
            return "wschód";
        case CARDINAL_DIRECTIONS_ENUM::ESE:
            return "wschód - południowy wschód";
        case CARDINAL_DIRECTIONS_ENUM::SE:
            return "południowy wschód";
        case CARDINAL_DIRECTIONS_ENUM::SSE:
            return "południe - południowy wschód";
        case CARDINAL_DIRECTIONS_ENUM::S:
            return "południe";
        case CARDINAL_DIRECTIONS_ENUM::SSW:
            return "południe - południowy zachów";
        case CARDINAL_DIRECTIONS_ENUM::SW:
            return "południowy zachów";
        case CARDINAL_DIRECTIONS_ENUM::WSW:
            return "zachód - południowy zachów";
        case CARDINAL_DIRECTIONS_ENUM::W:
            return "zachód";
        case CARDINAL_DIRECTIONS_ENUM::WNW:
            return "zachód - północny zachód";
        case CARDINAL_DIRECTIONS_ENUM::NW:
            return "północny zachód";
        case CARDINAL_DIRECTIONS_ENUM::NNW:
            return "północ - północny zachód";
        default:
            return "UNKNOWN DIRECTION";
        }
    }
    struct ALARM_INFO{
        bool riseAlarm = false;
        std::stringstream data;
        unsigned int timestamp = 0; //second
        double distance  = 0.0;  //km
        CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM bearingENG = CARDINAL_DIRECTIONS::CARDINAL_DIRECTIONS_ENUM::ERROR;
    };
};

class LIGHTNING
{
public:
    LIGHTNING();
    ~LIGHTNING();
    CARDINAL_DIRECTIONS::ALARM_INFO lightningAlert(nlohmann::json jj);
    void checkLightningAlert(CARDINAL_DIRECTIONS::ALARM_INFO* info);
};

#endif // LIGHTNING_H