#include "FileSystem.hpp"

#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>


#if defined(_WIN32) || defined(__WIN32__)
// Windows
#define SYS_WINDOWS
#include <direct.h> // _mkdir
#elif defined(__linux__)
// Linux
#define SYS_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
// Mac OS
#define SYS_MACOS
#endif

namespace filesystem
{

std::string dirname(const std::string& path)
{
    size_t last_separator = path.find_last_of("/\\");
    std::string result = path.substr(0, last_separator + 1);
    return result;
}


bool is_directory(const std::string& path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) == 0)
    {
        return (buf.st_mode & S_IFDIR) != 0;
    }
    return false;
}


bool create_directory(const std::string& name)
{
    bool success = false;

#ifdef SYS_WINDOWS
    success = _mkdir(name.c_str()) == 0;
#elif defined(SYS_LINUX) || defined(SYS_MACOS)
    success = mkdir(name.c_str(), 0755) == 0;
#endif

    if (!success)
        std::cerr << "cannot create directory " << name << std::endl;
    return success;
}


std::string init_app_directory(const std::string& appname)
{
    std::string app_dir;

    // Set config directory path
#ifdef SYS_WINDOWS
    app_dir = std::string(std::getenv("APPDATA")) + "\\" + appname;
#elif defined(SYS_LINUX)
    app_dir = std::string(std::getenv("HOME")) + "/.config/" + appname;
#elif defined(SYS_MACOS)
    app_dir = std::string(std::getenv("HOME")) + "/Library/Application Support/"
        + appname;
#endif

    // Create directory if it doesn't already exist
    if (!is_directory(app_dir))
    {
        create_directory(app_dir);
    }
    return app_dir;
}

}
