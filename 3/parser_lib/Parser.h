#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <string_view>

class ParserBuilder;

class Parser {
   public:
    friend class ParserBuilder;
    static ParserBuilder Make();
    enum class TokType { STRING, DIGIT };
    void Parse(std::string_view p_str);
    Parser() = default;

   private:
    std::function<void(void)> beg_callback_;
    std::function<void(std::string_view)> digit_calback_;
    std::function<void(std::string_view)> string_calback_;
    std::function<void()> end_calback_;

    TokType cur_type_{};
    std::string_view parsed_str_{};
    std::string_view::iterator beg_tok_{};
    std::string_view::iterator cur_it_{};
    bool new_tok_{};

    void UpdateDigit(std::string_view::iterator cur_it);
    void UpdateChar(std::string_view::iterator cur_it);
    void CallCallback();
};

class ParserBuilder {
   public:
    friend class Parser;

    ParserBuilder& AddBegCalback(const std::function<void(void)>& beg_callback);

    ParserBuilder& AddDigCalback(
        const std::function<void(std::string_view)>& digit_calback);

    ParserBuilder& AddStrCalback(
        const std::function<void(std::string_view)>& string_calback);

    ParserBuilder& AddEndCalback(const std::function<void(void)>& end_calback);

    ParserBuilder(const ParserBuilder&) = delete;
    ParserBuilder& operator=(const ParserBuilder&) = delete;
    ParserBuilder(ParserBuilder&&) noexcept = default;
    ParserBuilder& operator=(ParserBuilder&&) noexcept = default;

    Parser Build();

   private:
    ParserBuilder() = default;
    Parser parser_{};
};
