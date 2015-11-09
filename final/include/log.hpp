#ifndef LOG_HPP_
#define LOG_HPP_

#include "config.hpp"

namespace LOG
{

extern bool open();
extern void close();

extern void info(const char *fmt, ...);
extern void warn(const char *fmt, ...);
extern void error(const char *fmt, ...);

}

#endif // LOG_HPP_
