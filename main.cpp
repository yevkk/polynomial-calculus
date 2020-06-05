#include <Polynomial.hpp>
#include <PolynomialRing.hpp>
#include <PolynomialField.hpp>
#include <iostream>

int main() {

    using namespace lab;

    Polynomial p {2, 2, 2};
    PolynomialField f {3, p};
    PolynomialRing r {3};

    Polynomial test{1, 0, 2, 2, 0, 1, 1, 0, 2, 2, 0, 1};
    const auto result = f.berlekampFactorization(test);

    for (const auto& [p, i] : result) {
        std::cout << p << " " << i << "\n";
    }

    return 0;
}
