#include "../src/PolynomialField.hpp"
#include "../src/FieldMultiplicationCache.hpp"

#include "catch.hpp"
#include <algorithm>
#include <iostream>

TEST_CASE("Polynomial Field test", "[Polynomial field]") {
    using namespace lab;

    SECTION("F3") {
        PolynomialField F3{3, {1, 1}};
        const auto& elements = F3.elements();

        SECTION("elements") {
            REQUIRE(elements.size() == 3);

            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{2}) != elements.end());
        }

        SECTION("addition") {
            REQUIRE(F3.add(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F3.add(Polynomial{0}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F3.add(Polynomial{0}, Polynomial{2}) == Polynomial{2});
            REQUIRE(F3.add(Polynomial{1}, Polynomial{0}) == Polynomial{1});
            REQUIRE(F3.add(Polynomial{1}, Polynomial{1}) == Polynomial{2});
            REQUIRE(F3.add(Polynomial{1}, Polynomial{2}) == Polynomial{0});
            REQUIRE(F3.add(Polynomial{2}, Polynomial{0}) == Polynomial{2});
            REQUIRE(F3.add(Polynomial{2}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F3.add(Polynomial{2}, Polynomial{2}) == Polynomial{1});
        }

        SECTION("multiplication") {
            REQUIRE(F3.multiply(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F3.multiply(Polynomial{0}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F3.multiply(Polynomial{0}, Polynomial{2}) == Polynomial{0});
            REQUIRE(F3.multiply(Polynomial{1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F3.multiply(Polynomial{1}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F3.multiply(Polynomial{1}, Polynomial{2}) == Polynomial{2});
            REQUIRE(F3.multiply(Polynomial{2}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F3.multiply(Polynomial{2}, Polynomial{1}) == Polynomial{2});
            REQUIRE(F3.multiply(Polynomial{2}, Polynomial{2}) == Polynomial{1});
        }

        SECTION("powering"){
            REQUIRE(F3.pow(Polynomial{2}, 2) == F3.multiply(Polynomial{2}, Polynomial{2}));
            REQUIRE(F3.pow(Polynomial{2}, 4) == F3.multiply(F3.multiply(Polynomial{2}, Polynomial{2}),
                                                            F3.multiply(Polynomial{2}, Polynomial{2})));
        }

    }

    SECTION("F4") {
        PolynomialField F4{2, {1, 1, 1}};
        const auto& elements = F4.elements();

        SECTION("elements") {
            REQUIRE(elements.size() == 4);

            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0, 1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1, 1}) != elements.end());
        }

        SECTION("addition") {
            REQUIRE(F4.add(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F4.add(Polynomial{0}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F4.add(Polynomial{0}, Polynomial{0, 1}) == Polynomial{0, 1});
            REQUIRE(F4.add(Polynomial{0}, Polynomial{1, 1}) == Polynomial{1, 1});

            REQUIRE(F4.add(Polynomial{1}, Polynomial{0}) == Polynomial{1});
            REQUIRE(F4.add(Polynomial{1}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F4.add(Polynomial{1}, Polynomial{0, 1}) == Polynomial{1, 1});
            REQUIRE(F4.add(Polynomial{1}, Polynomial{1, 1}) == Polynomial{0, 1});

            REQUIRE(F4.add(Polynomial{0, 1}, Polynomial{0}) == Polynomial{0, 1});
            REQUIRE(F4.add(Polynomial{0, 1}, Polynomial{1}) == Polynomial{1, 1});
            REQUIRE(F4.add(Polynomial{0, 1}, Polynomial{0, 1}) == Polynomial{0});
            REQUIRE(F4.add(Polynomial{0, 1}, Polynomial{1, 1}) == Polynomial{1});

            REQUIRE(F4.add(Polynomial{1, 1}, Polynomial{0}) == Polynomial{1, 1});
            REQUIRE(F4.add(Polynomial{1, 1}, Polynomial{1}) == Polynomial{0, 1});
            REQUIRE(F4.add(Polynomial{1, 1}, Polynomial{0, 1}) == Polynomial{1});
            REQUIRE(F4.add(Polynomial{1, 1}, Polynomial{1, 1}) == Polynomial{0});
        }

        SECTION("multiplication") {
            REQUIRE(F4.multiply(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{0}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{0}, Polynomial{0, 1}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{0}, Polynomial{1, 1}) == Polynomial{0});

            REQUIRE(F4.multiply(Polynomial{1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{1}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F4.multiply(Polynomial{1}, Polynomial{0, 1}) == Polynomial{0, 1});
            REQUIRE(F4.multiply(Polynomial{1}, Polynomial{1, 1}) == Polynomial{1, 1});

            REQUIRE(F4.multiply(Polynomial{0, 1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{0, 1}, Polynomial{1}) == Polynomial{0, 1});
            REQUIRE(F4.multiply(Polynomial{0, 1}, Polynomial{0, 1}) == Polynomial{1, 1});
            REQUIRE(F4.multiply(Polynomial{0, 1}, Polynomial{1, 1}) == Polynomial{1});

            REQUIRE(F4.multiply(Polynomial{1, 1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F4.multiply(Polynomial{1, 1}, Polynomial{1}) == Polynomial{1, 1});
            REQUIRE(F4.multiply(Polynomial{1, 1}, Polynomial{0, 1}) == Polynomial{1});
            REQUIRE(F4.multiply(Polynomial{1, 1}, Polynomial{1, 1}) == Polynomial{0, 1});
        }
    }

    SECTION("F9") {
        PolynomialField F9{3, {1, 0, 1}};
        const auto& elements = F9.elements();

        SECTION("elements") {
            REQUIRE(elements.size() == 9);

            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{2}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0, 1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1, 1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{2, 1}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{0, 2}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{1, 2}) != elements.end());
            REQUIRE(std::find(elements.begin(), elements.end(), Polynomial{2, 2}) != elements.end());
        }

        SECTION("addition") {
            REQUIRE(F9.add(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{2}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{0, 1}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{1, 1}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{2, 1}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{0, 2}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{1, 2}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{0}, Polynomial{2, 2}) == Polynomial{2, 2});

            REQUIRE(F9.add(Polynomial{1}, Polynomial{0}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{1}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{2}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{0, 1}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{1, 1}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{2, 1}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{0, 2}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{1, 2}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{1}, Polynomial{2, 2}) == Polynomial{0, 2});

            REQUIRE(F9.add(Polynomial{2}, Polynomial{0}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{2}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{0, 1}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{1, 1}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{2, 1}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{0, 2}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{1, 2}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{2}, Polynomial{2, 2}) == Polynomial{1, 2});

            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{0}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{1}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{2}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{0, 1}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{1, 1}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{2, 1}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{0, 2}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{1, 2}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{0, 1}, Polynomial{2, 2}) == Polynomial{2});

            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{0}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{1}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{2}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{0, 1}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{1, 1}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{2, 1}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{0, 2}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{1, 2}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{1, 1}, Polynomial{2, 2}) == Polynomial{0});

            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{0}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{1}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{2}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{0, 1}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{1, 1}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{2, 1}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{0, 2}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{1, 2}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{2, 1}, Polynomial{2, 2}) == Polynomial{1});

            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{0}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{1}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{2}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{0, 1}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{1, 1}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{2, 1}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{0, 2}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{1, 2}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{0, 2}, Polynomial{2, 2}) == Polynomial{2, 1});

            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{0}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{1}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{2}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{0, 1}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{1, 1}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{2, 1}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{0, 2}) == Polynomial{1, 1});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{1, 2}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{1, 2}, Polynomial{2, 2}) == Polynomial{0, 1});

            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{0}) == Polynomial{2, 2});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{1}) == Polynomial{0, 2});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{2}) == Polynomial{1, 2});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{0, 1}) == Polynomial{2});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{1, 1}) == Polynomial{0});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{2, 1}) == Polynomial{1});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{0, 2}) == Polynomial{2, 1});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{1, 2}) == Polynomial{0, 1});
            REQUIRE(F9.add(Polynomial{2, 2}, Polynomial{2, 2}) == Polynomial{1, 1});
        }

        SECTION("multiplication") {
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{1}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{2}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{0, 1}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{1, 1}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{2, 1}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{0, 2}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0}, Polynomial{2, 2}) == Polynomial{0});

            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{1}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{2}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{0, 1}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{1, 1}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{2, 1}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{0, 2}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{1, 2}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{1}, Polynomial{2, 2}) == Polynomial{2, 2});

            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{1}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{2}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{0, 1}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{1, 1}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{2, 1}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{0, 2}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{1, 2}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{2}, Polynomial{2, 2}) == Polynomial{1, 1});

            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{1}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{2}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{0, 1}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{1, 1}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{2, 1}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{0, 2}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{1, 2}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{0, 1}, Polynomial{2, 2}) == Polynomial{1, 2});

            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{1}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{2}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{0, 1}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{1, 1}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{2, 1}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{0, 2}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{1, 2}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{1, 1}, Polynomial{2, 2}) == Polynomial{0, 1});

            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{1}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{2}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{0, 1}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{1, 1}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{2, 1}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{0, 2}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{1, 2}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{2, 1}, Polynomial{2, 2}) == Polynomial{2});

            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{1}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{2}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{0, 1}) == Polynomial{1});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{1, 1}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{2, 1}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{0, 2}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{1, 2}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{0, 2}, Polynomial{2, 2}) == Polynomial{2, 1});

            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{1}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{2}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{0, 1}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{1, 1}) == Polynomial{2});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{2, 1}) == Polynomial{0, 2});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{0, 2}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{1, 2}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{1, 2}, Polynomial{2, 2}) == Polynomial{1});

            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{0}) == Polynomial{0});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{1}) == Polynomial{2, 2});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{2}) == Polynomial{1, 1});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{0, 1}) == Polynomial{1, 2});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{1, 1}) == Polynomial{0, 1});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{2, 1}) == Polynomial{2, 0});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{0, 2}) == Polynomial{2, 1});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{1, 2}) == Polynomial{1, 0});
            REQUIRE(F9.multiply(Polynomial{2, 2}, Polynomial{2, 2}) == Polynomial{0, 2});
        }
    }
}
