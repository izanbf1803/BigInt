#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
using namespace std;

namespace bigint
{

#define D(x) cerr << #x << " = " << (x) << ", "

class BigInt
{
public:
    friend ostream& operator<<(ostream& out, const BigInt& n);
    inline BigInt() { v_ = {0}; };
    BigInt(int64_t n);
    BigInt(const vector<bool>& v, int8_t sign);
    BigInt(const string& s, int base);
    bool operator<(const BigInt& n) const;
    bool operator>(const BigInt& n) const;
    bool operator<=(const BigInt& n) const;
    bool operator>=(const BigInt& n) const;
    bool operator==(const BigInt& n) const;
    bool operator!=(const BigInt& n) const;
    BigInt operator+(const BigInt& n) const;
    BigInt operator-(const BigInt& n) const;
    BigInt operator%(const BigInt& n) const;
    BigInt operator/(const BigInt& n) const;
    BigInt operator<<(const BigInt& n) const;
    BigInt operator>>(const BigInt& n) const;
    // BigInt operator*(const BigInt& n) const;
    inline BigInt operator+=(const BigInt& n) { *this = *this + n; return *this; };
    inline BigInt operator-=(const BigInt& n) { *this = *this - n; return *this; };
    inline BigInt operator%=(const BigInt& n) { *this = *this % n; return *this; };
    inline BigInt operator/=(const BigInt& n) { *this = *this / n; return *this; };
    inline BigInt operator<<=(const BigInt& n) { *this = *this << n; return *this; };
    inline BigInt operator>>=(const BigInt& n) { *this = *this >> n; return *this; };
    // inline BigInt operator*=(const BigInt& n) { *this = *this * n; return *this; };
    bool bit(size_t index) const;
    pair<BigInt,BigInt> div(const BigInt& n) const;
    string str() const;
    int64_t int64() const;

