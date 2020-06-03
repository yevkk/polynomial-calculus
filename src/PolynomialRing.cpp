#include "PolynomialRing.hpp"

#include <cmath>
#include <cassert>

namespace lab {


    namespace {
        bool prime(const uint64_t& n){
            for(uint64_t i = 2; i <= sqrt(n); i++) {
                if (n % i == 0) {
                    return false;
                }
            }
            return true;
        }
    } // namespace


uint64_t PolynomialRing::_divide_coefficients(uint64_t a, uint64_t b) const{
    return _dividing_table[a][b];
}

void PolynomialRing::_create_dividing_table(int field) {
    _dividing_table.resize(field, std::vector<uint64_t>(field, 1));
    for(int i = 1; i < field; i++){
        for(int j = 1; j < field; j++){
            int64_t res = (i * j) % field;
            _dividing_table[res][i] = j;
            _dividing_table[res][j] = i;
        }
    }
}

PolynomialRing::PolynomialRing(uint64_t p) : _p{p} {

    assert(prime(p) && "p should be prime");
    _create_dividing_table(p);
}

uint64_t PolynomialRing::getP() const {
    return _p;
}

Polynomial PolynomialRing::add(const Polynomial &left, const Polynomial &right) const {
    return (left + right).modified(_p);
}

Polynomial PolynomialRing::subtract(const Polynomial &left, const Polynomial &right) const {
    return (left - right).modified(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &left, const Polynomial &right) const {
    return (left * right).modified(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &polynomial, const uint64_t &num) const {
    return (polynomial * num).modified(_p);
}

Polynomial PolynomialRing::multiply(const uint64_t &num, const Polynomial &polynomial) const {
    return multiply(polynomial, num);
}

/*
 * @brief calculates the result of left polynomial divided by right one in terms of a ring or a field
 * @return a pair - the value of division and the remainder
 */
std::pair <Polynomial, Polynomial> PolynomialRing::div_mod(const Polynomial &left, const Polynomial &right) const {

    assert(right != Polynomial{0});
    Polynomial divided = left.modified(_p);
    Polynomial divisor = right.modified(_p);

    const auto PolyLen = divisor.degree();
    const auto PolyDiff = divided.degree() - divisor.degree();

    if(divided.degree() < divisor.degree())
        return {Polynomial{0}, divided};
    std::vector <int64_t> div(PolyDiff+1);
    std::vector <int64_t> mod;
    auto rest = divided.coefficients();



    for (int i = static_cast<int>(divided.degree()); i >= divisor.degree() && i >= 0; i--) {
        uint64_t higher_divided = rest[i];
        if(higher_divided == 0){
            div[i - divisor.degree()] = 0;
            continue;
        }
        uint64_t higher_divisor = divisor.coefficient(PolyLen);
        uint64_t next_coefficient = _divide_coefficients(higher_divided, higher_divisor);
        div[i - divisor.degree()] = next_coefficient;
        for(int j = static_cast<int>(i); j >= i - divisor.degree() && j >= 0; j--){
            rest[j] = rest[j] - ((divisor.coefficient(PolyLen - (i - j)) * next_coefficient) % _p);
            while(rest[j] < 0) {
                rest[j] += _p;
            }
        }
        assert(rest[i] == 0 && "Division coefficients are incorrect");
    }

        return {Polynomial{div}.modified(_p), Polynomial{rest}.modified(_p)};
}

/*
 * @brief calculates the result of left polynomial divided by right one in terms of a ring or a field
 * @return the result of division
 */
Polynomial PolynomialRing::divide(const Polynomial &left, const Polynomial &right) const{
    return div_mod(left, right).first;
}

/*
 * @brief calculates the remainder of left polynomial divided by right
 */
Polynomial PolynomialRing::mod(const Polynomial &left, const Polynomial &right) const{
    return div_mod(left, right).second;
}

Polynomial PolynomialRing::normalize(Polynomial &polynomial) const {
    Polynomial result(polynomial.modified(_p));
    uint64_t normalizator = 1;
    if (_p > 2) {
        normalizator = std::pow(polynomial.coefficient(polynomial.degree()), _p - 2);
    }
    return (result * normalizator).modified(_p);
}

    uint64_t PolynomialRing::evaluate(Polynomial &polynomial, uint64_t point) const {
        polynomial = polynomial.modified(_p);
        uint64_t result = 0;
        int64_t point_power = 1;
        for (size_t power = 0; power <= polynomial.degree(); ++power) {
            result += (polynomial.coefficient(power) * point_power) % _p;
            point_power *= point;
        }
        result %= _p;
        return result;
    }

    Polynomial PolynomialRing::derivate(Polynomial &polynomial) const {
        return polynomial.derivate().modified(_p);
    }

    Polynomial PolynomialRing::gcd(Polynomial left, Polynomial right) const{
        while (left != Polynomial{0} && right != Polynomial{0}) {
            left = mod(left, right);
            std::swap(left, right);
        }
        if (left == Polynomial{0})
            return right;
        return left;
    }

Polynomial PolynomialRing::cyclotomicPolinomial(uint64_t order) const {
    assert(order % _p);
    auto polynomial1 = Polynomial{1};
    auto polynomial2 = Polynomial{1};
    for (uint64_t i = 1; i <= uint64_t(sqrt(order)); i++){
        if (order % i == 0) {
            std::vector<int64_t> poly1(i, 0);
            poly1[0] = -1;
            poly1.push_back(1);
            auto pow = detail::moebiusFunction(order / i);
            if (pow == 1) {
                polynomial1 = multiply(polynomial1, poly1);
            } else if (pow == -1) {
                polynomial2 = multiply(polynomial2, poly1);
            }


            std::vector<int64_t> poly2(order / i, 0);
            poly2[0] = -1;
            poly2.push_back(1);
            pow = detail::moebiusFunction(i);
            if (pow == 1) {
                polynomial1 = multiply(polynomial1, poly2);
            } else if (pow == -1) {
                polynomial2 = multiply(polynomial2, poly2);

            }
        }
    }
    return divide(polynomial1, polynomial2);
}


namespace detail {
    std::vector<uint64_t> sieveOfEratosthenes(uint64_t n) {
        std::vector<char> prime(n + 1, true);
        prime[0] = prime[1] = false;
        for (uint64_t i = 2; i <= n; ++i) {
            if (prime[i]) {
                for (uint64_t j = i * i; j <= n; j += i)
                    prime[j] = false;
            }
        }
        std::vector<uint64_t> result(0);
        for (uint64_t number = 2; number <= n; ++number){
            if (prime[number]){
                result.push_back(number);
            }
        }
        return result;
    }

    int8_t moebiusFunction(uint64_t n) {
        if (n == 1)
            return 1;
        if (prime(n))
            return -1;
        auto primes = sieveOfEratosthenes(uint64_t(n));
        auto pow = true;
        for (const auto &prime: primes) {
            if (n % prime == 0) {
                pow = !pow;
                if (n % (prime * prime) == 0) {
                    return 0;
                }
            }
        }

        return pow ? 1 : -1;
    }
}//namespace detail


} // namespace lab
