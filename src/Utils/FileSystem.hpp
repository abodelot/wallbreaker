#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

namespace filesystem {

/**
 * Strips last component from path
 */
std::string dirname(const std::string& path);

/**
 * Checks if the given path corresponds to a directory.
 * @return true if the file indicated refers to a directory, false otherwise
 */
bool is_directory(const std::string& path);

/**
 * Creates a directory.
 * @return true if a directory was created, false otherwise
 */
bool create_directory(const std::string& dir);

/**
 * Ensures application settings directory is created.
 * @param appname: name of application settings directory
 * @return path to directory, according to OS specifications
 */
std::string init_app_directory(const std::string& appname);

}

#endif // FILESYSTEM_HPP
