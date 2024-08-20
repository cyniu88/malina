#include "../house_room_handler.h"

class house_room_handler_stub : public house_room_handler

{
public:
    explicit house_room_handler_stub(thread_context *k)
        : house_room_handler(k){};
    void onLock(){};
    void onUnlock(){};
    void onSunset(){};
    void onSunrise(){};
};
