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

    SECTION("String representation") {
        SECTION("empty") {
            const Polynomial<13> p1{};
            REQUIRE(to_string(p1).empty());

            const Polynomial<11> p2{};
            REQUIRE(to_string(p2, 'x', true) == "0*x^0");
        }

        SECTION("normal") {
            const Polynomial<13> p1{1, 1};
            REQUIRE(to_string(p1) == "1*x^1 + 1");

            const Polynomial<13> p2{0, 0, 1, 2, 4};
            REQUIRE(to_string(p2) == "4*x^4 + 2*x^3 + 1*x^2");
            REQUIRE(to_string(p2, 'a') == "4*a^4 + 2*a^3 + 1*a^2");
            REQUIRE(to_string(p2, '(') == "4*x^4 + 2*x^3 + 1*x^2");
            REQUIRE(to_string(p2, 'A', true) == "4*A^4 + 2*A^3 + 1*A^2 + 0*A^1 + 0*A^0");
        }
    }

    SECTION("Relational operators") {
        const Polynomial<11> p1{};
        const Polynomial<11> p2{0, 0, 0};
        REQUIRE(p1 == p2);

        const Polynomial<11> p3{3, 2, 43, 23, 12, 57};
        const Polynomial<11> p4{3, 2, 10, 1, 1, 2};
        REQUIRE(p3 == p4);

        const Polynomial<13> p5{56, 132, 46, 13, 75, 13, 2};
        const Polynomial<13> p6{56, 132, 46, 0, 75, 13, 2, 0, 0, 0};
        REQUIRE(p5 == p6);

        const Polynomial<121> p7{3, 2, 43, 23, 12, 57};
        const Polynomial<121> p8{3, 2, 10, 1, 1, 2};
        REQUIRE(p7 != p8);

        const Polynomial<17> p9{56, 132, 46, 13, 75, 13, 2};
        const Polynomial<17> p10{56, 132, 46, 13, 76, 13, 2, 0, 0, 0};
        REQUIRE(p9 != p10);
    }

    SECTION("Transforming") {
        const Polynomial<5> p1{};
        REQUIRE(to_string(transform<5, 3>(p1)).empty());

        const Polynomial<121> p2{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(to_string(transform<121, 2>(p2)) == "1*x^5 + 1*x^4 + 1*x^3 + 1*x^1");

        const Polynomial<13> p3{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(to_string(transform<13, 5>(p3)) == "2*x^6 + 2*x^2 + 2*x^1 + 4");

        const Polynomial<11> p4{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(to_string(transform<11, 17>(p4)) == to_string(p4));
    }
}
