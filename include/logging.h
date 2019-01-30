#ifndef LOGGING_H
#define LOGGING_H

#include <vector>
#include <string>

namespace logging
{

bool init();

bool ok();

void exit(int code);

void reset();

void write(const std::vector<uint8_t> &data);

void flush();

} // namespace logging

#endif // LOGGING_H
