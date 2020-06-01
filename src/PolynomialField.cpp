#include "PolynomialField.hpp"

#include <cassert>
#include <cmath>
#include <map>
#include <utility>

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

PolynomialField::PolynomialField(uint64_t p, const Polynomial &irreducible) :
        _p{p},
        _n{irreducible.degree()},
        _irreducible{irreducible} {
    assert(prime(p));

    //creating elements vector
    for (int64_t i = 0; i < p; i++) {
        _elements.push_back(Polynomial{i});
    }

    std::vector<Polynomial> tmp;
    for (int64_t n = _n - 2; n >= 0; n--) {
        tmp = _elements;
        _elements.clear();
        for (auto& item : tmp) {
            for (int64_t j = 0; j < _p; j++) {
                _elements.push_back(item * Polynomial::x(1) + Polynomial{j});
            }
        }
    }
}

/*
 * @return elements of field
 */
std::vector<Polynomial> PolynomialField::elements() const {
    return _elements;
}

uint64_t PolynomialField::getP() const {
    return _p;
}

uint64_t PolynomialField::getN() const {
    return _n;
}

Polynomial PolynomialField::getIrreducible() const {
    return _irreducible;
}

Polynomial PolynomialField::add(const Polynomial &left, const Polynomial &right) const {
    assert(left.degree() < _n);
    assert(right.degree() < _n);
    return (left + right).modify(_p);
}

Polynomial PolynomialField::subtract(const Polynomial &left, const Polynomial &right) const {
    assert(left.degree() < _n);
    assert(right.degree() < _n);
    return (left - right).modify(_p);
}

Polynomial PolynomialField::multiply(const Polynomial &left, const Polynomial &right) const {
    assert(left.degree() < _n);
    assert(right.degree() < _n);

    static std::map<std::pair<Polynomial, Polynomial>, Polynomial> cache_map;
    if (cache_map.find({left, right}) != cache_map.end()) {
        return cache_map[{left, right}];
    }
    if (cache_map.find({right, left}) != cache_map.end()) {
        return cache_map[{right, left}];
    }

    Polynomial result = (left * right).modify(_p);

    auto irreducible_coefs = _irreducible.coefficients();
    irreducible_coefs.pop_back();
    static Polynomial from_irreducible = -1 * Polynomial{irreducible_coefs};

    while (result.degree() >= _n) {
        auto tmp = result.coefficients().back() * Polynomial::x(result.degree() - _n);

        auto result_coefs = result.coefficients();
        result_coefs.pop_back();

        result = Polynomial{result_coefs} + (from_irreducible * tmp);
    }

    result = result.modify(_p);

    cache_map[{left, right}] = result;
    return result;
}

} // namespace lab
