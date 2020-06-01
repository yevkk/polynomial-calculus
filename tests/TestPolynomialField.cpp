#include "../src/PolynomialField.hpp"

#include "catch.hpp"

TEST_CASE("Polynomial Rings test", "[Polynomial ring]") {
    using namespace lab;

    PolynomialField p{2, Polynomial{1, 0, 1}};
}
