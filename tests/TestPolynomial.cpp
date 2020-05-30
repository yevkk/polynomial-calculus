#include "../src/Polynomial.hpp"

#include <sstream>

#include "catch.hpp"

TEST_CASE("Polynomials test", "[Polynomial]") {
    using namespace lab;

    SECTION("Polynomial constructor") {
        SECTION("empty") {
            const Polynomial p1;
            REQUIRE(p1.coefficient(0) == 0);
            REQUIRE(p1.coefficient(3) == 0);
            REQUIRE(p1.degree() == 0);

            const Polynomial p2{};
            REQUIRE(p2.coefficient(0) == 0);
            REQUIRE(p2.coefficient(4) == 0);
            REQUIRE(p2.degree() == 0);
        }

        SECTION("normal") {
            const Polynomial p1{0, 0, 1, 2, 4};
            REQUIRE(p1.coefficient(0) == 0);
            REQUIRE(p1.coefficient(2) == 1);
            REQUIRE(p1.coefficient(4) == 4);
            REQUIRE(p1.coefficient(5) == 0);
            REQUIRE(p1.coefficient(7) == 0);
            REQUIRE(p1.degree() == 4);

            const Polynomial p2{3, 4, 1, 10, 0, 11, 4, 5, 1, 8};
            REQUIRE(p2.coefficient(0) == 3);
            REQUIRE(p2.coefficient(9) == 8);
            REQUIRE(p2.coefficient(3) == 10);
            REQUIRE(p2.coefficient(4) == 0);
            REQUIRE(p2.coefficient(7) == 5);
            REQUIRE(p2.coefficient(10) == 0);
            REQUIRE(p2.coefficient(100) == 0);
            REQUIRE(p2.degree() == 9);
        }

        SECTION("tail of zeros") {
            const Polynomial p1{11, -6, 0, 0, 0, 0};
            REQUIRE(p1.degree() == 1);

            const Polynomial p2{11, -6, 0, 0, 5, 10, 0, 0};
            REQUIRE(p2.degree() == 5);

            const Polynomial p3{0, 0, 0};
            REQUIRE(p3.degree() == 0);

            const Polynomial p5{-12, 0, 0};
            REQUIRE(p5.degree() == 0);
        }
    }

    SECTION("String representation") {
        SECTION("empty") {
            const Polynomial p1{};
            REQUIRE(to_string(p1) == "0");

            const Polynomial p2{};
            REQUIRE(to_string(p2, 'x', true) == "0*x^0");
        }

        SECTION("normal") {
            const Polynomial p1{1, 1};
            REQUIRE(to_string(p1) == "1*x^1 +1");

            const Polynomial p2{-4, 1, -8};
            REQUIRE(to_string(p2) == "-8*x^2 +1*x^1 -4");

            const Polynomial p3{0, 0, 1, -2, 4};
            REQUIRE(to_string(p3) == "4*x^4 -2*x^3 +1*x^2");
            REQUIRE(to_string(p3, 'a') == "4*a^4 -2*a^3 +1*a^2");
            REQUIRE(to_string(p3, '(') == "4*x^4 -2*x^3 +1*x^2");
            REQUIRE(to_string(p3, 'A', true) == "4*A^4 -2*A^3 +1*A^2 +0*A^1 +0*A^0");
        }
    }

    SECTION("Relational operators") {
        const Polynomial p1{};
        const Polynomial p2{0, 0, 0};
        REQUIRE(p1 == p2);

        const Polynomial p3{3, 2, 43, 23, 12, 57};
        const Polynomial p4{3, 2, 10, 1, 1, 2};
        REQUIRE(p3 != p4);

        const Polynomial p5{56, 132, 46, 13, 75, 13, 2};
        const Polynomial p6{56, 132, 46, 13, 75, 13, 2, 0, 0, 0};
        REQUIRE(p5 == p6);

        const Polynomial p7{3, 2, 43, 23, 12, 57};
        const Polynomial p8{3, 2, 10, 1, 1, 2};
        REQUIRE(p7 != p8);

        const Polynomial p9{56, 132, 46, 13, 75, 13, 2};
        const Polynomial p10{56, 132, 46, 13, 76, 13, 2, 0, 0, 0};
        REQUIRE(p9 != p10);
    }

    SECTION("Addition") {
        SECTION("simple") {
            const Polynomial p1{};
            const Polynomial p2{};
            REQUIRE(p1 + p2 == Polynomial{});
            REQUIRE((p1 + p2).degree() == 0);
            REQUIRE(p1 + p2 == p2 + p1);

            const Polynomial p3{11, 13, 1, 21, 22, 0, 19, 7, 16, 5};
            const Polynomial p4{2, 5, 11, 0, 0, 0, 2, 7, 5, 3};
            REQUIRE(p3 + p4 == Polynomial{13, 18, 12, 21, 22, 0, 21, 14, 21, 8});
            REQUIRE((p3 + p4).degree() == 9);
            REQUIRE(p3 + p4 == p4 + p3);

            const Polynomial p5{21, 34, 56, 0, 0, 23, 19, 0, 78, 5, 1, 32, 45, 24, 94, 100, 55};
            const Polynomial p6{56, 21, 11, 7, 0, 88, 2, 7, 5, 0};
            REQUIRE(p5 + p6 == Polynomial{77, 55, 67, 7, 0, 111, 21, 7, 83, 5, 1, 32, 45, 24, 94, 100, 55});
            REQUIRE((p5 + p6).degree() == 16);
            REQUIRE(p5 + p6 == p6 + p5);

            const Polynomial p7{43, 23, 6, 2, 0, 1, 45, 11};
            const Polynomial p8{-22, 0, 9, 2, -6, 2, -12, 34, -21, 56, -67, 55, -33};
            REQUIRE(p7 + p8 == Polynomial{21, 23, 15, 4, -6, 3, 33, 45, -21, 56, -67, 55, -33});
            REQUIRE((p7 + p8).degree() == 12);
            REQUIRE(p7 + p8 == p8 + p7);
        }
    }

    SECTION("Subtraction") {
        SECTION("simple") {
            const Polynomial p1{};
            const Polynomial p2{};
            REQUIRE(p1 - p2 == Polynomial{});
            REQUIRE((p1 - p2).degree() == 0);

            const Polynomial p3{45, 32, 45, 57, 43, 59, 67, 70, 34};
            const Polynomial p4{12, 23, 44, 57, 12, 23, 45, 67, 12};
            REQUIRE(p3 - p4 == Polynomial{33, 9, 1, 0, 31, 36, 22, 3, 22});
            REQUIRE((p3 - p4).degree() == 8);

            REQUIRE(p3 - Polynomial{} == p3);
            REQUIRE(p4 - Polynomial{} == p4);

            const Polynomial p5{34, 35, 32, -20, 23, 34, 14, 25, 43, 42, -30, 37, 39, 40};
            const Polynomial p6{-12, 13, 14, 15, 16, -34, 14, 25, 44, -67, 12};
            REQUIRE(p5 - p6 == Polynomial{46, 22, 18, -35, 7, 68, 0, 0, -1, 109, -42, 37, 39, 40});
            REQUIRE((p5 - p6).degree() == 13);
        }
    }

    SECTION("Multiplication") {
        SECTION("by number") {
            const Polynomial p1{5, -4, 7, 5, -6, 10, -10, 3, 2};
            int num = 0;

            REQUIRE(p1 * num == Polynomial{});
            REQUIRE(num * p1 == p1 * num);

            num = 3;
            REQUIRE(p1 * num == Polynomial{15, -12, 21, 15, -18, 30, -30, 9, 6});
            REQUIRE(num * p1 == p1 * num);

            const Polynomial p2{21, 34, -21, 25, 2, -3, 4, 21, 0, -21, 35};
            num = -33;
            REQUIRE(p2 * num == Polynomial{-693, -1122, 693, -825, -66, 99, -132, -693, 0, 693, -1155});
            REQUIRE(num * p2 == p2 * num);

            const Polynomial p3{123, 145, -165, 134, 23, -146, 175, 123, -178, 111};
            num = 201;
            REQUIRE(p3 * num == Polynomial{24723, 29145, -33165, 26934, 4623, -29346, 35175, 24723, -35778, 22311});
            REQUIRE(num * p3 == p3 * num);
        }

        SECTION("by polynomial") {
            const Polynomial p1{1, 5, 1};
            const Polynomial p2{2, 6, -7, 1};
            REQUIRE(p1 * p2 == Polynomial{2, 16, 25, -28, -2, 1});
            REQUIRE(p1 * p2 == p2 * p1);
            REQUIRE((p1 * p2).degree() == 5);

            REQUIRE(p1 * Polynomial{} == Polynomial{});

            const Polynomial p3{1, -2, 3, 4, 1, -2, 3, 4, -1, 2, 3, 4};
            const Polynomial p4{4, 3, -2, 1, -4, 3, 2, 1};
            REQUIRE(p3 * p4 == Polynomial{4, -5, 4, 30, 4, 1, -8, 20, 12, 22, 12, 10, 28, -5, 0, -4, 20, 11, 4});
            REQUIRE(p3 * p4 == p4 * p3);
            REQUIRE((p3 * p4).degree() == 18);
        }
    }
}
