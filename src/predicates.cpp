#include <predicates.h>
#include <hardware.h>
#include <config.h>

namespace predicates
{

bool fillLineConnected()
{
    return false;
}

bool nitrousTankFull()
{
    // centimeters at which tank is considered full
    // const uint16_t nitrousFull = 300;
    // return state::nh >= nitrousFull;
    return false;
}

bool nitrousTankStable()
{
    return true;
}

bool continuityOk()
{
    return hardware::continuity();
}

bool telemetryOk()
{
    return true;
}

bool overpressureOccured()
{
    return false;
}

bool motorLocked()
{
    return hardware::isLocked();
}

} // namespace predicates

