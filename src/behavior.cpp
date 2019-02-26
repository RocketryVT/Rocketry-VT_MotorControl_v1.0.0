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

{0x01, {"MARCO",
[] (std::vector<uint8_t>)
{
    logging::announce("POLO", true, true);
    state::last_ping = std::chrono::steady_clock::now();
}}},

{0x02, {"ECHO LEXICON",
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

    logging::announce("Lexicon query:", true, true);
    for (auto e : on_receive)
        logging::announce(lexMsg(e.first, e.second.description), true, true);
}}},

{0x03, {"ECHO LOGLIST",
[] (std::vector<uint8_t>)
{
    auto logMsg = [] (uint8_t id, const std::chrono::milliseconds &dur,
        const std::string &desc)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id << std::dec
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

{0x04, {"ECHO RECIPES",
[] (std::vector<uint8_t>)
{
    auto recipeMsg = [] (uint8_t id, const std::string &str)
    {
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(2)
            << std::setfill('0') << (int) id
            << ": " << str << std::dec;
        return ss.str();
    };

    logging::announce("Recipe query:", true, true);
    for (auto e : logging::recipes())
    {
        logging::announce(recipeMsg(e.first,
            e.second.description), true, true);
    }
}}},

{0x08, {"UNLOG [uint8_t id = ALL]",
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

{0x09, {"LOG <uint8_t id> [uint16_t millis = 1000]",
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

{0x18, {"SET LOCK [uint8_t lockstate = 0]",
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

{0x23, {"ASCII message packet.",
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

{0x30, {"SET STATUS <uint8_t status>",
[] (std::vector<uint8_t> data)
{
    if (data.size() == 1)
        state::status = data[0];
}}},

{0x31, {"ECHO STATUS",
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

{0x50, {"FILL NITROUS <uint8_t percent>",
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

{0x51, {"DISCONNECT FEED LINE",
[] (std::vector<uint8_t>)
{
    logging::announce("Disconnecting feed line... (TODO)", true, true);
    hardware::disconnectFeedLine();
}}},

{0x55, {"BLEED NITROUS",
[] (std::vector<uint8_t>)
{
    logging::announce("Bleeding nitrous... (TODO)", true, true);
}}},

{0x60, {"CONTINUITY TEST.",
[] (std::vector<uint8_t>)
{
    logging::announce("Testing continuity...", true, true);
    bool success = predicates::continuityOk();
    if (success)
        logging::announce("Continuity is good.", true, true);
    else
        logging::announce("Continuity is not good.", true, true);
}}},

{126, {"RESET",
[] (std::vector<uint8_t>)
{
    control::reset();
}}},

{127, {"SHUTDOWN <uint8_t code>",
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

