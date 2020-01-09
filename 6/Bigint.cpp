//
// Created by user on 13.12.2019.
//

#include "Bigint.h"
#include <iterator>

/*template <typename Integer,
          std::enable_if_t<std::is_integral<Integer>::value, int>>*/
Bigint::Bigint(int val) {
    sign_ = val >= 0;
    val = sign_ ? val : -val;

    while (val > 0) {
        words_.PushBack(val % WordMax);
        val /= WordMax;
    }
}

/*template <typename Integer,
          std::enable_if_t<std::is_integral<Integer>::value, int>>*/
Bigint& Bigint::operator=(int val) {
    sign_ = val >= 0;
    val = sign_ ? val : -val;

    words_.Clear();
    while (val > 0) {
        words_.PushBack(val % WordMax);
        val /= WordMax;
    }

    return *this;
}
Bigint Bigint::operator+(const Bigint& other) const {
    Bigint tmp;
    if (this->sign_ == other.sign_) {
        auto res_size = std::max(words_.Size(), other.words_.Size());
        int carry = 0, tmp_val;
        for (int i = 0; i < res_size; i++) {
            tmp_val = i < words_.Size() ? words_[i] : 0;
            tmp_val += i < other.words_.Size() ? other.words_[i] : 0;
            tmp_val += carry;
            tmp.words_.PushBack(tmp_val % WordMax);
            carry = tmp_val > WordMax;
        }
        if (carry) {
            tmp.words_.PushBack(carry % WordMax);
        }

        tmp.sign_ = this->sign_;
    } else {
        tmp = *this - (-other);
    }
    return tmp;
}

Bigint Bigint::operator+(int other) const {
    Bigint tmp;
    if ((this->sign_ && other > 0) || (!this->sign_ && other < 0)) {
        other = other > 0 ? other : -other;
        tmp = SumImpl(other);
        tmp.sign_ = this->sign_;
    } else {
        tmp = *this - (-other);
    }
    return tmp;
}
Bigint Bigint::SumImpl(int other) const {
    Bigint tmp;
    int carry = 0;
    int tmp_val = 0;
    tmp_val = words_[0] + other;
    tmp.words_.PushBack(tmp_val % WordMax);
    carry = tmp_val > WordMax;

    for (int idx = 1; idx < words_.Size() || carry; ++idx) {
        if (idx < words_.Size()) {
            tmp_val = words_[idx] + carry;
        } else {
            tmp_val = carry;
        }

        tmp.words_.PushBack(tmp_val % WordMax);
        carry = tmp_val > WordMax;
    }

    return tmp;
}

Bigint Bigint::operator-() const {
    Bigint tmp = *this;
    tmp.sign_ = !sign_;
    return tmp;
}

Bigint Bigint::operator-(const Bigint& other) const {
    Bigint tmp;
    if (this->sign_ == other.sign_) {
        if (*this > other)
            tmp = SubImpl(other);
        else {
            tmp = other.SubImpl(*this);
            tmp.sign_ = !tmp.sign_;
        }
        tmp.Trim();
    } else {
        tmp = *this + (-other);
    }

    return tmp;
}

Bigint Bigint::operator-(int other) const {
    Bigint tmp(*this);

    if ((tmp.sign_ && other >= 0) || (!tmp.sign_ && other < 0)) {
        if (other < 0)
            other = -other;

        if (tmp.words_.Empty()) {
            tmp.sign_ = !sign_;
            tmp.words_.PushBack(other);
        } else if (words_.Size() == 1) {
            tmp.words_[0] -= other;
            if (tmp.words_[0] < 0) {
                tmp.words_[0] = -tmp.words_[0];
                tmp.sign_ = !tmp.sign_;
            }
        } else {
            int tmp_val = 0;
            int carry = 0;
            for (int i = 0; i < tmp.words_.Size() || carry; ++i) {
                tmp.words_[i] -= carry;
                if (i == 0)
                    tmp.words_[i] -= other;
                carry = tmp.words_[i] < 0;
                if (carry)
                    tmp.words_[i] += WordMax;
            }
        }
        tmp.Trim();
    } else {
        tmp = *this + (-other);
    }
    return tmp;
}

Bigint Bigint::Abs() const {
    Bigint a = *this;
    a.sign_ = true;
    return a;
}
Bigint Bigint::SubImpl(const Bigint& b) const {
    auto res = *this;
    int carry = 0;
    for (int i = 0; i < words_.Size(); ++i) {
        res.words_[i] -= carry;
        res.words_[i] -= b.words_[i];
        carry = res.words_[i] < 0;
        if (carry)
            res.words_[i] += WordMax;
    }
    res.Trim();
    return res;
}
bool Bigint::operator==(const Bigint& rhs) const {
    return words_ == rhs.words_ && sign_ == rhs.sign_;
}
bool Bigint::operator!=(const Bigint& rhs) const {
    return !(rhs == *this);
}
bool Bigint::operator<(const Bigint& rhs) const {
    if (sign_ < rhs.sign_)
        return true;
    if (sign_ > rhs.sign_)
        return false;
    if (words_.Size() < rhs.words_.Size() && sign_)
        return true;
    if (words_.Size() < rhs.words_.Size() && !sign_)
        return false;
    if (words_.Size() > rhs.words_.Size() && sign_)
        return false;
    if (words_.Size() > rhs.words_.Size() && !sign_)
        return false;
    for (int i = words_.Size() - 1; i >= 0; i--) {
        if (words_[i] != rhs.words_[i])
            return words_[i] < rhs.words_[i];
    }
    return false;
}
bool Bigint::operator>(const Bigint& rhs) const {
    auto flag = rhs < *this;
    return flag;
}
bool Bigint::operator<=(const Bigint& rhs) const {
    return !(rhs < *this);
}
bool Bigint::operator>=(const Bigint& rhs) const {
    return !(*this < rhs);
}

