#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#include "FileSystem.hpp"


#if defined(_WIN32) || defined(__WIN32__)
    // Windows
    #define SYS_WINDOWS
    #include <direct.h>     // _mkdir

#elif defined(linux) || defined(__linux)
    // Linux
    #define SYS_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    // Mac OS
    #define SYS_MACOS

#endif


bool FileSystem::isDirectory(const std::string& path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) == 0)
    {
        return (buf.st_mode & S_IFDIR) != 0;
    }
    return false;
}


bool FileSystem::createDirectory(const std::string& name)
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


std::string FileSystem::initSettingsDirectory(const std::string& appname)
{
    std::string config_dir;

    // Set config directory path
#ifdef SYS_WINDOWS
    config_dir = getenv("APPDATA");
    config_dir += "\\" + appname;

#elif defined(SYS_LINUX)
    config_dir = getenv("HOME");
    config_dir += "/.config/" + appname;

#elif defined(SYS_MACOS)
    config_dir = getenv("HOME");
    config_dir += "/Library/Application Support/" + appname;

#endif

    // Create directory if it doesn't already exist
    if (!isDirectory(config_dir))
    {
        createDirectory(config_dir);
    }
    return config_dir;
}


