#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    BigInt sum("1");
    string s;
    while (getline(cin, s)) {
        BigInt n(s);
        sum *= n;
    }
    cout << sum << endl;
}