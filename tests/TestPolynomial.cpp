#include "../src/Polynomial.hpp"

#include <sstream>

#include "catch.hpp"

TEST_CASE("Polynomials test", "[Polynomial]") {
    using namespace lab;

    SECTION("Polynomial constructor") {
        SECTION("empty") {
            const Polynomial<11> p1;
            REQUIRE(coefficient(p1, 0) == 0);
            REQUIRE(coefficient(p1, 3) == 0);
            REQUIRE(degree(p1) == 0);

            const Polynomial<11> p2{};
            REQUIRE(coefficient(p2, 0) == 0);
            REQUIRE(coefficient(p2, 4) == 0);
            REQUIRE(degree(p2) == 0);
        }

        SECTION("normal") {
            const Polynomial<13> p1{0, 0, 1, 2, 4};
            REQUIRE(coefficient(p1, 0) == 0);
            REQUIRE(coefficient(p1, 2) == 1);
            REQUIRE(coefficient(p1, 4) == 4);
            REQUIRE(coefficient(p1, 5) == 0);
            REQUIRE(coefficient(p1, 7) == 0);
            REQUIRE(degree(p1) == 4);

            const Polynomial<17> p2{3, 4, 1, 10, 0, 11, 4, 5, 1, 8};
            REQUIRE(coefficient(p2, 0) == 3);
            REQUIRE(coefficient(p2, 9) == 8);
            REQUIRE(coefficient(p2, 3) == 10);
            REQUIRE(coefficient(p2, 4) == 0);
            REQUIRE(coefficient(p2, 7) == 5);
            REQUIRE(coefficient(p2, 10) == 0);
            REQUIRE(coefficient(p2, 100) == 0);
            REQUIRE(degree(p2) == 9);
        }

        SECTION("with modification") {
            const Polynomial<5> p1{11, -6, 18, 35, -2, 9};
            REQUIRE(coefficient(p1, 0) == 1);
            REQUIRE(coefficient(p1, 1) == 4);
            REQUIRE(coefficient(p1, 2) == 3);
            REQUIRE(coefficient(p1, 3) == 0);
            REQUIRE(coefficient(p1, 4) == 3);
            REQUIRE(coefficient(p1, 5) == 4);
            REQUIRE(degree(p1) == 5);

            const Polynomial<131> p2{1132, 1048, 2934, 567, -2147, -1234, -98, 233};
            REQUIRE(coefficient(p2, 0) == 84);
            REQUIRE(coefficient(p2, 1) == 0);
            REQUIRE(coefficient(p2, 2) == 52);
            REQUIRE(coefficient(p2, 3) == 43);
            REQUIRE(coefficient(p2, 4) == 80);
            REQUIRE(coefficient(p2, 5) == 76);
            REQUIRE(coefficient(p2, 6) == 33);
            REQUIRE(coefficient(p2, 7) == 102);
            REQUIRE(coefficient(p2, 8) == 0);
            REQUIRE(coefficient(p2, 15) == 0);
            REQUIRE(degree(p2) == 7);
        }

        SECTION("tail of zeros") {
            const Polynomial<5> p1{11, -6, 0, 0, 0, 0};
            REQUIRE(degree(p1) == 1);

            const Polynomial<5> p2{11, -6, 0, 0, 5, 10, 0, 15};
            REQUIRE(degree(p1) == 1);

            const Polynomial<5> p3{0, 0, 0};
            REQUIRE(degree(p3) == 0);

            const Polynomial<5> p4{10, 15, 20, 35};
            REQUIRE(degree(p4) == 0);

            const Polynomial<5> p5{1, 0, 0};
            REQUIRE(degree(p5) == 0);
        }
    }
}
