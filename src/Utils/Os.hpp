#ifndef OS_HPP
#define OS_HPP

#include <filesystem>
#include <string>

namespace os
{

/**
 * Get the OS-specific application configuration directory
 */
std::filesystem::path config_path(const std::string& appname);

}

#endif
