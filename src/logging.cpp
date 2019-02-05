#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <deque>
#include <fstream>
#include <numeric>

#include <logging.h>
#include <transmission.h>

namespace logging
{

std::deque<uint8_t> buffer;
std::ofstream logfile;
bool fail_flag = false;

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
        << std::setw(2) << std::setfill('0') << utc.tm_sec << ".bin";

    #ifdef DEBUG
    std::cout << "Begin log: " << filename.str() << std::endl;
    #endif

    logging::write(transmission::buildPacket("VT ROCKETRY IS BEST ROCKETRY!"));

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

void exit(int)
{
    logfile.close();
}

void reset()
{
    flush();
    exit(0);
    init();
}

void write(const std::vector<uint8_t> &data)
{
    for (auto e : data)
        buffer.push_back(e);
}

void flush()
{
    while (buffer.size() > 0)
    {
        uint8_t e = buffer.front();
        logfile << e;
        buffer.pop_front();
    }
}

} // namespace logging

