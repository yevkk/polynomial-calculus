#include <Polynomial.hpp>
#include <PolynomialRing.hpp>
#include <PolynomialField.hpp>

#include <iostream>

int main() {
    using namespace lab;

    PolynomialField field{3, Polynomial{1, 0, 1}};

    auto elements = field.elements();
    for (size_t i = 0; i < elements.size(); i++) {
        std::cout << elements[i] << ',';
        for (size_t j = 0; j < elements.size(); j++) {
            std::cout << field.multiply(elements[i], elements[j]) << ',';
        }
        std::cout << std::endl;
    }

    return 0;
}
