#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>
#include <chrono>
using namespace std;

namespace bigint
{

#define D(x) cerr << #x << " = " << (x) << ", "
using DTYPE = int;

const int BASE = 1000000000;
const int LOGB = 9;

class BigInt
{
public:
    friend ostream& operator<<(ostream& out, const BigInt& n);
    inline BigInt() { v_ = {0}; };
    // BigInt(int64_t n);
    BigInt(string s);
    bool operator<(const BigInt& n) const;
    bool operator>(const BigInt& n) const;
    bool operator<=(const BigInt& n) const;
    bool operator>=(const BigInt& n) const;
    bool operator==(const BigInt& n) const;
    bool operator!=(const BigInt& n) const;
    BigInt operator+(const BigInt& n) const;
    BigInt operator-(const BigInt& n) const;
    // BigInt operator<<(const BigInt& n) const;
    // BigInt operator>>(const BigInt& n) const;
    // BigInt operator<<(int64_t n) const;
    // BigInt operator>>(int64_t n) const;
    BigInt operator*(const BigInt& n) const;
    BigInt operator*(DTYPE k) const;
    BigInt operator/(BigInt n) const;
    BigInt operator/(DTYPE n) const;
    inline BigInt operator%(const BigInt& n) const { return *this - *this/n*n; };
    inline BigInt operator%(DTYPE n) const { return *this - *this/n*n; };
    inline BigInt operator+=(const BigInt& n) { *this = *this + n; return *this; };
    inline BigInt operator-=(const BigInt& n) { *this = *this - n; return *this; };
    // inline BigInt operator<<=(const BigInt& n) { *this = *this << n; return *this; };
    // inline BigInt operator>>=(const BigInt& n) { *this = *this >> n; return *this; };
    // inline BigInt operator<<=(int64_t n) { *this = *this << n; return *this; };
    // inline BigInt operator>>=(int64_t n) { *this = *this >> n; return *this; };
    inline BigInt operator*=(const BigInt& n) { *this = *this * n; return *this; };
    inline BigInt operator*=(DTYPE n) { *this = *this * n; return *this; };
    inline BigInt operator/=(const BigInt& n) { *this = *this / n; return *this; };
    inline BigInt operator/=(DTYPE n) { *this = *this / n; return *this; };
    inline BigInt operator%=(const BigInt& n) { *this = *this % n; return *this; };
    inline BigInt operator%=(DTYPE n) { *this = *this % n; return *this; };
    inline DTYPE mod2() const { return v_.back() % 2; };
    void remove_leading_zeros();
    // bool bit(int index) const;
    // pair<BigInt,BigInt> div(const BigInt& n) const;
    // inline BigInt pow(const BigInt& n) const { return this->pow(n.int64()); };
    BigInt pow(const BigInt& n) const;
    string str() const;
    // int64_t int64() const;

