#ifndef PEAR_CLI_FORMAT_H
#define PEAR_CLI_FORMAT_H

#ifdef PEAR_WIN
#include <format>
#endif

#include <fmt/format.h>
#include <string>

namespace pear {

#ifdef PEAR_WIN
    namespace fmtlib = std;
#else
    namespace fmtlib = fmt;
#endif

    template<typename... T>
    constexpr auto format(fmtlib::format_string<T...> fmt, T &&...args) {
        return fmtlib::format(fmt, args...);
    }

}

#endif // PEAR_CLI_FORMAT_H
