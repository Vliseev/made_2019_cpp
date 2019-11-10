#include "Parser.h"
#include <algorithm>
#include "likely.h"

ParserBuilder Parser::Make() {
    return ParserBuilder();
}
ParserBuilder& ParserBuilder::AddBegCallback(
    const Parser::VoidCallback& beg_callback) {
    parser_.beg_callback_ = beg_callback;
    return *this;
}
ParserBuilder& ParserBuilder::AddDigCallback(
    const Parser::DigitCallback& digit_calback) {
    parser_.digit_callback_ = digit_calback;
    return *this;
}
ParserBuilder& ParserBuilder::AddStrCallback(
    const Parser::StringCallback& string_calback) {
    parser_.string_callback_ = string_calback;
    return *this;
}
ParserBuilder& ParserBuilder::AddEndCallback(
    const Parser::VoidCallback& end_calback) {
    parser_.end_callback_ = end_calback;
    return *this;
}
Parser ParserBuilder::Build() {
    return std::move(parser_);
}

void Parser::UpdateDigit(std::string_view::iterator cur_it) {
    if (UNLIKELY(new_tok_)) {
        cur_type_ = TokType::DIGIT;
        beg_tok_ = cur_it;
        new_tok_ = false;
    }
}

void Parser::UpdateChar(std::string_view::iterator cur_it) {
    if (LIKELY(!new_tok_)) {
        if (cur_type_ == TokType::DIGIT)
            cur_type_ = TokType::STRING;
    } else {
        cur_type_ = TokType::STRING;
        beg_tok_ = cur_it;
        new_tok_ = false;
    }
}

void Parser::Parse(std::string_view p_str) {
    if (beg_callback_)
        beg_callback_();

    cur_it_ = p_str.begin();
    new_tok_ = true;

    while (std::isspace(*cur_it_))
        ++cur_it_;

    beg_tok_ = cur_it_;
    for (; cur_it_ != p_str.end(); cur_it_++) {
        if (std::isspace(*cur_it_)) {
            if (!new_tok_) {
                new_tok_ = true;
                CallCallback();
            }
        } else if (new_tok_) {
            beg_tok_ = cur_it_;
            new_tok_ = false;
        }
    }
    if (!new_tok_)
        CallCallback();
    if (end_callback_)
        end_callback_();
}
void Parser::CallCallback() {
    int result;
    std::string_view view(beg_tok_, std::distance(beg_tok_, cur_it_));
    if (auto [p, ec] =
            std::from_chars(std::begin(view), std::end(view), result);
        ec == std::errc()) {
        if (digit_callback_)
            digit_callback_(result);
    } else {
        if (string_callback_)
            string_callback_(view);
    }
}
