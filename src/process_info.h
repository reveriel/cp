#pragma once
#include <string>
#include <sys/unistd.h>

namespace cp {

namespace process_info {

pid_t pid();
std::string pid_str();

// uid_t uid();
// std::string username();

std::string hostname();

} // namespace process_info

} // namespace cp
