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
    cout << BigInt("123").pow(BigInt("456")) << endl;
}