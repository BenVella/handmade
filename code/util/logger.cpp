#pragma once
#include <cstdarg>
#include <cstdio>
#include <string_view>

enum class LogLevel { Info, Warn, Error };

inline void log(LogLevel level, std::string_view msg) {
  const char *prefix = (level == LogLevel::Info)   ? "[INFO] "
                       : (level == LogLevel::Warn) ? "[WARN] "
                                                   : "[ERROR] ";

  std::fprintf(stderr, "%s%.*s\n", prefix, static_cast<int>(msg.size()),
               msg.data());
}

inline void logf(LogLevel level, const char *fmt, ...) {
  const char *prefix = (level == LogLevel::Info)   ? "[INFO] "
                       : (level == LogLevel::Warn) ? "[WARN] "
                                                   : "[ERROR] ";

  std::fprintf(stderr, "%s", prefix);

  va_list args;
  va_start(args, fmt);
  std::vfprintf(stderr, fmt, args);
  va_end(args);

  std::fprintf(stderr, "\n");
}
