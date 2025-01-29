#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#include <windows.h>
#endif

namespace gyo {

class FileSystem {
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

    static std::string CombinePath(const std::string& path1, const std::string& path2, const std::string& path3) {
        return path1 + PATH_SEPARATOR + path2 + PATH_SEPARATOR + path3;
    }

    static std::string CombinePath(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4) {
        return path1 + PATH_SEPARATOR + path2 + PATH_SEPARATOR + path3 + PATH_SEPARATOR + path4;
    }

    static std::string GetFileName(const char* filePath) {
        return GetFileName(std::string(filePath));
    }

    static std::string GetFileName(std::string filePath) {
        // find the position of the last directory separator ('/' or '\')
        size_t pos = filePath.find_last_of("/\\");
        
        // extract and return the substring after the last separator
        return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
    }
};

} // namespace gyo

#endif // FILE_UTILS_H
