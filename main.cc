#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    cout << BigInt("1000") + BigInt("0111") - BigInt("0101") + BigInt("-1") << endl;
    cout << (BigInt("11") > BigInt("11")) << endl;
    cout << (BigInt("11") >= BigInt("11")) << endl;
    cout << (BigInt("11") == BigInt("11")) << endl;
    cout << (BigInt("10") < BigInt("10")) << endl;
    cout << (BigInt("101000111") - BigInt("1010")) << endl;
    cout << (BigInt("1100100") - BigInt("1010")) << endl;
    cout << BigInt("11000111") % BigInt("1010") << endl;
}