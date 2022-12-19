
#include "APM.h"

typedef Olly::APM::Decimal number;

int main() {

    number::scale(100);

    std::cout << "scale = " << number::scale() << std::endl;
    std::cout << std::endl;

    number a("1234567890987654321123456789098765432112345678909876543211234567890987654321.0");
    number b("0.125");
    number c("undefined");

    std::cout << "a.is() = " << a.is() << std::endl;
    std::cout << "b.is() = " << b.is() << std::endl;
    std::cout << "c.is() = " << c.is() << std::endl;
    std::cout << std::endl;

    std::cout << "a.sign()  = " << a.sign() << std::endl;
    std::cout << "b.sign()  = " << b.sign() << std::endl;
    std::cout << "c.sign()  = " << c.sign() << std::endl;
    std::cout << std::endl;

    std::cout << "a == b = " << (a == b) << std::endl;
    std::cout << "a != b = " << (a != b) << std::endl;
    std::cout << "a >= b = " << (a >= b) << std::endl;
    std::cout << "a <= b = " << (a <= b) << std::endl;
    std::cout << "a >  b = " << (a > b) << std::endl;
    std::cout << "a <  b = " << (a < b) << std::endl;
    std::cout << std::endl;

    c = a + b;
    std::cout << "a + b = " << c.to_string() << std::endl;
    c = a - b;
    std::cout << "a - b = " << c.to_string() << std::endl;
    c = a * b;
    std::cout << "a * b = " << c.to_string() << std::endl;
    c = a / b;
    std::cout << "a / b = " << c.to_string() << std::endl;
    c = a % b;
    std::cout << "a % b = " << c.to_string() << std::endl;
    std::cout << std::endl;

    std::cout << "a.ln() = " << a.ln().to_string() << std::endl;

    return 0;
}
