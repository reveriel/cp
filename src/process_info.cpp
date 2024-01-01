
#include "process_info.h"
#include "unistd.h"
namespace cp {

namespace process_info {

pid_t pid() { return getpid(); }

std::string pid_str() {
  char buf[32];
  snprintf(buf, sizeof(buf), "%d", pid());
  return buf;
}

std::string hostname() {

  char buf[256];
  if (gethostname(buf, sizeof(buf)) == 0) {
    return buf;
  } else {
    return "unknown";
  }
}

} // namespace process_info

} // namespace cp