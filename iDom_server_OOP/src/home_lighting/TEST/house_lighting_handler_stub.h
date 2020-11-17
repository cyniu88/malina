#include "../house_lighting_handler.h"

class house_lighting_handler_stub : public house_lighting_handler

{
public:
    house_lighting_handler_stub(thread_data* k):house_lighting_handler(k){};
    void onLock(){};
    void onUnlock(){};
    void onSunset(){};
    void onSunrise(){};
};
