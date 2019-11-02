#include <iostream>
#include "parser_lib/Parser.h"
#include "test_runner.h"

void Test() {
    std::ostringstream oss;

    Parser p = Parser::Make()
                   .AddBegCalback([&oss]() {
                       //clang-format off
                       oss << "Start parsing" << std::endl;
                   })
                   .AddDigCalback([&oss](std::string_view v) {
                       //clang-format off

                       oss << "Digit callback: " << v << std::endl;
                   })
                   .AddStrCalback([&oss](std::string_view v) {
                       //clang-format off
                       oss << "Str callback: " << v << std::endl;
                   })
                   .AddEndCalback([&oss]() {
                       //clang-format off
                       oss << "End parsing" << std::endl;
                   })
                   .Build();

    std::string s("  abc xyz b1as 222  ");
    std::string ans(
        "Start parsing\n"
        "Str callback: abc\n"
        "Str callback: xyz\n"
        "Str callback: b1as\n"
        "Digit callback: 222\n"
        "End parsing\n");
    p.Parse(s);
    ASSERT_EQUAL(oss.str(), ans);

    oss.clear();
    oss.str("");

    s = std::string("  abc xyz b1as 222 rrrrr");
    ans =
        "Start parsing\n"
        "Str callback: abc\n"
        "Str callback: xyz\n"
        "Str callback: b1as\n"
        "Digit callback: 222\n"
        "Str callback: rrrrr\n"
        "End parsing\n";

    p.Parse(s);
    ASSERT_EQUAL(oss.str(), ans);
}

void EmptyTest() {
    std::ostringstream oss;
    Parser p = Parser::Make()
                   .AddDigCalback([&oss](std::string_view v) {
                       //clang-format off

                       oss << "Digit callback: " << v << std::endl;
                   })
                   .Build();
    std::string s = std::string("  abc xyz b1as 222 rrrrr");
    std::string ans("Digit callback: 222\n");
    p.Parse(s);
    ASSERT_EQUAL(oss.str(), ans);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    RUN_TEST(tr, EmptyTest);
    return 0;
}