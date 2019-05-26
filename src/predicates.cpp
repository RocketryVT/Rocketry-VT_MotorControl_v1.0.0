#include <predicates.h>
#include <hardware.h>
#include <config.h>

namespace predicates
{

std::vector<test> test_list
{
    { "Fill line connected?", fillLineConnected },
    { "Fill ongoing?", fillOngoing },
    { "Nitrous tank full?", nitrousTankFull },
    { "Continuity good?", continuityOk },
    { "Motor locked?", motorLocked }
};

bool fillLineConnected()
{
    return hardware::feedLineConnected();
}

bool fillOngoing()
{
    return hardware::isFillOngoing();
}

bool nitrousTankFull()
{
    return state::nh >= cfg::max_nitrous_level;
}

bool continuityOk()
{
    return hardware::continuity();
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

