#include "Parser.h"
#include <algorithm>
#include "likely.h"

ParserBuilder Parser::Make() {
    return ParserBuilder();
}
ParserBuilder& ParserBuilder::AddBegCalback(
    const std::function<void(void)>& beg_callback) {
    parser_.beg_callback_ = beg_callback;
    return *this;
}
ParserBuilder& ParserBuilder::AddDigCalback(
    const std::function<void(std::string_view)>& digit_calback) {
    parser_.digit_calback_ = digit_calback;
    return *this;
}
ParserBuilder& ParserBuilder::AddStrCalback(
    const std::function<void(std::string_view)>& string_calback) {
    parser_.string_calback_ = string_calback;
    return *this;
}
ParserBuilder& ParserBuilder::AddEndCalback(
    const std::function<void(void)>& end_calback) {
    parser_.end_calback_ = end_calback;
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
        }
        if (std::isdigit(*cur_it_)) {
            UpdateDigit(cur_it_);
        } else if (std::isalpha(*cur_it_)) {
            UpdateChar(cur_it_);
        }
    }
    if (!new_tok_)
        CallCallback();
    if (end_calback_)
        end_calback_();
}
void Parser::CallCallback() {
    std::string_view view(beg_tok_, std::distance(beg_tok_, cur_it_));
    if (cur_type_ == TokType::DIGIT && digit_calback_)
        digit_calback_(view);
    else if (cur_type_ == TokType::STRING && string_calback_)
        string_calback_(view);
}
