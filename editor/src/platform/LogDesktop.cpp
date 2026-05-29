#include "Core/Log.h"
#include <cstdio>
#include <cstdarg>

namespace pony {

void Log::i(const char *fmt, ...) {
    printf("[INFO] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void Log::w(const char *fmt, ...) {
    printf("[WARN] ");
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
}

void Log::e(const char *fmt, ...) {
    fprintf(stderr, "[ERROR] ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

} // namespace pony
