//
// Created by user on 17.11.2019.
//
// serializer.h
#pragma once
#include <charconv>

enum class Error { NO_ERROR, CORRUPTED_ARCHIVE };

class Serializer {
    static constexpr char Separator = ' ';
    std::ostream& out_;

   public:
    explicit Serializer(std::ostream& out) : out_(out) {
    }

    template <class T>
    Error Save(T object) {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args) {
        return Process(args...);
    }

   private:
    // Process использует variadic templates
    template <class T>
    Error Process(T val) {
        SaveVal(val);
        if (out_)
            return Error::NO_ERROR;
        else
            return Error::CORRUPTED_ARCHIVE;
    }

    template <class T, class... ArgsT>
    Error Process(T val, ArgsT... args) {
        SaveVal(val);
        out_ << Separator;
        if (out_)
            return Process(args...);
        else
            return Error::CORRUPTED_ARCHIVE;
    }

    void SaveVal(uint64_t value) {
        out_ << value;
    }

    void SaveVal(bool value) {
        if (value)
            out_ << "true";
        else
            out_ << "false";
    }
};

class Deserializer {
    static constexpr char Separator = ' ';
    std::istream& in_;

   public:
    explicit Deserializer(std::istream& in) : in_(in) {
    }

    template <class T>
    Error Load(T& object) {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) {
        return Process(args...);
    }

   private:
    template <class T>
    Error Process(T& val) {
        if (LoadVal(val) == Error::NO_ERROR) {
            return Error::NO_ERROR;
        } else {
            return Error::CORRUPTED_ARCHIVE;
        }
    }

    template <class T, class... ArgsT>
    Error Process(T& val, ArgsT&... args) {
        if (LoadVal(val) == Error::NO_ERROR) {
            return Process(args...);
        } else {
            return Error::CORRUPTED_ARCHIVE;
        }
    }

    Error LoadVal(bool& value) {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CORRUPTED_ARCHIVE;

        return Error::NO_ERROR;
    }

    Error LoadVal(uint64_t& value) {
        std::string text;
        in_ >> text;

        if (auto [p, ec] =
                std::from_chars(text.data(), text.data() + text.size(), value);
            ec == std::errc()) {
            return Error::NO_ERROR;
        } else {
            return Error::CORRUPTED_ARCHIVE;
        }
    }
};