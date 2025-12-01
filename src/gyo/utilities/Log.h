#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <chrono>
#include <ctime>

namespace gyo {

// return just the filename portion of a path
static inline const char* Basename(const char* path) {
    const char* file = path;
    for (const char* p = path; *p; ++p) {
        if (*p == '/' || *p == '\\') {
            file = p + 1;
        }
    }
    return file;
}

// fill buffer with local timestamp: "YYYY-MM-DD HH:MM:SS.mmm"
static inline void FormatTimestamp(char* out, size_t outSize) {
    using namespace std::chrono;

    const auto now      = system_clock::now();
    const auto ms_part  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    const std::time_t t = system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::snprintf(
        out, outSize,
        "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        (int)ms_part.count()
    );
}

// core macro with explicit level
#define LOG(level_cstr, fmt_cstr, ...)                                  \
    do {                                                                \
        char _ts[32];                                                   \
        FormatTimestamp(_ts, sizeof(_ts));                              \
        std::fprintf(stderr, "%s [%s] [%s:%d] ",                        \
                     _ts, (level_cstr), Basename(__FILE__), __LINE__);  \
        std::fprintf(stderr, (fmt_cstr), ##__VA_ARGS__);                \
        std::fprintf(stderr, "\n");                                     \
    } while (0)

// shorthands
#define LOGT(fmt, ...) LOG("TRACE", (fmt), ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG("DEBUG", (fmt), ##__VA_ARGS__)
#define LOGI(fmt, ...) LOG("INFO",  (fmt), ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG("WARN",  (fmt), ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG("ERROR", (fmt), ##__VA_ARGS__)

} // namespace gyo

#endif // LOG_H
