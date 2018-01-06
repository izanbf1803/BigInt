#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    cout << BigInt("1000") + BigInt("0111") - BigInt("0101") + BigInt("-1") << endl;
}