#include <Polynomial.hpp>
#include <PolynomialRing.hpp>
#include <PolynomialField.hpp>
#include <iostream>

int main() {
    const lab::PolynomialRing ring3{3};
    std::vector <lab::Polynomial> ans = ring3.cyclotomicFactorization(52);
    for (auto &it:ans)
        std::cout<<it<<"     ____  ";

    /*const lab::Polynomial p3{2, 4, 6};
    const lab::Polynomial p4_1{0,1};
    const auto p = ring11.divide(p3, p4_1);
    const auto md = ring11.mod(p3,p4_1);
    std::cout << p << std::endl;
    const auto p1 = ring11.multiply(p4_1,p);
    const auto p2 = ring11.multiply(p,p4_1);
    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << md << std::endl;*/
    return 0;
}
