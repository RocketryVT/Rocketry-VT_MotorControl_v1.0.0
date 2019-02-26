// led.h

#include <drivers/led.h>

namespace drivers
{

namespace led
{

bool is_init = false;
uint8_t state = 0;

bool init()
{
    is_init = true;
    return ok();
}

bool ok()
{
    return true;
}

void set(uint8_t newState)
{
    if (!is_init) return;
    state = newState;
}

void set(uint8_t index, bool newState)
{
    if (!is_init || index > 7U) return;
    state |= (newState << index);
}

uint8_t get()
{
    return state;
}

} // namespace led

} // namespace drivers

