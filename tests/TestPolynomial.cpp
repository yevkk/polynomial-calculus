#include "../src/Polynomial.hpp"

#include <sstream>

#include "catch.hpp"

TEST_CASE("Polynomials test", "[Polynomial]") {
    using namespace lab;

    SECTION("Polynomial constructor") {
        SECTION("empty") {
            const Polynomial<11> p1;
            REQUIRE(p1.coefficient(0) == 0);
            REQUIRE(p1.coefficient(3) == 0);
            REQUIRE(p1.degree() == 0);

            const Polynomial<11> p2{};
            REQUIRE(p2.coefficient(0) == 0);
            REQUIRE(p2.coefficient(4) == 0);
            REQUIRE(p2.degree() == 0);
        }

        SECTION("normal") {
            const Polynomial<13> p1{0, 0, 1, 2, 4};
            REQUIRE(p1.coefficient(0) == 0);
            REQUIRE(p1.coefficient(2) == 1);
            REQUIRE(p1.coefficient(4) == 4);
            REQUIRE(p1.coefficient(5) == 0);
            REQUIRE(p1.coefficient(7) == 0);
            REQUIRE(p1.degree() == 4);

            const Polynomial<17> p2{3, 4, 1, 10, 0, 11, 4, 5, 1, 8};
            REQUIRE(p2.coefficient(0) == 3);
            REQUIRE(p2.coefficient(9) == 8);
            REQUIRE(p2.coefficient(3) == 10);
            REQUIRE(p2.coefficient(4) == 0);
            REQUIRE(p2.coefficient(7) == 5);
            REQUIRE(p2.coefficient(10) == 0);
            REQUIRE(p2.coefficient(100) == 0);
            REQUIRE(p2.degree() == 9);
        }

        SECTION("with modification") {
            const Polynomial<5> p1{11, -6, 18, 35, -2, 9};
            REQUIRE(p1.coefficient(0) == 1);
            REQUIRE(p1.coefficient(1) == 4);
            REQUIRE(p1.coefficient(2) == 3);
            REQUIRE(p1.coefficient(3) == 0);
            REQUIRE(p1.coefficient(4) == 3);
            REQUIRE(p1.coefficient(5) == 4);
            REQUIRE(p1.degree() == 5);

            const Polynomial<131> p2{1132, 1048, 2934, 567, -2147, -1234, -98, 233};
            REQUIRE(p2.coefficient(0) == 84);
            REQUIRE(p2.coefficient(1) == 0);
            REQUIRE(p2.coefficient(2) == 52);
            REQUIRE(p2.coefficient(3) == 43);
            REQUIRE(p2.coefficient(4) == 80);
            REQUIRE(p2.coefficient(5) == 76);
            REQUIRE(p2.coefficient(6) == 33);
            REQUIRE(p2.coefficient(7) == 102);
            REQUIRE(p2.coefficient(8) == 0);
            REQUIRE(p2.coefficient(15) == 0);
            REQUIRE(p2.degree() == 7);
        }

        SECTION("tail of zeros") {
            const Polynomial<5> p1{11, -6, 0, 0, 0, 0};
            REQUIRE(p1.degree() == 1);

            const Polynomial<5> p2{11, -6, 0, 0, 5, 10, 0, 15};
            REQUIRE(p1.degree() == 1);

            const Polynomial<5> p3{0, 0, 0};
            REQUIRE(p3.degree() == 0);

            const Polynomial<5> p4{10, 15, 20, 35};
            REQUIRE(p4.degree() == 0);

            const Polynomial<5> p5{1, 0, 0};
            REQUIRE(p5.degree() == 0);
        }
    }

    SECTION("String representation") {
        SECTION("empty") {
            const Polynomial<13> p1{};
            REQUIRE(p1.to_string() == "0");

            const Polynomial<11> p2{};
            REQUIRE(p2.to_string('x', true) == "0*x^0");
        }

        SECTION("normal") {
            const Polynomial<13> p1{1, 1};
            REQUIRE(p1.to_string() == "1*x^1 + 1");

            const Polynomial<13> p2{0, 0, 1, 2, 4};
            REQUIRE(p2.to_string() == "4*x^4 + 2*x^3 + 1*x^2");
            REQUIRE(p2.to_string('a') == "4*a^4 + 2*a^3 + 1*a^2");
            REQUIRE(p2.to_string('(') == "4*x^4 + 2*x^3 + 1*x^2");
            REQUIRE(p2.to_string('A', true) == "4*A^4 + 2*A^3 + 1*A^2 + 0*A^1 + 0*A^0");
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
        REQUIRE(transform<5, 3>(p1).to_string() == "0");

        const Polynomial<121> p2{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(transform<121, 2>(p2).to_string() == "1*x^5 + 1*x^4 + 1*x^3 + 1*x^1");

        const Polynomial<13> p3{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(transform<13, 5>(p3).to_string() == "2*x^6 + 2*x^2 + 2*x^1 + 4");

        const Polynomial<11> p4{56, 132, 46, 13, 75, 13, 2};
        REQUIRE(transform<11, 17>(p4).to_string() == p4.to_string());
    }

    SECTION("Addition") {
        SECTION("simple") {
            const Polynomial<17> p1{};
            const Polynomial<17> p2{};
            REQUIRE(p1 + p2 == Polynomial<17>{});
            REQUIRE((p1 + p2).degree() == 0);
            REQUIRE(p1 + p2 == p2 + p1);

            const Polynomial<23> p3{11, 13, 1, 21, 22, 0, 19, 7, 16, 5};
            const Polynomial<23> p4{2, 5, 11, 0, 0, 0, 2, 7, 5, 3};
            REQUIRE(p3 + p4 == Polynomial<23>{13, 18, 12, 21, 22, 0, 21, 14, 21, 8});
            REQUIRE((p3 + p4).degree() == 9);
            REQUIRE(p3 + p4 == p4 + p3);

            const Polynomial<131> p5{21, 34, 56, 0, 0, 23, 19, 0, 78, 5, 1, 32, 45, 24, 94, 100, 55};
            const Polynomial<131> p6{56, 21, 11, 7, 0, 88, 2, 7, 5, 0};
            REQUIRE(p5 + p6 == Polynomial<131>{77, 55, 67, 7, 0, 111, 21, 7, 83, 5, 1, 32, 45, 24, 94, 100, 55});
            REQUIRE((p5 + p6).degree() == 16);
            REQUIRE(p5 + p6 == p6 + p5);

            const Polynomial<71> p7{43, 23, 6, 2, 0, 1, 45, 11};
            const Polynomial<71> p8{22, 0, 9, 2, 6, 2, 12, 34, 21, 56, 67, 55, 33};
            REQUIRE(p7 + p8 == Polynomial<71>{65, 23, 15, 4, 6, 3, 57, 45, 21, 56, 67, 55, 33});
            REQUIRE((p7 + p8).degree() == 12);
            REQUIRE(p7 + p8 == p8 + p7);
        }

        SECTION("with modification") {
            const Polynomial<2> p1{1, 1, 1, 1, 0, 0, 1, 1, 0, 1};
            const Polynomial<2> p2{0, 1, 1, 0, 0, 0, 0, 1, 1};
            REQUIRE(p1 + p2 == Polynomial<2>{1, 0, 0, 1, 0, 0, 1, 0, 1, 1});
            REQUIRE((p1 + p2).degree() == 9);
            REQUIRE(p1 + p2 == p2 + p1);

            const Polynomial<37> p3{21, 34, 21, 25, 2, 3, 4, 21, 0, 21, 35};
            const Polynomial<37> p4{21, 34, 26, 3, 9, 36, 32, 21, 21, 20, 24, 34, 36, 36, 36, 21, 23};
            REQUIRE(p3 + p4 == Polynomial<37>{5, 31, 10, 28, 11, 2, 36, 5, 21, 4, 22, 34, 36, 36, 36, 21, 23});
            REQUIRE((p3 + p4).degree() == 16);
            REQUIRE(p3 + p4 == p4 + p3);

            const Polynomial<199> p5{123, 145, 165, 134, 23, 146, 175, 123, 198, 111, 143, 43, 98, 143, 165, 129};
            const Polynomial<199> p6{154, 162, 153, 141, 123, 67, 89, 98, 123, 198, 176, 187, 134, 123, 123, 123, 198};
            REQUIRE(p5 + p6 == Polynomial<199>{78, 108, 119, 76, 146, 14, 65, 22, 122, 110, 120, 31, 33, 67, 89, 53, 198});
            REQUIRE((p5 + p6).degree() == 16);
            REQUIRE(p5 + p6 == p6 + p5);
        }

        SECTION("tail of zeros in result") {
            const Polynomial<2> p1{1, 0, 1, 1, 1, 1, 1, 1, 1};
            const Polynomial<2> p2{0, 1, 1, 1, 1, 1, 1, 1, 1};
            REQUIRE(p1 + p2 == Polynomial<2>{1, 1});
            REQUIRE((p1 + p2).degree() == 1);

            const Polynomial<5> p3{1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
            const Polynomial<5> p4{1, 1, 1, 1, 4, 3, 2, 1, 4, 3, 2, 1};
            REQUIRE(p3 + p4 == Polynomial<5>{2, 3, 4});
            REQUIRE((p3 + p4).degree() == 2);

            const Polynomial<11> p5{10, 1, 10, 1, 10, 1, 10, 1, 10};
            const Polynomial<11> p6{1, 10, 1, 10, 1, 10, 1, 10, 1};
            REQUIRE(p5 + p6 == Polynomial<11>{});
            REQUIRE((p5 + p6).degree() == 0);
        }
    }

    SECTION("Subtraction") {
        SECTION("simple") {
            const Polynomial<17> p1{};
            const Polynomial<17> p2{};
            REQUIRE(p1 - p2 == Polynomial<17>{});
            REQUIRE((p1 - p2).degree() == 0);

            const Polynomial<71> p3{45, 32, 45, 57, 43, 59, 67, 70, 34};
            const Polynomial<71> p4{12, 23, 44, 57, 12, 23, 45, 67, 12};
            REQUIRE(p3 - p4 == Polynomial<71>{33, 9, 1, 0, 31, 36, 22, 3, 22});
            REQUIRE((p3 - p4).degree() == 8);

            REQUIRE(p3 - Polynomial<71>{} == p3);
            REQUIRE(p4 - Polynomial<71>{} == p4);

            const Polynomial<47> p5{34, 35, 32, 35, 23, 34, 14, 25, 43, 42, 45, 37, 39, 40};
            const Polynomial<47> p6{12, 13, 14, 15, 16, 17, 14, 25, 43, 12, 12};
            REQUIRE(p5 - p6 == Polynomial<47>{22, 22, 18, 20, 7, 17, 0, 0, 0, 30, 33, 37, 39, 40});
            REQUIRE((p5 - p6).degree() == 13);
        }

        SECTION("with modification") {
            const Polynomial<5> p1{1, 2, 1, 2, 3, 4, 2, 1, 3, 1};
            const Polynomial<5> p2{4, 3, 3, 3, 4, 2, 3, 3, 3, 4};
            REQUIRE(p1 - p2 == Polynomial<5>{2, 4, 3, 4, 4, 2, 4, 3, 0, 2});
            REQUIRE((p1 - p2).degree() == 9);

            const Polynomial<37> p3{21, 34, 21, 25, 2, 3, 4, 21, 0, 21, 35};
            const Polynomial<37> p4{21, 34, 26, 3, 9, 36, 32, 21, 21, 20, 24, 34, 36, 36, 36, 21, 23};
            REQUIRE(p3 - p4 == Polynomial<37>{0, 0, 32, 22, 30, 4, 9, 0, 16, 1, 11, 3, 1, 1, 1, 16, 14});
            REQUIRE((p3 - p4).degree() == 16);

            const Polynomial<199> p5{123, 145, 165, 134, 23, 146, 175, 123, 198, 111, 143, 43, 98, 143};
            const Polynomial<199> p6{154, 162, 153, 141, 123, 67, 89, 98, 123, 198, 176, 187, 134, 123, 123, 123, 198};
            REQUIRE(p5 - p6 == Polynomial<199>{168, 182, 12, 192, 99, 79, 86, 25, 75, 112, 166, 55, 163, 20, 76, 76, 1});
            REQUIRE((p5 - p6).degree() == 16);
        }

        SECTION("tail of zeros in result") {
            const Polynomial<11> p1{5, 5, 5, 5, 6, 10, 10, 3, 2};
            REQUIRE(p1 - p1 == Polynomial<11>{});
            REQUIRE((p1 - p1).degree() == 0);

            const Polynomial<11> p2{1, 7, 5, 5, 10, 10, 10, 3, 2};
            REQUIRE(p1 - p2 == Polynomial<11>{4, 9, 0, 0, 7});
            REQUIRE((p1 - p2).degree() == 4);

            const Polynomial<157> p3{123, 43, 125, 22, 43, 154, 143, 124, 132, 1, 4, 34, 45};
            const Polynomial<157> p4{122, 43, 126, 22, 42, 154, 144, 124, 132, 1, 4, 34, 45};
            REQUIRE(p3 - p4 == Polynomial<157>{1, 0, 156, 0, 1, 0, 156});
            REQUIRE((p3 - p4).degree() == 6);
        }
    }

    SECTION("Multiplication") {
        SECTION("by number") {
            const Polynomial<11> p1{5, 4, 7, 5, 6, 10, 10, 3, 2};
            uint64_t num = 0;

            REQUIRE(p1 * num == Polynomial<11>{});
            REQUIRE(num * p1 == p1 * num);

            num = 11;
            REQUIRE(p1 * num == Polynomial<11>{});
            REQUIRE(num * p1 == p1 * num);

            num = 3;
            REQUIRE(p1 * num == Polynomial<11>{4, 1, 10, 4, 7, 8, 8, 9, 6});
            REQUIRE(num * p1 == p1 * num);

            const Polynomial<37> p2{21, 34, 21, 25, 2, 3, 4, 21, 0, 21, 35};
            num = 321;
            REQUIRE(p2 * num == Polynomial<37>{7, 36, 7, 33, 13, 1, 26, 7, 0, 7, 24});
            REQUIRE(num * p2 == p2 * num);

            const Polynomial<181> p3{123, 145, 165, 134, 23, 146, 175, 123, 178, 111, 143, 43, 98, 143};
            num = 654;
            REQUIRE(p3 * num == Polynomial<181>{78, 167, 34, 32, 19, 97, 58, 78, 29, 13, 126, 67, 18, 126});
            REQUIRE(num * p3 == p3 * num);
        }

        SECTION("by polynomial") {
            const Polynomial<11> p1{1, 5, 1};
            const Polynomial<11> p2{2, 6, 7, 1};
            REQUIRE(p1 * p2 == Polynomial<11>{2, 5, 6, 9, 1, 1});
            REQUIRE(p1 * p2 == p2 * p1);
            REQUIRE((p1 * p2).degree() == 5);

            REQUIRE(p1 * Polynomial<11>{} == Polynomial<11>{});

            const Polynomial<5> p3{1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
            const Polynomial<5> p4{4, 3, 2, 1, 4, 3, 2, 1};
            REQUIRE(p3 * p4 == Polynomial<5>{4, 1, 0, 0, 3, 3, 4, 0, 3, 4, 3, 0, 4, 3, 3, 0, 0, 1, 4});
            REQUIRE(p3 * p4 == p4 * p3);
            REQUIRE((p3 * p4).degree() == 18);

            const Polynomial<157> p5{123, 145, 5, 134, 23, 146};
            const Polynomial<157> p6{122, 43, 126, 22, 42, 154, 144, 124, 132, 1, 4, 34, 45};
            REQUIRE(p5 * p6 == Polynomial<157>{91, 57, 49, 16, 127, 68, 95, 45, 90, 26, 63, 16, 45, 150, 152, 17, 33, 133});
            REQUIRE(p5 * p6 == p6 * p5);
            REQUIRE((p5 * p6).degree() == 17);
        }
    }
}
