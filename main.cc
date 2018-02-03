#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    cout << "A " << (BigInt("1000", 2) + BigInt("0111", 2) - BigInt("0101", 2) + BigInt("-1", 2)) << endl;
    cout << (BigInt("11", 2) > BigInt("11", 2)) << endl;
    cout << (BigInt("11", 2) >= BigInt("11", 2)) << endl;
    cout << (BigInt("11", 2) == BigInt("11", 2)) << endl;
    cout << (BigInt("10", 2) < BigInt("10", 2)) << endl;
    cout << "B " << (BigInt("101000111", 2) - BigInt("1010", 2)) << endl;
    cout << "C " << (BigInt("1100100", 2) - BigInt("1010", 2)) << endl;
    cout << "D " << (BigInt("11000111", 2) % BigInt("1010", 2)) << endl;
}