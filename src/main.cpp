#include <iostream>
#include <signal.h>
#include <chrono>
#include <cmath>
#include <bitset>

#include <config.h>
#include <control.h>
#include <logging.h>
#include <comms.h>
#include <transmission.h>

void printLoop()
{
    const char icons[] = {'-', '\\', '|', '/'};
    static uint8_t index = 0;
    static auto last_called = state::time;
    std::cout << icons[index] << '\r' << std::flush;
    
    auto now = std::chrono::steady_clock::now();
    const auto period = std::chrono::milliseconds(250);
    if (now - last_called >= period)
    {
        index = (index + 1) % sizeof(icons);
        last_called = now;
    }
}

int main()
{
    signal(SIGINT, control::exit);

    if (!control::init())
    {
        logging::announce("Controller init error", true, true);
        return 1;
    }

    logging::addRecipe(0x00, "Vehicle state binary log.", [] ()
    {
        std::vector<uint8_t> data;
        uint8_t id = 200;
        data << (uint64_t) state::millis(state::time)
            << (uint8_t) state::status
            << (float) state::o2p
            << (float) state::o2t
            << (float) state::cp
            << (float) state::ct
            << (float) state::nh
            << (float) state::thrust;
        return transmission::buildPacket(id, data);
    });
    logging::addLog(0x00, std::chrono::milliseconds(500));

    while (control::ok())
    {
        control::loop();
        printLoop();
    }

    logging::announce("Exited successfully.", true, true);
    logging::flush();
    comms::flush();
    return 0;
}

