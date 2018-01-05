#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

namespace bigint
{

inline int ctoi(char c) { return c - '0'; }
inline char itoc(int n) { return n + '0'; }
inline size_t min(size_t a, size_t b) { return (a < b ? a : b); }
inline size_t max(size_t a, size_t b) { return (a > b ? a : b); }

class BigInt
{
public:
    friend ostream& operator<<(ostream& out, const BigInt& n);
    inline BigInt() { v = {0}; };
    inline BigInt(const vector<uint8_t>& v_) { v = v_; };
    BigInt(const string& str, bool reversed);
    void print(ostream& out) const;
    BigInt operator+(const BigInt& n) const;
    BigInt operator*(const BigInt& n) const;
    inline BigInt operator+=(const BigInt& n) { *this = *this + n; return *this; };
    inline BigInt operator*=(const BigInt& n) { *this = *this * n; return *this; };
    BigInt digit_mul(const uint8_t n, const size_t right_zeros) const; // Mutliplicate BigInt by integer (of one digit)

private:
    vector<uint8_t> v; // Vector of bytes storing digits of the number
};

ostream& operator<<(ostream& out, const BigInt& n)
{
    n.print(out);
    return out;
}

BigInt::BigInt(const string& s, bool reversed = false)
{
    size_t size = s.size();
    if (size > 0) {
        v = vector<uint8_t>(size);
        if (reversed) {
            for (int i = 0; i < size; ++i) {
                v[i] = ctoi(s[i]);
            }
        }
        else {
            for (int i = 0; i < size; ++i) {
                v[i] = ctoi(s[size - i - 1]);
            }
        }
    }
    else {
        v = {0};
    }
}

void BigInt::print(ostream& out) const
{
    for (size_t i = 0; i < v.size(); ++i) {
        out << (int)v[v.size() - i - 1];
    }
}

BigInt BigInt::operator+(const BigInt& n) const
{
    size_t max_size = max(v.size(), n.v.size());

    vector<uint8_t> result;
    result.reserve(max_size + 1);

    uint8_t carry = 0;

    for (size_t i = 0; i < max_size; ++i) {
        uint8_t sum = carry;
        carry = 0;

        if (i < v.size()) sum += v[i];
        if (i < n.v.size()) sum += n.v[i];

        if (sum >= 10) {
            sum -= 10;
            carry = 1;
        }

        result.push_back(sum);
    }

    if (carry > 0) {
        result.push_back(carry);
    }

    return BigInt(result);
}

BigInt BigInt::operator*(const BigInt& n) const
{
    vector<BigInt> products(n.v.size());

    size_t right_zeros = 0;

    for (size_t i = 0; i < n.v.size(); ++i) {
        products[i] = digit_mul(n.v[i], right_zeros);
        ++right_zeros;
    }

    BigInt prod = products[0];

    for (size_t i = 1; i < products.size(); ++i) {
        prod += products[i];
    }

    return prod;
}

BigInt BigInt::digit_mul(const uint8_t n, const size_t right_zeros) const
{
    vector<uint8_t> result;
    result.reserve(v.size() + 1 + right_zeros);

    for (size_t i = 0; i < right_zeros; ++i) {
        result.push_back(0);
    }

    uint8_t carry = 0;

    for (size_t i = 0; i < v.size(); ++i) {
        uint8_t prod = v[i] * n + carry;

        carry = prod / 10;
        prod = prod % 10;

        result.push_back(prod);
    }

    if (carry > 0) {
        result.push_back(carry);
    }

    return BigInt(result);
}

};