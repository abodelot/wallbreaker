#include "Os.hpp"
#include <cstdlib>

#if defined(_WIN32) || defined(__WIN32__) // Windows
#define SYS_WINDOWS
#elif defined(__linux__) // Linux
#define SYS_LINUX
#elif defined(__APPLE__) // Mac OS
#define SYS_MACOS
#endif

namespace os
{

std::filesystem::path config_path(const std::string& appname)
{
#ifdef SYS_WINDOWS
    return std::string(std::getenv("APPDATA")) + "\\" + appname;
#elif defined(SYS_LINUX)
    return std::string(std::getenv("HOME")) + "/.config/" + appname;
#elif defined(SYS_MACOS)
    return std::string(std::getenv("HOME")) + "/Library/Application Support/" + appname;
#endif
}

}
