#pragma once
#include <charconv>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <variant>

class ParserBuilder;

class Parser {
   public:
    using VoidCallback = std::function<void(void)>;
    using StringCallback = std::function<void(std::string_view)>;
    using DigitCallback = std::function<void(int)>;

    friend class ParserBuilder;
    static ParserBuilder Make();
    enum class TokType { STRING, DIGIT };
    void Parse(std::string_view p_str);
    Parser() = default;

   private:
    Parser::VoidCallback beg_callback_;
    Parser::DigitCallback digit_callback_;
    Parser::StringCallback string_callback_;
    Parser::VoidCallback end_callback_;

    TokType cur_type_{};
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

    ParserBuilder& AddBegCallback(const Parser::VoidCallback& beg_callback);
    ParserBuilder& AddDigCallback(const Parser::DigitCallback& digit_calback);
    ParserBuilder& AddStrCallback(const Parser::StringCallback& string_calback);
    ParserBuilder& AddEndCallback(const Parser::VoidCallback& end_calback);

    ParserBuilder(const ParserBuilder&) = delete;
    ParserBuilder& operator=(const ParserBuilder&) = delete;
    ParserBuilder(ParserBuilder&&) noexcept = default;
    ParserBuilder& operator=(ParserBuilder&&) noexcept = default;

    Parser Build();

   private:
    ParserBuilder() = default;
    Parser parser_{};
};