bool Bigint::operator==(int rhs) const {
    return words_.Size() == 1 && ((sign_ && (rhs >= 0) && words_[0] == rhs) ||
                                  (!sign_ && (rhs < 0) && words_[0] == -rhs));
}
bool Bigint::operator!=(int rhs) const {
    return !(*this == rhs);
}

bool Bigint::operator<(int rhs) const {
    if (sign_ && rhs < 0)
        return false;
    if (!sign_ && rhs > 0)
        return true;
    if (sign_) {
        if (words_.Size() > 1)
            return false;
        else if (words_.Empty())
            return true;
        else
            return words_[0] < rhs;
    } else {
        if (words_.Size() > 1)
            return true;
        else if (words_.Empty())
            return false;
        else
            return words_[0] >= rhs;
    }
}

bool Bigint::operator>(int rhs) const {
    return (*this != rhs) && !(*this < rhs);
}
bool Bigint::operator<=(int rhs) const {
    return !(*this > rhs);
}
bool Bigint::operator>=(int rhs) const {
    return !(*this < rhs);
}

Bigint::Bigint(const char* s) {
    size_t s_len = strlen(s);
    int beg = 0;

    if (s[0] == '-') {
        sign_ = false;
        beg++;
    } else {
        sign_ = true;
    }

    int tmp_word = 0;
    int num_s = 0;

    for (int i = s_len - 1; i >= beg; i -= NumDig) {
        for (int j = std::max(beg, i - NumDig + 1); j <= i; j++) {
            unsigned char tmp_c = __gmp_digit_value_tab[static_cast<int>(s[j])];
            tmp_word *= Base;
            tmp_word += tmp_c;
        }
        words_.PushBack(tmp_word);
        tmp_word = 0;
    }
    this->Trim();
}
void Bigint::operator*=(int v) {
    int carry = 0;
    int64_t tmp_val = 0;

    if (v < 0) {
        sign_ = !sign_;
        v = -v;
    }
    for (int i = 0; i < words_.Size() || carry; ++i) {
        if (i == words_.Size())
            words_.PushBack(0);
        tmp_val = words_[i] * (int64_t)v + carry;
        carry = tmp_val / WordMax;
        words_[i] = tmp_val % WordMax;
    }
}

void Bigint::operator+=(int v) {
    if ((sign_ && v >= 0) || (!sign_ && v < 0)) {
        if (v < 0)
            v = -v;
        int carry = 0;
        int64_t tmp_val = 0;

        if (words_.Empty()) {
            words_.PushBack(v);
            return;
        }

        for (int i = 0; i < words_.Size() || carry; ++i) {
            if (i == words_.Size())
                words_.PushBack(0);
            tmp_val = words_[i] + carry;
            if (i == 0)
                tmp_val += (int64_t)v;

            carry = tmp_val > WordMax;
            words_[i] = tmp_val % WordMax;
        }

    } else {
        *this -= -v;
    }
}
void Bigint::operator-=(int v) {
    if ((sign_ && v >= 0) || (!sign_ && v < 0)) {
        if (v < 0)
            v = -v;

        if (words_.Empty()) {
            sign_ = !sign_;
            words_.PushBack(v);
            return;
        }
        if (words_.Size() == 1) {
            words_[0] -= v;
            if (words_[0] < 0) {
                words_[0] = -words_[0];
                sign_ = !sign_;
            }
            return;
        } else {
            int tmp_val = 0;
            int carry = 0;
            for (int i = 0; i < words_.Size() || carry; ++i) {
                words_[i] -= carry;
                if (i == 0)
                    words_[i] -= v;
                carry = words_[i] < 0;
                if (carry)
                    words_[i] += WordMax;
            }
        }
    } else {
        *this += -v;
    }
}

std::ostream& operator<<(std::ostream& os, const Bigint& bigint) {
    if (!bigint.sign_)
        os << "-";
    for (int i = 0; i < bigint.words_.Size(); ++i) {
        os << bigint.words_[i];
    }

    return os;
}
Bigint operator+(int i_val, const Bigint& v) {
    Bigint tmp;
    tmp = v + i_val;
    return tmp;
}
Bigint operator-(int i_val, const Bigint& v) {
    Bigint tmp;
    tmp = -v + i_val;
    return tmp;
}
void Bigint::Trim() {
    while (!words_.Empty() && !words_.Back())
        words_.PopBack();
    if (words_.Empty())
        sign_ = true;
}
Bigint::Bigint(Bigint&& other) noexcept {
    sign_ = other.sign_;
    words_ = std::move(other.words_);
}

Bigint& Bigint::operator=(const Bigint& other) {
    if (this == std::addressof(other))
        return *this;
    sign_ = other.sign_;
    words_ = other.words_;

    return *this;
}
Bigint& Bigint::operator=(Bigint&& other) noexcept {
    if (this == std::addressof(other))
        return *this;
    sign_ = other.sign_;
    words_ = std::move(other.words_);

    return *this;
}
