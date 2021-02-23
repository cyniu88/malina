#include "../house_room_handler.h"

class house_room_handler_stub : public house_room_handler

{
public:
    house_room_handler_stub(thread_data* k):house_room_handler(k){};
    void onLock(){};
    void onUnlock(){};
    void onSunset(){};
    void onSunrise(){};
};
