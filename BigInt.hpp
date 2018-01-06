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
    BigInt(const vector<bool>& v_);
    BigInt(const string& str, bool reversed);
    BigInt operator+(const BigInt& n) const;
    BigInt operator-(const BigInt& n) const;
    // BigInt operator*(const BigInt& n) const;
    inline BigInt operator+=(const BigInt& n) { *this = *this + n; return *this; };
    inline BigInt operator-=(const BigInt& n) { *this = *this - n; return *this; };
    // inline BigInt operator*=(const BigInt& n) { *this = *this * n; return *this; };
    bool bit(size_t index) const;

private:
    void print(ostream& out) const;
    void remove_leading_zeros();
    // BigInt digit_mul(const uint8_t n, const size_t right_zeros) const; // Mutliplicate BigInt by integer (of one digit)

    vector<bool> v; // Vector of bits storing digits of the number
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
        v = vector<bool>(size);
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
        remove_leading_zeros();
    }
    else {
        v = {0};
    }
}

BigInt::BigInt(const vector<bool>& v_) 
{ 
    v = v_; 
    remove_leading_zeros(); 
}

void BigInt::print(ostream& out) const
{
    for (size_t i = 0; i < v.size(); ++i) {
        out << (int)v[v.size() - i - 1];
    }
}

// Details: https://en.wikipedia.org/wiki/Adder_(electronics)#Full_adder
BigInt BigInt::operator+(const BigInt& n) const
{
    size_t max_size = max(v.size(), n.v.size());
    vector<bool> addition(max_size + 1);
    bool c = 0; // carry
    for (size_t i = 0; i <= max_size; ++i) {
        bool x = bit(i), y = n.bit(i);
        addition[i] = (x ^ y) ^ c;
        c = (x & y) | (x & c) | (y & c);
    }
    return BigInt(addition);
}

// Details: https://en.wikipedia.org/wiki/Subtractor#Full_Subtractor
BigInt BigInt::operator-(const BigInt& n) const
{
    size_t max_size = max(v.size(), n.v.size());
    vector<bool> subtraction(max_size + 1);
    bool b = 0; // borrow
    for (size_t i = 0; i <= max_size; ++i) {
        bool x = bit(i), y = n.bit(i);
        subtraction[i] = (x ^ y) ^ b;
        b = (!x & y) | ( (x ^ y) & b );
    }
    return BigInt(subtraction);
}

/*
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
*/

bool BigInt::bit(size_t index) const
{
    if (index >= v.size())
        return 0;
    return v[index];
}

void BigInt::remove_leading_zeros()
{
    size_t count = 0;
    for (size_t i = 0; i < v.size() - 1; ++i) { // Don't delete first digit never
        if (v[v.size() - i - 1] == 0) {
            ++count;
        }
        else {
            break;
        }
    }
    v.resize(v.size() - count);
}

/*
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
*/

};