#ifndef RVT_GPIO_HPP
#define RVT_GPIO_HPP

#include <string>

namespace gpio
{

int led_set_value(unsigned int led, bool value);

// int export(unsigned int gpio);

// int unexport(unsigned int gpio);

int set_dir(unsigned int gpio, bool out_flag);

int set_value(unsigned int gpio, bool value);

int get_value(unsigned int gpio, unsigned int &value);

int set_edge(unsigned int gpio, std::string &edge);

int fd_open(unsigned int gpio);

int fd_close(int fd);

} // namespace gpio

namespace adc
{

int read_value(unsigned int adc, unsigned int &value);

int fd_open(unsigned int adc);

int fd_close(int fd);

} // namespace adc

#endif /* SIMPLEGPIO_H_ */
