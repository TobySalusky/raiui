//
// Created by Tobiathan on 12/16/22.
//

#pragma once

#include <string_view>
#include <iostream>

template <typename FmtType>
void Log(const FmtType& fmt) {
    std::cout << fmt << '\n';
}

template <typename FirstArg>
void Log(const std::string_view& fmt, FirstArg arg) {
    size_t i = fmt.find_first_of("{}");
    if (i == std::string::npos) { throw std::invalid_argument("`Log` does not use all arguments!"); }
    std::cout << fmt.substr(0, i) << arg;
    Log(fmt.substr(i + 2));
}

template <typename FirstArg, typename SecondArg, typename... Arguments>
void Log(const std::string_view& fmt, const FirstArg& arg1, const SecondArg& arg2, const Arguments&... args) {
    size_t i = fmt.find_first_of("{}");
    if (i == std::string::npos) { throw std::invalid_argument("`Log` does not use all arguments!"); }
    std::cout << fmt.substr(0, i) << arg1;
    Log(fmt.substr(i + 2), arg2, args...);
}