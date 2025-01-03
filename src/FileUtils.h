#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#include <windows.h>
#endif

class FileUtils {
private:

#ifdef __APPLE__
    static const char PATH_SEPARATOR = '/';
#else
    static const char PATH_SEPARATOR = '\\';
#endif

public:
    static std::string GetCurrentWorkingDirectory() {
#ifdef __APPLE__
        char buffer[PATH_MAX];
        uint32_t size = PATH_MAX;
        if (_NSGetExecutablePath(buffer, &size) != 0) {
            throw std::runtime_error("Failed to retrieve executable path");
        }
        std::string fullPath(buffer);
        return fullPath.substr(0, fullPath.find_last_of("/")); // Remove executable name
#else
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string fullPath(buffer);
        return fullPath.substr(0, fullPath.find_last_of("\\/")); // Remove executable name
#endif
    }

    static std::string CombinePath(const std::string& path1, const std::string& path2) {
        return path1 + PATH_SEPARATOR + path2;
    }
};

#endif // FILE_UTILS_H