    vector<DTYPE> v_; // Vector of bits storing digits of the number
    int8_t sign_ = +1;
private:
    // void print(ostream& out) const;
};

const vector<BigInt> BI = { 
    BigInt("0"), BigInt("1"), BigInt("2"), BigInt("3"), 
    BigInt("4"), BigInt("5"), BigInt("6"), BigInt("7"), 
    BigInt("8"), BigInt("9"), BigInt("10"),  
};

inline int ctoi(char c) { return c - '0'; }
inline char itoc(int n) { return n + '0'; }
inline DTYPE min(DTYPE a, DTYPE b) { return (a < b ? a : b); }
inline DTYPE max(DTYPE a, DTYPE b) { return (a > b ? a : b); }
inline BigInt abs(BigInt n) { n.sign_ = +1; return n; };
inline DTYPE abs(DTYPE n) { return (n < 0 ? -n : n);  };
inline int8_t sign(int64_t n) { return (n < 0 ? -1 : +1); };
BigInt raw_addition(BigInt a, const BigInt& b);
BigInt raw_subtraction(BigInt a, const BigInt& b);


ostream& operator<<(ostream& out, const BigInt& n)
{
    out << n.str();
    return out;
}

// BigInt::BigInt(int64_t n)
// {
//     if (n < 0) sign_ = -1;
//     n = abs(n);
//     int size = 0;
//     int64_t k = n;
//     while (k > 0) {
//         ++size;
//         k /= 2;
//     }
//     v_ = vector<DTYPE>(size, 0);
//     for (int i = 0; n > 0; ++i) {
//         v_[i] = n % 2;
//         n /= 2;
//     }   
// }

BigInt::BigInt(string s)
{
    if (s[0] == '+') s.erase(s.begin());
    if (s[0] == '-') {
        s.erase(s.begin());
        sign_ = -1;
    }
    for (int i = (int)s.length(); i > 0; i -= 9) {
        if (i < 9) {
            v_.push_back(atoi(s.substr(0, i).c_str()));
        }
        else {
            v_.push_back(atoi(s.substr(i-9, 9).c_str()));
        }
    }
    remove_leading_zeros();
}

// BigInt::BigInt(const vector<DTYPE>& v, int8_t sign = +1)
// { 
//     v_ = v; 
//     sign_ = sign;
//     remove_leading_zeros();
// }

// void BigInt::print(ostream& out) const
// {
//     if (sign_ == -1) out << '-';
//     for (int i = 0; i < v_.size(); ++i) {
//         out << (int)v_[v_.size() - i - 1];
//     }
// }

bool BigInt::operator<(const BigInt& n) const
{
    if (sign_ == -1 and n.sign_ == +1) return true;
    if (sign_ == +1 and n.sign_ == -1) return false;
    if (v_.size() < n.v_.size()) return (sign_ == +1 ? true : false);
    if (v_.size() > n.v_.size()) return (sign_ == +1 ? false : true);
    for (int i = 0; i < v_.size(); ++i) {
        int index = v_.size() - 1 - i;
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
    for (int i = 0; i < v_.size(); ++i) {
        int index = v_.size() - 1 - i;
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
    for (int i = 0; i < v_.size(); ++i) {
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
        return raw_addition(*this, n);
    }
    else {
        if (sign_ == +1 and n.sign_ == -1) {
            BigInt x = n;
            x.sign_ = +1;
            return *this - x;
        }
        else {
            BigInt x = *this;
            x.sign_ = +1;
            return n - x;
        }
    }
    
}

BigInt BigInt::operator-(const BigInt& n) const
{
    if (sign_ != n.sign_) {
        return raw_addition(*this, n);
    }
    else {
        BigInt x = *this;
        BigInt y = n;
        x.sign_ = y.sign_ = +1;
        if (x >= y) { // sign_(*this) = +1, sign_(n) = -1
            // If this >= n: return this - n
            BigInt r = raw_subtraction(x, y);
            r.sign_ = sign_;
            return r;
        }
        else { // sign_(*this) = -1, sign_(n) = +1
            // If n > this: return -(n - this)
            BigInt r = raw_subtraction(y, x);
            r.sign_ = -1*sign_;
            return r;
        }
    }
}

BigInt BigInt::operator*(const BigInt& k) const
{
    BigInt n = *this;
    BigInt r;
    vector<DTYPE>& a = n.v_;
    const vector<DTYPE>& b = k.v_;
    vector<DTYPE>& c = r.v_;

    n.sign_ *= k.sign_;

    c.resize(a.size() + b.size());
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = 0, carry = 0; j < (int)b.size() or carry; ++j) {
            long long cur = c[i+j] + a[i] * 1ll * (j < (int)b.size() ? b[j] : 0) + carry;
            c[i+j] = int(cur % BASE);
            carry = int(cur / BASE);
        }
    }
    r.remove_leading_zeros();

    return r;
}

BigInt BigInt::operator*(DTYPE k) const
{
    BigInt n = *this;
    vector<DTYPE>& a = n.v_;

    n.sign_ *= sign(k);
    k = abs(k);

    int carry = 0;
    for (size_t i = 0; i < (int)a.size() or carry; ++i) {
        if (i == a.size()) a.push_back (0);
        long long cur = carry + a[i] * 1ll * k;
        a[i] = int(cur % BASE);
        carry = int(cur / BASE);
    }
    n.remove_leading_zeros();

    return n;
}

BigInt BigInt::operator/(BigInt k) const 
{   // TODO: implement a real division algorithm :)
    assert(k != BI[0]);

    BigInt n = *this;
    n.sign_ = +1;
    int8_t q_final_sign = sign_ * k.sign_;
    k.sign_ = +1;

    BigInt l = BI[0], r = n, q = BI[0];
    while (r >= l) {
        BigInt m = (l+r)/2;
        BigInt mk = m * k;
        if (mk > n) {
            r = m - BI[1];
        }
        else {
            q = l;
            l = m + BI[1];
        }
    }

    q.sign_ = q_final_sign;
    return q;
}

BigInt BigInt::operator/(DTYPE k) const
{
    assert(k != 0);

    BigInt n = *this;
    vector<DTYPE>& a = n.v_;

    n.sign_ *= sign(k);
    k = abs(k);

    int carry = 0;
    for (int i = (int)a.size()-1; i >= 0; --i) {
        long long cur = a[i] + carry * 1ll * BASE;
        a[i] = int(cur / k);
        carry = int(cur % k);
    }
    n.remove_leading_zeros();

    return n;
}

// BigInt BigInt::operator%(const BigInt& n) const
// {
//     assert( n != BI[0] );
//     return div(n).second;
// }

// BigInt BigInt::operator/(const BigInt& n) const
// {
//     assert( n != BI[0] );
//     return div(n).first;
// }

// BigInt BigInt::operator<<(int64_t n) const 
// {
//     BigInt r = *this;
//     r.v_.insert(r.v_.begin(), abs(n), 0);
//     r.remove_leading_zeros();
//     return r;
// }

// BigInt BigInt::operator>>(int64_t n) const
// {
//     if (abs(n) >= this->v_.size()) return BI[0];

//     BigInt r = *this;
//     r.v_.erase(r.v_.begin(), r.v_.begin() + abs(n));
//     return r;
// }

// BigInt BigInt::operator<<(const BigInt& n) const 
// {
//     return *this << n.int64();
// }

// BigInt BigInt::operator>>(const BigInt& n) const
// {
//     return *this >> n.int64(); 
// }

// BigInt BigInt::mul(const BigInt& n) const
// {
//     vector<DTYPE> r(v_.size() + n.v_.size(), 0);
//     for (int i = 0; i < v_.size(); ++i) {
//         int8_t carry = 0;
//         for (int j = 0; j < n.v_.size(); ++j) {
//             int8_t p = carry + (v_[i] & n.v_[j]) + r[i+j];
//             carry = p >> 1;
//             r[i+j] = p & 1;
//         }
//         r[i+n.v_.size()] = r[i+n.v_.size()] | carry;
//     }
//     BigInt x(r, sign_ * n.sign_);
//     x.remove_leading_zeros();
//     return x;
// }

// BigInt BigInt::pow(int64_t n) const
// {
//     assert( n >= 0 );
//     if (n == 0) return BI[1];
//     if (n == 1) return *this;
//     BigInt x2 = this->mul(*this);
//     if (n % 2 == 0) return x2.pow(n/2);
//     return this->mul(x2.pow((n-1)/2));
// }

// // positive division:
// pair<BigInt,BigInt> BigInt::div(const BigInt& n) const
// {
//     // chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

//     BigInt q = BI[0];
//     BigInt r = *this;
//     BigInt d = n;
//     r.sign_ = d.sign_ = +1;
//     int64_t k = 0;
//     while (d <= r) {
//         ++k;
//         d = d << 1;
//     }
//     while (k-- > 0) {
//         d = d >> 1;
//         if (d <= r) {
//             r = r - d;
//             q = (q << 1) + 1;
//         }
//         else {
//             q = q << 1;
//         }
//     }

//     // chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
//     // auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
//     // D(duration) << endl;

//     return make_pair(q, r);
// }

// int64_t BigInt::int64() const
// {
//     assert( v_.size() <= 62 );
//     int size = v_.size();
//     int64_t r = v_[size-1];
//     for (int i = 1; i < v_.size(); ++i) {
//         r <<= 1;
//         r += v_[size-1-i];
//     }
//     r *= sign_;
//     return r;
// }

// string BigInt::str() const
// {
//     string res;
//     res.reserve(v_.size()/3);
//     BigInt n = *this;
//     n.sign_ = +1;
//     if (n == BI[0]) return "0";

//     chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();

//     while (n > BI[0]) {
//         pair<BigInt, BigInt> q_r = n.div(BI[10]);
//         res += '0' + q_r.second.int64();
//         n = q_r.first;
//     }

//     if (this->sign_ == -1) res += '-';
//     for (int i = 0; i < res.size()/2; ++i) {
//         swap(res[i], res[res.size()-1-i]);
//     }

//     chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
//     auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
//     D(duration) << endl;

//     return res;
// }

void BigInt::remove_leading_zeros()
{
    while (v_.size() > 1 and v_.back() == 0) {
        v_.pop_back();
    }
}

string BigInt::str() const 
{
    string repr = "";
    if (sign_ == -1) repr += '-';
    repr += (v_.empty() ? "0" : to_string(v_.back()));
    for (int i = (int)v_.size()-2; i >= 0; --i) {
        string str = to_string(v_[i]);
        while (str.size() < LOGB) {
            str.insert(str.begin(), '0');
        }
        repr += str;
    }

    return repr;
}

BigInt BigInt::pow(const BigInt& n) const
{
    if (n == BI[0]) return BI[1];
    if (n == BI[1]) return *this;
    if (n.mod2() == 0) return ((*this) * (*this)).pow(n/2);
    return (*this) * ((*this) * (*this)).pow((n-BI[1])/2);
}

BigInt raw_addition(BigInt n, const BigInt& k)
{
    vector<DTYPE>& a = n.v_;
    const vector<DTYPE>& b = k.v_;

    int carry = 0;
    for (int i = 0; i < (int)max(a.size(), b.size()) or carry; ++i) {
        if (i == a.size()) a.push_back (0);
        a[i] += carry + (i < b.size() ? b[i] : 0);
        carry = a[i] >= BASE;
        if (carry)  a[i] -= BASE;
    }

    return n;
}

BigInt raw_subtraction(BigInt n, const BigInt& k)
{
    vector<DTYPE>& a = n.v_;
    const vector<DTYPE>& b = k.v_;

    int carry = 0;
    for (int i = 0; i < (int)b.size() or carry; ++i) {
        a[i] -= carry + (i < b.size() ? b[i] : 0);
        carry = a[i] < 0;
        if (carry) a[i] += BASE;
    }
    n.remove_leading_zeros();

    return n;
}

};
