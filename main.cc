#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    BigInt a("0101");
    BigInt b("1010");
    cout << a + b << endl;
}