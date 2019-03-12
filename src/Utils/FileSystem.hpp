#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

/**
 * Static class for handling OS-specific paths
 */
class FileSystem
{
public:
    static bool isDirectory(const std::string& path);

    static bool createDirectory(const std::string& path);

    /**
     * Ensure settings directory is created
     * @param appname: name of application directory
     * @return path to settings directory
     */
    static std::string initSettingsDirectory(const std::string& appname);
};

#endif // FILESYSTEM_HPP
