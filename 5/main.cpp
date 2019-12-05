#include <iostream>
#include <sstream>
#include "../test_runner.h"
#include "serrializer.h"

struct Data1 {
    uint64_t a{};
    bool b{};
    uint64_t c{};

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer) {
        return deserializer(a, b, c);
    }
    bool operator==(const Data1& rhs) const {
        return std::tie(a, b, c) == std::tie(rhs.a, rhs.b, rhs.c);
    }
    bool operator!=(const Data1& rhs) const {
        return !(rhs == *this);
    }
    friend std::ostream& operator<<(std::ostream& os, const Data1& data_1) {
        os << "a: " << data_1.a << " b: " << data_1.b << " c: " << data_1.c;
        return os;
    }
};

void Test1() {
    std::stringstream ss;

    Serializer ser(ss);
    Deserializer des(ss);

    Data1 d{100, true, 500}, d_d{};

    auto err = ser.Save(d);
    ASSERT(err == Error::NO_ERROR);
    err = des.Load(d_d);
    ASSERT(err == Error::NO_ERROR);

    ASSERT_EQUAL(d_d, d);
}

void Test2() {
    std::stringstream ss("1 fals 1.12");

    Deserializer des(ss);

    Data1 d{};

    auto err = des.Load(d);
    ASSERT(err == Error::CORRUPTED_ARCHIVE);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test2);

    return 0;
}
