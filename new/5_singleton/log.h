#ifndef __LOG_H__
#define __LOG_H__

#include <chrono>
#include <deque>
#include <iostream>
#include <ostream>
#include <string>

enum LOG_LEVEL { LOG_NORMAL, LOG_WARN, LOG_ERROR };

class Log {
public:
  inline static size_t max_size = 10;

private:
  Log() = default;
  ~Log() = default; //
  inline static Log *first_instance = nullptr;
  inline static std::deque<std::pair<LOG_LEVEL, std::string>> msgs = {};

public:
  static Log *Instance() {
    if (!first_instance)
      first_instance = new Log();

    return first_instance;
  }
  static void message(LOG_LEVEL lvl, std::string msg) {
    auto now = std::chrono::system_clock::now();
    msgs.push_back({lvl, std::format("{0:%F_%T}]: ", now) + msg});
    if (msgs.size() > max_size) {
      msgs.pop_front();
    }
  }
  static void print() {
    for (auto &msg : msgs) {
      switch (msg.first) {
      case LOG_LEVEL::LOG_NORMAL:
        std::cerr << "INFO [";
        break;
      case LOG_LEVEL::LOG_WARN:
        std::cerr << "WARN [";
        break;
      case LOG_LEVEL::LOG_ERROR:
        std::cerr << "ERROR[";
        break;
      }
      std::cerr << msg.second << std::endl;
    }
  }
};

#endif //!__LOG_H__
