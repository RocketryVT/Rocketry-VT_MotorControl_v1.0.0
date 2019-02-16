// led.h

#include <drivers/led.h>

namespace drivers
{

namespace led
{

uint8_t state = 0;

void set(uint8_t newState)
{
    state = newState;
}

void set(uint8_t index, bool newState)
{
    if (index > 7U) return;
    state |= (newState << index);
}

uint8_t get()
{
    return state;
}

} // namespace led

} // namespace drivers

