#include <Polynomial.hpp>
#include <PolynomialRing.hpp>
#include <PolynomialField.hpp>
#include <iostream>

int main() {
    const lab::PolynomialRing r13{13};
    std::vector <lab::Polynomial> ans = r13.cyclotomicFactorization(1);

    return 0;
}
