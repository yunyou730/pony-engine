#pragma once

namespace pony {

class Log {
public:
    static void i(const char *fmt, ...);
    static void w(const char *fmt, ...);
    static void e(const char *fmt, ...);
};

} // namespace pony
