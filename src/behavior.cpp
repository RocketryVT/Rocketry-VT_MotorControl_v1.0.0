#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <functional>
#include <bitset>
#include <sstream>

#include <behavior.h>
#include <logging.h>
#include <transmission.h>
#include <hardware.h>
#include <control.h>
#include <config.h>
#include <comms.h>
#include <predicates.h>

namespace behavior
{

std::string channel_str(const std::string &str)
{
    uint8_t id = transmission::getId(str);
    std::stringstream ss;
    ss << "0x" << std::hex << std::setw(2)
        << std::setfill('0') << (int) id << " | "
        << std::dec << std::setw(3) << (int) id
        << ": " << str;
    return ss.str();
}

std::map<std::string, std::function
    <void(std::vector<uint8_t>)>> on_receive
{

{"/control/motor/ping", [] (std::vector<uint8_t>)
{
    logging::announce("POLO", true, true);
}},

{"/control/motor/small-talk", [] (std::vector<uint8_t> data)
{
    std::string msg(data.begin(), data.end());
    std::stringstream ss;
    ss << "Recieved new ASCII message: \"" << msg << "\"";
    logging::announce(ss.str(), false, false);

    if (msg == "MARCO")
    {
        state::last_ping = std::chrono::steady_clock::now();
        logging::announce(ss.str(), true, true);
    }
    else if (msg == "VERSION")
    {
        logging::announce("Current firmware is "
            + cfg::version, true, true);
    }
    else if (msg == "SAY HI")
        logging::announce("Hello, world!", true, true);
    else if (msg == "BEST SUBTEAM?")
        logging::announce("Software is the best subteam!",
            true, true);
    else if (msg == "WHAT TEAM?")
        logging::announce("WILDCATS", true, true);
    else if (msg == "RESET")
        control::reset();
    else if (msg == "SHUTDOWN")
        control::exit(exit_code::soft_shutdown);
    else
        logging::announce("Unknown ASCII message: \"" +
            msg + "\"", true, true);
}},

// HARDWARE LOCKS

{"/control/motor/lock", [] (std::vector<uint8_t>)
{
    hardware::lock();
}},

{"/control/motor/unlock", [] (std::vector<uint8_t> data)
{
    if (data.size() == 0) hardware::lock();
    if (data.size() == 1)
    {
        uint8_t unlock_code = data[0];
        hardware::unlock(unlock_code);
    }
}},

// HARDWARE OPERATIONS

{"/control/begin-fill", [] (std::vector<uint8_t>)
{
    hardware::beginFill();
}},

{"/control/detach-fill-line", [] (std::vector<uint8_t>)
{
    hardware::disconnectFeedLine();
}},

{"/control/open-valves", [] (std::vector<uint8_t>)
{
    hardware::openValves();
}},

{"/control/close-valves", [] (std::vector<uint8_t>)
{
    hardware::closeValves();
}},

{"/control/launch", [] (std::vector<uint8_t>)
{
    hardware::launch();
}},

// PROGRAM REFLECTION

{"/control/motor/echo-commands", [] (std::vector<uint8_t>)
{
    logging::announce("Commands query:", true, true);
    for (auto e : on_receive)
        logging::announce(channel_str(e.first), true, true);
}},

{"/control/motor/echo-channels", [] (std::vector<uint8_t>)
{
    logging::announce("Channel query:", true, true);
    for (auto e : transmission::channels())
        logging::announce(channel_str(e.second), true, true);
}},

// LOGGING - TEMPORARILY DISABLED

/*
{"/ground/echo-loglist", [] (std::vector<uint8_t>)
{
    auto logMsg = [] (uint8_t id, const std::chrono::milliseconds &dur,
        const std::string &desc)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id << " | "
            << std::dec << std::setw(3) << (int) id
            << " (" << dur.count() << "ms): " << desc;
        return ss.str();
    };

    logging::announce("Loglist query:", true, true);
    for (auto e : logging::loglist())
    {
        uint8_t query = e.first;
        auto recipe_book = logging::recipes();
        auto recipe = recipe_book.find(query);
        if (recipe != recipe_book.end())
            logging::announce(logMsg(e.first, e.second,
                recipe->second.description), true, true);
        else
            logging::announce(logMsg(e.first, e.second,
                "No description for this log."), true, true);
    }
}}},

{transmission::getId("/ground/echo-logs"), {"ECHO LOGS",
[] (std::vector<uint8_t>)
{
    auto recipeMsg = [] (uint8_t id, const std::string &str)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id << " | "
            << std::dec << std::setw(3) << (int) id
            << ": " << str;
        return ss.str();
    };

    logging::announce("Log query:", true, true);
    for (auto e : logging::recipes())
    {
        logging::announce(recipeMsg(e.first,
            e.second.description), true, true);
    }
}}},

{transmission::getId("/ground/unlog-all"), {"UNLOG [uint8_t id = ALL]",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 0)
    {
        logging::clearLogs();
    }
    else if (data.size() == 1)
    {
        uint8_t id = data[0];
        logging::removeLog(id);
    }
}}},

{transmission::getId("/ground/begin-log"), {"LOG <uint8_t id> [uint16_t millis = 1000]",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 1)
    {
        uint8_t id = data[0];
        std::stringstream ss;
        ss << "Add log ID " << std::dec << (int) id
            << " with period 1000 ms";
        logging::announce(ss.str(), true, true);
        logging::addLog(id, std::chrono::milliseconds(1000));
    }
    else if (data.size() == 3)
    {
        uint8_t id;
        uint16_t millis;
        data >> id >> millis;
        std::stringstream ss;
        ss << "Add log ID " << std::dec << (int) id
            << " with period " << millis << " ms";
        logging::announce(ss.str(), true, true);
        logging::addLog(id, std::chrono::milliseconds(millis));
    }
}}},
*/

// UNIT TESTS

{"/control/motor/echo-tests", [] (std::vector<uint8_t>)
{
    logging::announce("Test query:", true, true);
    size_t id = 0;
    for (auto e : predicates::tests())
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << id << " | "
            << std::dec << std::setw(3) << id
            << ": " << e.description;
        logging::announce(ss.str(), true, true);
        ++id;
    }
}},

{"/control/motor/perform-tests", [] (std::vector<uint8_t> data)
{
    for (uint8_t id : data)
    {
        auto tests = predicates::tests();
        if (id >= tests.size())
        {
            std::stringstream ss;
            ss << "No test with ID " << (int) id;
            logging::announce(ss.str(), true, true);
            continue;
        }
        auto test = predicates::tests()[id];
        logging::announce("Performing test: " +
            test.description, true, true);
        bool ret = test.function();
        logging::announce(test.description + " -> " +
            (ret ? "YES" : "NO"), true, true);
    }
}},

// PROGRAM TERMINATION

{"/control/abort-all", [] (std::vector<uint8_t>)
{
    std::abort();
}},

{"/control/motor/abort", [] (std::vector<uint8_t>)
{
    std::abort();
}},

{"/control/motor/reboot", [] (std::vector<uint8_t>)
{
    logging::announce("Rebooting motor BB...", true, true);
    comms::flush();
    std::system("sudo reboot");
}},

{"/control/motor/reset", [] (std::vector<uint8_t>)
{
    control::reset();
}},

{"/control/motor/shutdown", [] (std::vector<uint8_t> data)
{
    uint8_t code = exit_code::soft_shutdown;
    if (data.size() > 0) code = data[0];
    control::exit(code);
}}

}; // on_receive

void dataReceipt(uint8_t id, const std::vector<uint8_t> &data)
{
    std::string channel = transmission::getChannel(id);
    std::stringstream ss;
    ss << "Recieved " << data.size() << " on " + channel;
    logging::announce(ss.str(), false, true);

    auto action = on_receive.find(channel);
    if (action != on_receive.end())
    {
        action->second(data);
    }
}

} // namespace behavior

