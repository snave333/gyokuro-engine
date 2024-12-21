
#include <string>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#else
#include <windows.h>
#endif

std::string GetCurrentWorkingDirectory() {
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

#ifdef __APPLE__
    const char PATH_SEPARATOR = '/';
#else
    const char PATH_SEPARATOR = '\\';
#endif

std::string CombinePath(const std::string& path1, const std::string& path2) {
    return path1 + PATH_SEPARATOR + path2;
}
