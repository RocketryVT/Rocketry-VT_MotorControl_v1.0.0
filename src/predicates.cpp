#include <predicates.h>
#include <hardware.h>
#include <config.h>

namespace predicates
{

bool fillLineConnected()
{
    return hardware::feedLineConnected();
}

bool fillOngoing()
{
    return hardware::isFillOngoing();
}

bool supportLocked()
{
    return hardware::isLocked();
}

std::vector<test> test_list
{
    { "Fill line connected?", fillLineConnected },
    { "Fill ongoing?", fillOngoing },
    { "Support locked?", supportLocked }
};

std::vector<test> tests()
{
    return test_list;
}

} // namespace predicates

