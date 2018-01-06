#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    BigInt a("10010101010100101");
    BigInt b("10100101");
    cout << a - b << endl;
}