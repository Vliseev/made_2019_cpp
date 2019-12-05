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

template <size_t I>
struct VisitImpl {
    template <typename T, typename F>
    static void Visit(T& tup, size_t idx, F fun) {
        if (idx == I - 1) {
            fun(std::get<I - 1>(tup));
        } else {
            VisitImpl<I - 1>::Visit(tup, idx, fun);
        }
    }
};

template <>
struct VisitImpl<0> {
    template <typename T, typename F>
    static void Visit(T& tup, size_t idx, F fun) {
        throw std::runtime_error("invalid syntax");
    }
};

template <typename F, typename... Ts>
void VisitAt(std::tuple<Ts...> const& tup, size_t idx, F fun) {
    VisitImpl<sizeof...(Ts)>::Visit(tup, idx, fun);
}

template <typename F, typename... Ts>
void VisitAt(std::tuple<Ts...>& tup, size_t idx, F fun) {
    VisitImpl<sizeof...(Ts)>::Visit(tup, idx, fun);
}

}    // namespace

template <typename... Args>
std::string Format(const char* s, Args&&... args) {
    auto ftms_len = strlen(s);
    std::ostringstream oss;

    auto args_tuple = std::make_tuple(args...);
    auto arg_size = sizeof...(Args);
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
            if (idx > arg_size)
                throw std::runtime_error("invalid syntax");

            VisitAt(args_tuple, idx, [&oss](const auto& el) { oss << el; });
            beg_fmt = -1;

        } else if (beg_fmt == -1) {
            oss << s[i];
        }
    }

    return oss.str();
}