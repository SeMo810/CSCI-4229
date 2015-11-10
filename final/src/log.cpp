#include "log.hpp"

#define LOG_BUFFER_SIZE 256

static std::ofstream *g_logfile = nullptr;

static String getTimeStamp()
{
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[64];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, 64, "%H:%M:%S", timeinfo);
  return String(buffer);
}


namespace LOG
{

bool open()
{
  std::ofstream &file = *(g_logfile = new std::ofstream());
  std::ios_base::iostate emask = file.exceptions() | std::ios::failbit;
  file.exceptions(emask); // Allow exceptions

  try
  {
    file.open("applog.txt", std::ios::out | std::ios::trunc);
  }
  catch (std::ios_base::failure &e)
  {
    fprintf(stderr, "Logging output file could not be opened. Reason: \"%s\".", e.what());
    delete g_logfile;
    g_logfile = nullptr;
    return false;
  }

  emask &= (~std::ios::failbit);
  file.exceptions(emask); // Disallow exceptions
  return true;
}

void close()
{
  if (!g_logfile)
    return;

  g_logfile->flush();
  g_logfile->close();
  delete g_logfile;
  g_logfile = nullptr;
}

void info(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buffer[LOG_BUFFER_SIZE];
  vsprintf(buffer, fmt, args);
  va_end(args);

  String message = "[INFO][" + getTimeStamp() + "]:   " + buffer + '\n';
  if (g_logfile)
    *(g_logfile) << message;
  fprintf(stdout, message.c_str());
}

void warn(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buffer[LOG_BUFFER_SIZE];
  vsprintf(buffer, fmt, args);
  va_end(args);

  String message = "[WARN][" + getTimeStamp() + "]:   " + buffer + '\n';
  if (g_logfile)
    *(g_logfile) << message;
  fprintf(stdout, message.c_str());
}

void error(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buffer[LOG_BUFFER_SIZE];
  vsprintf(buffer, fmt, args);
  va_end(args);

  String message = "[ERR ][" + getTimeStamp() + "]:   " + buffer + '\n';
  if (g_logfile)
    *(g_logfile) << message;
  fprintf(stdout, message.c_str());
}

}
