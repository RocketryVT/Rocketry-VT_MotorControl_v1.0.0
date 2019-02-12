#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <functional>

#include <behavior.h>
#include <logging.h>
#include <transmission.h>
#include <hardware.h>
#include <control.h>
#include <config.h>
#include <comms.h>

namespace behavior
{

// fill nitrous
// bleed nitrous
// continuity test
// motor unlock dead period (10 seconds)
// ignition
// check if fill valve is attached

struct action
{
    std::string description;
    std::function<void(std::vector<uint8_t>)> function;
};

std::map<uint8_t, action> on_receive
{

{0x04, {"Queries the lexicon (for an ID if given).",
[] (std::vector<uint8_t> data)
{
    auto lexMsg = [] (uint8_t id, const std::string &str)
    {
        std::cout << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id
            << ": " << str << std::dec << std::endl;
    };

    if (data.size() == 0)
        for (auto e : on_receive)
            lexMsg(e.first, e.second.description);
    else if (data.size() == 1)
    {
        uint8_t query = data[0];
        auto action = on_receive.find(query);
        if (action != on_receive.end())
            lexMsg(action->first, action->second.description);
        else
            lexMsg(action->first, "No action associated.");
    }
}}},

{0x06, {"Updates the controller ping timer.",
[] (std::vector<uint8_t>)
{
    state::last_ping = std::chrono::steady_clock::now();
}}},

{0x18, {"Unlocks the motor with a code, or locks it with none.",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 0) hardware::lock();
    if (data.size() == 1)
    {
        uint8_t unlock_code = data[0];
        hardware::unlock(unlock_code);
    }

    #ifdef DEBUG
    std::cout << (hardware::isLocked() ? 
        "locked" : "unlocked") << " (state "
        << (int) hardware::lockState() << ")" << std::endl;
    #endif
}}},

{0x23, {"ASCII message packet.",
[] (std::vector<uint8_t> data)
{
    std::string msg(data.begin(), data.end());
    #ifdef DEBUG
    std::cout << "Recieved new ASCII message: \""
        << msg << "\"" << std::endl;
    #endif

    if (msg == "MARCO")
    {
        state::last_ping = std::chrono::steady_clock::now();
        std::cout << "Polo!" << std::endl;
    }
    else if (msg == "VERSION")
    {
        std::cout << "Current firmware version is "
            << cfg::version << std::endl;
    }
    else if (msg == "SAY HI")
        std::cout << "Hello, world!" << std::endl;
    else if (msg == "BEST SUBTEAM?")
        std::cout << "Software is the best subteam!" << std::endl;
    else if (msg == "WHAT TEAM?")
        std::cout << "WILDCATS" << std::endl;
    else if (msg == "RESET")
        control::reset();
    else if (msg == "SHUTDOWN")
        control::exit(1); // soft shutdown
    else
    {
        #ifdef DEBUG
        std::cout << "Unknown ASCII message: \""
            << msg << "\"" << std::endl;
        #endif
    }
}}},

{0x30, {"Set the rocket status to a provided value.",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 1)
    {
        state::status = data[0];
    }
}}},

{126, {"Resets the controller.",
[] (std::vector<uint8_t>)
{
    control::reset();
}}},

{127, {"Shuts down the controller with a given code.",
[] (std::vector<uint8_t> data)
{
    if (data.size() < 1) return;
    control::exit(data[0]);
}}},

}; // on_receive

void dataReceipt(uint8_t id, const std::vector<uint8_t> &data)
{
    auto action = on_receive.find(id);
    if (action != on_receive.end())
        action->second.function(data);
    else
    {
        #ifdef DEBUG
        std::cout << "It's free real estate: 0x"
            << std::hex << (int) id << std::endl;
        #endif
    }
}

} // namespace behavior

