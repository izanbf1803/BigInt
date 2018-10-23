#include "BigInt.hpp"
#include <iostream>
using namespace std;

using bigint::BigInt;

int main()
{
    BigInt x = BigInt("12345678910") + BigInt("-82373284723847");
    cout << x << " " << x/2 << endl;
    cout << BigInt("1233235252533255354674438888") / BigInt("-7") << endl;
    cout << BigInt("1233235252533255354674438888") / -7 << endl;
    cout << BigInt("23322") % BigInt("1") << endl;
    cout << BigInt("-12345678910") + BigInt("82373284723847") << endl;
    cout << BigInt("-12345678910") + BigInt("-82373284723847") << endl;
    cout << BigInt("12345678910") + BigInt("-82373284723847") << endl;
    cout << BigInt("-12345678910") - BigInt("82373284723847") << endl;
    cout << BigInt("-12345678910") - BigInt("-82373284723847") << endl;
    cout << BigInt("12345678910") - BigInt("-82373284723847") << endl;
    cout << BigInt(123).pow(45) << endl;
    cout << endl << endl; 
    BigInt a, b;
    a = 0, b = 1;
    for (int i = 0; i < 100; ++i) {
        BigInt temp = b;
        b += a;
        a = temp;
    }
    cout << a << endl;
}   