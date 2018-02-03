#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

namespace bigint
{

#define D(x) cerr << #x << " = " << (x) << ", "

class BigInt
{
public:
    friend ostream& operator<<(ostream& out, const BigInt& n);
    inline BigInt() { v = {0}; };
    BigInt(const vector<bool>& v_, int8_t sign_);
    BigInt(const string& s, int base);
    bool operator<(const BigInt& n) const;
    bool operator>(const BigInt& n) const;
    bool operator<=(const BigInt& n) const;
    bool operator>=(const BigInt& n) const;
    bool operator==(const BigInt& n) const;
    BigInt operator+(const BigInt& n) const;
    BigInt operator-(const BigInt& n) const;
    BigInt operator%(const BigInt& n) const;
    BigInt operator/(const BigInt& n) const;
    // BigInt operator*(const BigInt& n) const;
    inline BigInt operator+=(const BigInt& n) { *this = *this + n; return *this; };
    inline BigInt operator-=(const BigInt& n) { *this = *this - n; return *this; };
    inline BigInt operator%=(const BigInt& n) { *this = *this % n; return *this; };
    inline BigInt operator/=(const BigInt& n) { *this = *this / n; return *this; };
    // inline BigInt operator*=(const BigInt& n) { *this = *this * n; return *this; };
    bool bit(size_t index) const;
    pair<BigInt,BigInt> div(const BigInt& n) const;
    string str() const;

    vector<bool> v; // Vector of bits storing digits of the number
    int8_t sign = +1;
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
inline BigInt abs(BigInt n) { n.sign = +1; return n; };
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

BigInt::BigInt(const string& s, int base)
{
    size_t size = s.size();
    if (size > 0) {
        if (base == 2) {
            size_t i = 0;
            v = vector<bool>(size, 0);
            if (s[0] == '-') {
                ++i;
                sign = -1;
            }
            for (; i < size; ++i) {
                v[size-1-i] = ctoi(s[i]);
            }
        }
        else {
            // TODO
        }
        remove_leading_zeros();
    }
    else {
        v = {0};
    }
}

BigInt::BigInt(const vector<bool>& v_, int8_t sign_ = +1)
{ 
    v = v_; 
    sign = sign_;
    remove_leading_zeros();
}

void BigInt::print(ostream& out) const
{
    if (sign == -1) out << '-';
    for (size_t i = 0; i < v.size(); ++i) {
        out << (int)v[v.size() - i - 1];
    }
}

bool BigInt::operator<(const BigInt& n) const
{
    if (sign == -1 and n.sign == +1) return true;
    if (sign == +1 and n.sign == -1) return false;
    if (v.size() < n.v.size()) return (sign == +1 ? true : false);
    if (v.size() > n.v.size()) return (sign == +1 ? false : true);
    for (size_t i = 0; i < v.size(); ++i) {
        size_t index = v.size() - 1 - i;
        if (v[index] > n.v[index]) return (sign == +1 ? false : true);
        if (v[index] < n.v[index]) return (sign == +1 ? true : false);
    }
    return false; 
}

bool BigInt::operator>(const BigInt& n) const
{
    if (sign == -1 and n.sign == +1) return false;
    if (sign == +1 and n.sign == -1) return true;
    if (v.size() < n.v.size()) return (sign == +1 ? false : true);
    if (v.size() > n.v.size()) return (sign == +1 ? true : false);
    for (size_t i = 0; i < v.size(); ++i) {
        size_t index = v.size() - 1 - i;
        if (v[index] > n.v[index]) return (sign == +1 ? true : false);
        if (v[index] < n.v[index]) return (sign == +1 ? false : true);
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
    if (sign != n.sign) return false;
    if (v.size() != n.v.size()) return false;
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != n.v[i]) return false;
    }
    return true; 
}

BigInt BigInt::operator+(const BigInt& n) const
{
    if (sign == n.sign) {
        BigInt r = binary_addition(*this, n);
        r.sign = sign;
        return r;
    }
    else {
        if (sign == +1 and n.sign == -1) {
            BigInt r = binary_subtraction(*this, n);
            r.sign = +1;
            return r;
        }
        else {
            BigInt r = binary_subtraction(n, *this);
            r.sign = -1;
            return r;
        }
    }
    
}

BigInt BigInt::operator-(const BigInt& n) const
{
    if (sign != n.sign) {
        BigInt r = binary_addition(*this, n);
        r.sign = sign;
        return r;
    }
    else {
        if (*this >= n) { // sign(*this) = +1, sign(n) = -1
            // If n > this: return this - n
            BigInt r = binary_subtraction(*this, n);
            r.sign = +1;
            return r;
        }
        else { // sign(*this) = -1, sign(n) = +1
            // If n > this: return -(n - this)
            BigInt r = binary_subtraction(n, *this);
            r.sign = -1;
            return r;
        }
    }
}

BigInt BigInt::operator%(const BigInt& n) const
{
    return div(n).second;
}

BigInt BigInt::operator/(const BigInt& n) const
{
    return div(n).first;
}

pair<BigInt,BigInt> BigInt::div(const BigInt& n) const
{
    BigInt d = n;
    BigInt r = *this;
    BigInt q = BI[0];
    r.sign = d.sign = +1;
    int8_t result_sign = this->sign * n.sign;
    while (r >= d) {
        r -= d;
        q += BI[1];
    }
    if (result_sign == -1) {
        q.sign = -1;
    }
    return make_pair(q, r);
}

string BigInt::str() const
{
    string res = "";
    BigInt n = *this;
    n.sign = +1;
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
    if (this->sign == -1) res += '-';
    for (int i = 0; i < res.size()/2; ++i) {
        swap(res[i], res[res.size()-1-i]);
    }
    return res;
}

bool BigInt::bit(size_t index) const
{
    if (index >= v.size()) return 0;
    return v[index];
}

void BigInt::remove_leading_zeros()
{
    size_t count = 0;
    size_t size = v.size();
    for (size_t i = 0; i < size - 1 and v[size-1-i] == 0; ++i) { // size-1 : Don't delete first digit never
        ++count;
    }
    v.resize(size - count);
}

// Details: https://en.wikipedia.org/wiki/Adder_(electronics)#Full_adder
BigInt binary_addition(const BigInt& n, const BigInt& k)
{
    size_t max_size = max(n.v.size(), k.v.size());
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
    size_t max_size = max(n.v.size(), k.v.size());
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
