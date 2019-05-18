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

struct action
{
    std::string description;
    std::function<void(std::vector<uint8_t>)> function;
};

std::map<uint8_t, action> on_receive
{

{transmission::getId("/ground/ping"), {"MARCO",
[] (std::vector<uint8_t>)
{
    logging::announce("POLO", true, true);
    state::last_ping = std::chrono::steady_clock::now();
}}},

{transmission::getId("/ground/echo-commands"), {"ECHO LEXICON",
[] (std::vector<uint8_t>)
{
    auto lexMsg = [] (uint8_t id, const std::string &str)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id << " | "
            << std::dec << std::setw(3) << (int) id
            << ": " << str;
        return ss.str();
    };

    logging::announce("Commands query:", true, true);
    for (auto e : on_receive)
        logging::announce(lexMsg(e.first, e.second.description), true, true);
}}},

{transmission::getId("/ground/echo-channels"), {"ECHO CHANNELS",
[] (std::vector<uint8_t>)
{
    auto lexMsg = [] (uint8_t id, const std::string &str)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id << " | "
            << std::dec << std::setw(3) << (int) id
            << ": " << str;
        return ss.str();
    };

    logging::announce("Channel query:", true, true);
    auto channels = transmission::channels();
    for (size_t i = 0; i < channels.size(); ++i)
        logging::announce(lexMsg(i, channels[i]), true, true);
}}},

{transmission::getId("/ground/echo-loglist"), {"ECHO LOGLIST",
[] (std::vector<uint8_t>)
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

{transmission::getId("/ground/echo-tests"), {"ECHO TESTS",
[] (std::vector<uint8_t>)
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
}}},

{transmission::getId("/ground/perform-test"), {"PERFORM TEST <uint8_t id>",
[] (std::vector<uint8_t> data) 
{
    if (data.size() == 0) return;
    
    uint8_t id = data[0];
    auto tests = predicates::tests();
    if (id >= tests.size()) return;
    auto test = predicates::tests()[id];
    logging::announce("Performing test: " + test.description, true, true);
    bool ret = test.function();
    logging::announce(test.description + " -> " +
        (ret ? "YES" : "NO"), true, true);
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

{transmission::getId("/ground/set-lock"), {"SET LOCK [uint8_t lockstate = 0]",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 0) hardware::lock();
    if (data.size() == 1)
    {
        uint8_t unlock_code = data[0];
        hardware::unlock(unlock_code);
    }

    std::stringstream ss;
    ss << (hardware::isLocked() ? 
        "locked" : "unlocked") << " (state "
        << (int) hardware::lockState() << ")";
    logging::announce(ss.str(), true, true);
}}},

{transmission::getId("/ground/small-talk"), {"HAVE A CHAT <string message>",
[] (std::vector<uint8_t> data)
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
        control::exit(1); // soft shutdown
    else
        logging::announce("Unknown ASCII message: \"" + msg, true, true);
}}},

{transmission::getId("/ground/set-status"), {"SET STATUS <uint8_t status>",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 1)
        state::status = data[0];
}}},

{transmission::getId("/ground/echo-status") , {"ECHO STATUS",
[] (std::vector<uint8_t>)
{
    std::stringstream ss;
    ss << "Vehicle status: 0x" << std::hex << std::setw(2)
        << std::setfill('0') << (int) state::status << " ("
        << std::dec << (int) state::status << ") ";
    std::bitset<8> leds(state::status);
    for (size_t i = 0; i < 8; ++i)
    {
        if (leds[i])
            ss << "#";
        else
            ss << "-";
    }
    logging::announce(ss.str(), true, true);
}}},

{transmission::getId("/ground/fill-nitrous"), {"FILL NITROUS <uint8_t percent>",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 1)
    {
        uint8_t percent = data[0];
        if (percent > 100) percent = 100;
        std::stringstream ss;
        ss << "Filling the nitrous tank to "
            << std::dec << (int) percent << "%";
        logging::announce(ss.str(), true, true);
    }
}}},

{transmission::getId("/ground/disc-feedline"), {"DISCONNECT FEED LINE",
[] (std::vector<uint8_t>)
{
    logging::announce("Disconnecting feed line... (TODO)", true, true);
    hardware::disconnectFeedLine();
}}},

{transmission::getId("/ground/bleed-nitrous"), {"BLEED NITROUS",
[] (std::vector<uint8_t>)
{
    logging::announce("Bleeding nitrous... (TODO)", true, true);
}}},

{transmission::getId("/ground/abort"), {"ABORT (WARNING: VERY UNGRACEFUL)",
[] (std::vector<uint8_t>)
{
    std::abort();
}}},

{transmission::getId("/ground/reset"), {"RESET",
[] (std::vector<uint8_t>)
{
    control::reset();
}}},

{transmission::getId("/ground/shutdown"), {"SHUTDOWN [uint8_t code = 0]",
[] (std::vector<uint8_t> data)
{
    uint8_t code = 0;
    if (data.size() > 0) code = data[0];
    control::exit(code);
}}},

}; // on_receive

void dataReceipt(uint8_t id, const std::vector<uint8_t> &data)
{
    auto action = on_receive.find(id);
    if (action != on_receive.end())
    {
        std::stringstream ss;
        ss << "Recieve command: 0x"
            << std::hex << (int) id << " ("
            << action->second.description << ")";
        logging::announce(ss.str(), true, true);
        action->second.function(data);
    }
    else
    {
        std::stringstream ss;
        ss << "It's free real estate: 0x"
            << std::hex << (int) id;
        logging::announce(ss.str(), true, true);
    }
}

} // namespace behavior

