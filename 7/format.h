//
// Created by user on 02.12.2019.
//

#pragma once
#include <charconv>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <tuple>

namespace {
int GetIndex(int beg, int end, const char* s) {
    int result;
    if (auto [p, ec] = std::from_chars(s + beg, s + end, result);
        ec == std::errc()) {
        return result;
    } else {
        throw std::runtime_error("invalid syntax");
    }
}

template <class Tuple, std::size_t... Is>
void PrinTuple(std::ostream& os, const Tuple& t, std::index_sequence<Is...>) {
    ((os << (Is == 0 ? "" : " ") << std::get<Is>(t)), ...);
}

/*!
 * @brief example is taken from the
 * https://en.cppreference.com/w/cpp/utility/integer_sequence
 * @tparam Args
 * @param args
 * @return
 */
template <class... Args>
std::vector<std::string> ParseArgs(Args... args) {
    std::stringstream iss;
    std::vector<std::string> args_v(sizeof...(args));

    auto tup = std::make_tuple(args...);
    PrinTuple(iss, tup, std::index_sequence_for<Args...>{});
    std::string s;
    args_v.clear();

    while (iss >> s) {
        args_v.push_back(s);
    }

    return args_v;
}

}    // namespace

template <typename... Args>
std::string Format(const char* s, Args&&... args) {
    auto ftms_len = strlen(s);
    std::ostringstream oss;

    std::vector<std::string> args_v = ParseArgs(args...);
    int arg_size = args_v.size();
    int beg_fmt = -1;

    for (int i = 0; i < ftms_len; i++) {
        if (s[i] == '{') {
            if (beg_fmt != -1 || i == ftms_len - 1)
                throw std::runtime_error("invalid syntax");

            beg_fmt = i + 1;
        } else if (s[i] == '}') {
            if (beg_fmt == -1)
                throw std::runtime_error("invalid syntax");

            auto idx = GetIndex(beg_fmt, i, s);
            if (idx >= arg_size)
                throw std::runtime_error("invalid syntax");
            oss << args_v[idx];
            beg_fmt = -1;

        } else if (beg_fmt == -1) {
            oss << s[i];
        }
    }

    return oss.str();
}