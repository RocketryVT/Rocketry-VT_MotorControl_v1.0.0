#include <predicates.h>
#include <hardware.h>
#include <config.h>

namespace predicates
{

std::vector<test> test_list
{
    { "Fill line connected?", fillLineConnected },
    { "Nitrous tank full?", nitrousTankFull },
    { "Nitrous tank stable?", nitrousTankStable },
    { "Continuity good?", continuityOk },
    { "Telemetry good?", telemetryOk },
    { "Overpressure event occurred?", overpressureOccurred },
    { "Motor locked?", motorLocked }
};

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
    return false;
}

bool overpressureOccurred()
{
    return false;
}

bool motorLocked()
{
    return hardware::isLocked();
}

std::vector<test> tests()
{
    return test_list;
}

} // namespace predicates

