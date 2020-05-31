#include "PolynomialRing.hpp"

#include <cmath>
#include <cassert>

namespace lab {

namespace details {

    bool prime(const uint64_t& n){
        for(uint64_t i = 2; i <= sqrt(n); i++)
            if (n % i == 0)
                return false;
        return true;
    }

} // namespace details

PolynomialRing::PolynomialRing(uint64_t p) : _p{p} {
    assert(details::prime(p));
}

uint64_t PolynomialRing::getP() const {
    return _p;
}

Polynomial PolynomialRing::add(const Polynomial &left, const Polynomial &right) const {
    return (left + right).modify(_p);
}

Polynomial PolynomialRing::subtract(const Polynomial &left, const Polynomial &right) const {
    return (left - right).modify(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &left, const Polynomial &right) const {
    return (left * right).modify(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &polynomial, const uint64_t &num) const {
    return (polynomial * num).modify(_p);
}

Polynomial PolynomialRing::multiply(const uint64_t &num, const Polynomial &polynomial) const {
    return multiply(polynomial, num);
}

} // namespace lab
