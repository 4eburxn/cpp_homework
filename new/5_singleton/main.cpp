#include "log.h"
int main() {
  Log *log = Log::Instance();
  log->message(LOG_NORMAL, "program loaded");
  log->message(LOG_ERROR, "error happens !help me !");
  log->print();
}
