
#include <pin_driver.hpp>
#include <gpio.hpp>

namespace hardware
{

const std::vector<gpio_pin> all_gpios
{
    gpio_pin::P8_3,   gpio_pin::P8_4,
    gpio_pin::P8_5,   gpio_pin::P8_6,
    gpio_pin::P8_7,   gpio_pin::P8_8,
    gpio_pin::P8_9,   gpio_pin::P8_10,
    gpio_pin::P8_11,  gpio_pin::P8_12,
    gpio_pin::P8_13,  gpio_pin::P8_14,
    gpio_pin::P8_15,  gpio_pin::P8_16,
    gpio_pin::P8_17,  gpio_pin::P8_18,
    gpio_pin::P8_19,  gpio_pin::P8_20,
    gpio_pin::P8_21,  gpio_pin::P8_22,
    gpio_pin::P8_23,  gpio_pin::P8_24,
    gpio_pin::P8_25,  gpio_pin::P8_26,
    gpio_pin::P8_27,  gpio_pin::P8_28,
    gpio_pin::P8_29,  gpio_pin::P8_30,
    gpio_pin::P8_31,  gpio_pin::P8_32,
    gpio_pin::P8_33,  gpio_pin::P8_34,
    gpio_pin::P8_35,  gpio_pin::P8_36,
    gpio_pin::P8_37,  gpio_pin::P8_38,
    gpio_pin::P8_39,  gpio_pin::P8_40,
    gpio_pin::P8_41,  gpio_pin::P8_42,
    gpio_pin::P8_43,  gpio_pin::P8_44,
    gpio_pin::P8_45,  gpio_pin::P8_46,

    gpio_pin::P9_11,  gpio_pin::P9_12,
    gpio_pin::P9_13,  gpio_pin::P9_14,
    gpio_pin::P9_15,  gpio_pin::P9_16,
    gpio_pin::P9_17,  gpio_pin::P9_18,

    gpio_pin::P9_21,  gpio_pin::P9_22,
    gpio_pin::P9_23,  gpio_pin::P9_24,
    gpio_pin::P9_25,  gpio_pin::P9_26,
    gpio_pin::P9_27,  gpio_pin::P9_28,
    gpio_pin::P9_29,  gpio_pin::P9_30,
    gpio_pin::P9_31,

    gpio_pin::P9_41,  gpio_pin::P9_42
};

const std::vector<adc_pin> all_adcs
{
    adc_pin::P9_33,
    adc_pin::P9_35,   adc_pin::P9_36,
    adc_pin::P9_37,   adc_pin::P9_38,
    adc_pin::P9_39,   adc_pin::P9_40
};

pin_driver::pin_driver()
{
    for (gpio_pin g : all_gpios)
    {
        uint8_t id = static_cast<uint8_t>(g);
        gpio::export_gpio(id);
        write(g, false);
    }
}

pin_driver::~pin_driver()
{
    for (gpio_pin g : all_gpios)
    {
        write(g, false);
    }
}

bool pin_driver::read(gpio_pin g)
{
    uint8_t id = static_cast<uint8_t>(g);
    gpio::set_dir(id, false);
    unsigned int value;
    gpio::get_value(id, value);
    return value > 0;
}

void pin_driver::write(gpio_pin g, bool state)
{
    uint8_t id = static_cast<uint8_t>(g);
    gpio::set_dir(id, true);
    gpio::set_value(id, state);
}

double pin_driver::read(adc_pin a)
{
    uint8_t id = static_cast<uint8_t>(a);
    unsigned int reference, value;
    adc::read_value(7, reference);
    adc::read_value(id, value);
    return value*1.8/reference;
}

const std::vector<gpio_pin>& pin_driver::gpios()
{
    return all_gpios;
}

const std::vector<adc_pin>& pin_driver::adcs()
{
    return all_adcs;
}

} // namespace hardware
