#include <Polynomial.hpp>
#include <PolynomialRing.hpp>
#include <PolynomialField.hpp>
#include <iostream>

int main() {

    using namespace lab;

    Polynomial p {2, 2, 2};
    PolynomialField f {3, p};
    PolynomialRing r {3};

    for (auto i : r.irreducibleOfOrder(2)) {
        std::cout << i << "   " << f.isIrreducible(i) << std::endl;
    }
    return 0;
}