    vector<bool> v_; // Vector of bits storing digits of the number
    int8_t sign_ = +1;
private:
    void print(ostream& out) const;
    void remove_leading_zeros();
};

const vector<vector<bool>> BI_v = { {0}, {1}, {0, 1}, {1, 1}, {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1} };
const vector<BigInt> BI = { 
    BigInt(BI_v[0], +1), BigInt(BI_v[1], +1), BigInt(BI_v[2], +1), BigInt(BI_v[3], +1), 
    BigInt(BI_v[4], +1), BigInt(BI_v[5], +1), BigInt(BI_v[6], +1), BigInt(BI_v[7], +1), 
    BigInt(BI_v[8], +1), BigInt(BI_v[9], +1), BigInt(BI_v[10], +1),  
};

inline int ctoi(char c) { return c - '0'; }
inline char itoc(int n) { return n + '0'; }
inline size_t min(size_t a, size_t b) { return (a < b ? a : b); }
inline size_t max(size_t a, size_t b) { return (a > b ? a : b); }
inline BigInt abs(BigInt n) { n.sign_ = +1; return n; };
inline int64_t abs(int64_t n) { return (n < 0 ? -n : n);  };
inline int8_t sign(int64_t n) { return (n < 0 ? -1 : +1);  };
BigInt binary_addition(const BigInt& n, const BigInt& k);
BigInt binary_subtraction(const BigInt& n, const BigInt& k);


ostream& operator<<(ostream& out, const BigInt& n)
{
    /*out << "(";
    n.print(out);
    out << ")";*/
    out << n.str();
    return out;
}

BigInt::BigInt(int64_t n)
{
    n = abs(n);
    size_t size = 0;
    int64_t k = n;
    while (k > 0) {
        ++size;
        k /= 2;
    }
    v_ = vector<bool>(size, 0);
    for (int i = 0; n > 0; ++i) {
        v_[i] = n % 2;
        n /= 2;
    }   
}

BigInt::BigInt(const string& s, int base)
{
    size_t size = s.size();
    if (size > 0) {
        if (base == 2) {
            size_t i = 0;
            v_ = vector<bool>(size, 0);
            if (s[0] == '-') {
                ++i;
                sign_ = -1;
            }
            for (; i < size; ++i) {
                v_[size-1-i] = ctoi(s[i]);
            }
        }
        else {
            // TODO
        }
        remove_leading_zeros();
    }
    else {
        v_ = {0};
    }
}

BigInt::BigInt(const vector<bool>& v, int8_t sign = +1)
{ 
    v_ = v; 
    sign_ = sign;
    remove_leading_zeros();
}

void BigInt::print(ostream& out) const
{
    if (sign_ == -1) out << '-';
    for (size_t i = 0; i < v_.size(); ++i) {
        out << (int)v_[v_.size() - i - 1];
    }
}

bool BigInt::operator<(const BigInt& n) const
{
    if (sign_ == -1 and n.sign_ == +1) return true;
    if (sign_ == +1 and n.sign_ == -1) return false;
    if (v_.size() < n.v_.size()) return (sign_ == +1 ? true : false);
    if (v_.size() > n.v_.size()) return (sign_ == +1 ? false : true);
    for (size_t i = 0; i < v_.size(); ++i) {
        size_t index = v_.size() - 1 - i;
        if (v_[index] > n.v_[index]) return (sign_ == +1 ? false : true);
        if (v_[index] < n.v_[index]) return (sign_ == +1 ? true : false);
    }
    return false; 
}

bool BigInt::operator>(const BigInt& n) const
{
    if (sign_ == -1 and n.sign_ == +1) return false;
    if (sign_ == +1 and n.sign_ == -1) return true;
    if (v_.size() < n.v_.size()) return (sign_ == +1 ? false : true);
    if (v_.size() > n.v_.size()) return (sign_ == +1 ? true : false);
    for (size_t i = 0; i < v_.size(); ++i) {
        size_t index = v_.size() - 1 - i;
        if (v_[index] > n.v_[index]) return (sign_ == +1 ? true : false);
        if (v_[index] < n.v_[index]) return (sign_ == +1 ? false : true);
    }
    return false; 
}

bool BigInt::operator<=(const BigInt& n) const
{
    return *this < n or *this == n; 
}

bool BigInt::operator>=(const BigInt& n) const
{
    return *this > n or *this == n;
}

bool BigInt::operator==(const BigInt& n) const
{
    if (sign_ != n.sign_) return false;
    if (v_.size() != n.v_.size()) return false;
    for (size_t i = 0; i < v_.size(); ++i) {
        if (v_[i] != n.v_[i]) return false;
    }
    return true; 
}

bool BigInt::operator!=(const BigInt& n) const
{
    return not (*this == n);
}

BigInt BigInt::operator+(const BigInt& n) const
{
    if (sign_ == n.sign_) {
        BigInt r = binary_addition(*this, n);
        r.sign_ = sign_;
        return r;
    }
    else {
        if (sign_ == +1 and n.sign_ == -1) {
            BigInt r = binary_subtraction(*this, n);
            r.sign_ = +1;
            return r;
        }
        else {
            BigInt r = binary_subtraction(n, *this);
            r.sign_ = -1;
            return r;
        }
    }
    
}

BigInt BigInt::operator-(const BigInt& n) const
{
    if (sign_ != n.sign_) {
        BigInt r = binary_addition(*this, n);
        r.sign_ = sign_;
        return r;
    }
    else {
        if (*this >= n) { // sign_(*this) = +1, sign_(n) = -1
            // If n > this: return this - n
            BigInt r = binary_subtraction(*this, n);
            r.sign_ = +1;
            return r;
        }
        else { // sign_(*this) = -1, sign_(n) = +1
            // If n > this: return -(n - this)
            BigInt r = binary_subtraction(n, *this);
            r.sign_ = -1;
            return r;
        }
    }
}

BigInt BigInt::operator%(const BigInt& n) const
{
    assert( n != BI[0] );
    return div(n).second;
}

BigInt BigInt::operator/(const BigInt& n) const
{
    assert( n != BI[0] );
    return div(n).first;
}

BigInt BigInt::operator<<(const BigInt& n) const 
{
    BigInt r = *this;
    int64_t k = abs(n.int64());
    while (k > 0) {
        --k;
        r.v_.insert(r.v_.begin(), 0);
    }
    return r;
}

BigInt BigInt::operator>>(const BigInt& n) const
{
    BigInt r = *this;
    int64_t k = abs(n.int64());
    while (k > 0) {
        --k;
        r.v_.erase(r.v_.begin());
    }
    return r;
}

pair<BigInt,BigInt> BigInt::div(const BigInt& n) const
{
    BigInt d = n;
    BigInt r = *this;
    BigInt q = BI[0];
    r.sign_ = d.sign_ = +1;
    int8_t result_sign_ = this->sign_ * n.sign_;
    while (r >= d) {
        r -= d;
        q += BI[1];
    }
    if (result_sign_ == -1) {
        q.sign_ = -1;
    }
    return make_pair(q, r);
}

int64_t BigInt::int64() const
{
    assert( v_.size() <= 62 );
    size_t size = v_.size();
    int64_t r = v_[size-1];
    for (size_t i = 1; i < v_.size(); ++i) {
        r *= 2;
        r += v_[size-1-i];
    }
    r *= sign_;
    return r;
}

string BigInt::str() const
{
    string res;
    res.reserve(v_.size()/3);
    BigInt n = *this;
    n.sign_ = +1;
    while (n > BI[0]) {
        pair<BigInt, BigInt> q_r = n.div(BI[10]);
        for (int i = 0; i < 10; ++i) {
            if (q_r.second == BI[i]) {
                res += '0' + i;
                break;
            }
        }
        n = q_r.first;
    }
    if (this->sign_ == -1) res += '-';
    for (int i = 0; i < res.size()/2; ++i) {
        swap(res[i], res[res.size()-1-i]);
    }
    return res;
}

bool BigInt::bit(size_t index) const
{
    if (index >= v_.size()) return 0;
    return v_[index];
}

void BigInt::remove_leading_zeros()
{
    size_t count = 0;
    size_t size = v_.size();
    for (size_t i = 0; i < size - 1 and v_[size-1-i] == 0; ++i) { // size-1 : Don't delete first digit never
        ++count;
    }
    v_.resize(size - count);
}

// Details: https://en.wikipedia.org/wiki/Adder_(electronics)#Full_adder
BigInt binary_addition(const BigInt& n, const BigInt& k)
{
    size_t max_size = max(n.v_.size(), k.v_.size());
    vector<bool> addition(max_size + 1);
    bool c = 0; // carry
    for (size_t i = 0; i <= max_size; ++i) {
        bool x = n.bit(i), y = k.bit(i);
        addition[i] = (x ^ y) ^ c;
        c = (x & y) | (x & c) | (y & c);
    }
    return BigInt(addition);
}

// Details: https://en.wikipedia.org/wiki/Subtractor#Full_Subtractor
BigInt binary_subtraction(const BigInt& n, const BigInt& k)
{
    size_t max_size = max(n.v_.size(), k.v_.size());
    vector<bool> subtraction(max_size + 1);
    bool b = 0; // borrow
    for (size_t i = 0; i <= max_size; ++i) {
        bool x = n.bit(i), y = k.bit(i);
        subtraction[i] = (x ^ y) ^ b;
        b = (!x & y) | ( !(x ^ y) & b );
    }
    return BigInt(subtraction);
}

};
