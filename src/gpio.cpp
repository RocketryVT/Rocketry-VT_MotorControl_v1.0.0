#include <gpio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <string>

const std::string SYSFS_GPIO_DIR = "/sys/class/gpio";
const std::string SYSFS_ADC_DIR = "/sys/bus/iio/devices/iio:device0";
const uint8_t MAX_BUF = 64;
const uint16_t POLL_TIMEOUT = 3 * 1000; /* 3 seconds */
const std::string LED_DIR = "/sys/class/leds/beaglebone:green:usr";

namespace gpio
{

int led_set_value(unsigned int led, bool value)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (LED_DIR +
        "%d/brightness").c_str(), led);
    int fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("led/set-value");
        return fd;
    }

    write(fd, value ? "1" : "0", 2);
    close(fd);
    return 0;
}

int export_gpio(unsigned int gpio)
{
    char buf[MAX_BUF];
    int fd = open((SYSFS_GPIO_DIR +
        "/export").c_str(), O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/export");
        return fd;
    }
    int len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

int unexport_gpio(unsigned int gpio)
{
    char buf[MAX_BUF];
    int fd = open((SYSFS_GPIO_DIR +
        "/unexport").c_str(), O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/export");
        return fd;
    }

    int len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

int set_dir(unsigned int gpio, bool out_flag)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (SYSFS_GPIO_DIR +
        "/gpio%d/direction").c_str(), gpio);
    int fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/direction");
        return fd;
    }

    write(fd, out_flag ? "out" : "in", 4);
    close(fd);
    return 0;
}

int set_value(unsigned int gpio, bool value)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (SYSFS_GPIO_DIR +
        "/gpio%d/value").c_str(), gpio);
    int fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/set-value");
        return fd;
    }
    write(fd, value ? "1" : "0", 2);
    close(fd);
    return 0;
}

int get_value(unsigned int gpio, unsigned int &value)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (SYSFS_GPIO_DIR +
        "/gpio%d/value").c_str(), gpio);
    int fd = open(buf, O_RDONLY);
    if (fd < 0)
    {
        perror("gpio/get-value");
        return fd;
    }

    int ch;
    read(fd, &ch, 1);
    if (ch != '0')
    {
        value = 1;
    }
    else
    {
        value = 0;
    }
    close(fd);
    return 0;
}

int set_edge(unsigned int gpio, std::string &edge)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (SYSFS_GPIO_DIR + "/gpio%d/edge").c_str(), gpio);
    int fd = open(buf, O_WRONLY);
    if (fd < 0)
    {
        perror("gpio/set-edge");
        return fd;
    }
    write(fd, edge.c_str(), edge.length() + 1);
    close(fd);
    return 0;
}

int fd_open(unsigned int gpio)
{
    char buf[MAX_BUF];
    snprintf(buf, sizeof(buf), (SYSFS_GPIO_DIR + "/gpio%d/value").c_str(), gpio);
    int fd = open(buf, O_RDONLY | O_NONBLOCK );
    if (fd < 0) perror("gpio/fd_open");
    return fd;
}

int fd_close(int fd)
{
    return close(fd);
}

} // namespace gpio

namespace adc
{

int read_value(unsigned int adc, unsigned int &value)
{
    int fd;
    char buf[MAX_BUF];
    char retChars[5];

    snprintf(buf, sizeof(buf), (SYSFS_ADC_DIR +
        "/in_voltage%d_raw").c_str(), adc);

    fd = open(buf, O_RDONLY);
    if (fd < 0)
    {
        perror("adc/get-value");
        perror(buf);
        return fd;
    }

    read(fd, retChars, 6);
    value = strtol(retChars,NULL,0);
    close(fd);
    return 0;
}

int fd_open(unsigned int adc)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), (SYSFS_ADC_DIR +
        "/in_voltage%d_raw").c_str(), adc);

    fd = open(buf, O_RDONLY | O_NONBLOCK );
    if (fd < 0) perror("adc/fd_open");
    return fd;
}

int fd_close(int fd)
{
    return close(fd);
}

} // namespace adc

