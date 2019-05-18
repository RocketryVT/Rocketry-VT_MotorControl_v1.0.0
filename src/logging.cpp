#include <iostream>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <deque>
#include <fstream>
#include <numeric>
#include <map>

#include <logging.h>
#include <comms.h>
#include <transmission.h>

#include <config.h>

namespace logging
{

std::deque<uint8_t> buffer;
std::ofstream logfile;
bool fail_flag = false;

std::map<uint8_t, std::chrono::milliseconds> logs;
std::map<uint8_t, std::chrono::steady_clock::time_point> last_sent;
std::map<uint8_t, recipe> recipe_book;

bool init()
{
    // timestamps will be milliseconds since unix epoch
    auto now = std::chrono::system_clock::now();
    time_t ymd = std::chrono::system_clock::to_time_t(now);
    tm utc = *gmtime(&ymd);

    std::stringstream filename;
    filename << "log/LOG-" << (utc.tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (utc.tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << utc.tm_mday << "-"
        << std::setw(2) << std::setfill('0') << utc.tm_hour << "-"
        << std::setw(2) << std::setfill('0') << utc.tm_min << "-"
        << std::setw(2) << std::setfill('0') << utc.tm_sec << ".rvt";

    logging::announce("Begin log: " + filename.str(), true, true);

    std::string command("echo '" + filename.str() + "' > .lastlog");
    system(command.c_str());
    system("mkdir -p log/");

    logfile.open(filename.str());
    if (!logfile)
        fail_flag = true;

    return ok();
}

bool ok()
{
    return !fail_flag;
}

void loop()
{
    for (auto log : logs)
    {
        uint8_t logID = log.first;
        // check if a recipe exists for this log
        auto recipe = recipe_book.find(logID);
        if (recipe != recipe_book.end())
        {
            auto now = std::chrono::steady_clock::now();
            // only send this log if the log period has passed
            if (last_sent.find(logID) == last_sent.end() ||
                now - last_sent[logID] >= log.second)
            {
                auto packet = recipe->second.function();
                logging::announce(packet, true, false);
                if (last_sent.find(logID) != last_sent.end())
                    last_sent[logID] += log.second;
                else
                    last_sent.insert(std::make_pair(logID, now));
            }
        }
    }

    comms::flush();
    logging::flush();
}

void exit(int)
{
    clearLogs();
    flush();
    logfile.close();
}

bool reset()
{
    exit(0);
    return init();
}

void write(const std::vector<uint8_t> &data)
{
    for (auto e : data)
        buffer.push_back(e);
}

void write(const std::string &str)
{
    write(transmission::buildPacket(str));
}

void flush()
{
    while (buffer.size() > 0)
    {
        uint8_t e = buffer.front();
        logfile << e;
        buffer.pop_front();
    }
    logfile.flush();
}

void addLog(uint8_t id, const std::chrono::milliseconds &dur)
{
    auto ret = logs.insert(std::make_pair(id, dur));
    if (!ret.second)
    {
        logs[id] = dur;
    }
}

void removeLog(uint8_t id)
{
    logs.erase(id);
    last_sent.erase(id);
}

void clearLogs()
{
    logs.clear();
    last_sent.clear();
}

const std::map<uint8_t, std::chrono::milliseconds>& loglist()
{
    return logs;
}

void addRecipe(uint8_t id, const std::string &desc,
    const std::function<std::vector<uint8_t>(void)> &func)
{
    recipe r { desc, func };
    auto ret = recipe_book.insert(std::make_pair(id, r));
    if (!ret.second)
    {
        recipe_book[id] = r;
    }
}

void removeRecipe(uint8_t id)
{
    recipe_book.erase(id);
}

const std::map<uint8_t, recipe>& recipes()
{
    return recipe_book;
}

void announce(const std::string &str, bool radio, bool console)
{
    logging::write(str);
    // if (radio)
    {
        comms::transmit(str);
    }
    if (console)
    {
        std::cout << str << std::endl;
    }
}

void announce(const std::vector<uint8_t> &packet, bool radio, bool console)
{
    logging::write(packet);
    if (radio)
    {
        comms::transmit(packet);
    }
    if (console)
    {
        std::cout << transmission::packet2str(packet) << std::endl;
    }
}

} // namespace logging

