#include "PolynomialField.hpp"
#include "FieldMultiplicationCache.hpp"

#include <cassert>
#include <cmath>
#include <optional>

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
    return (left + right).modified(_p);
}

Polynomial PolynomialField::subtract(const Polynomial &left, const Polynomial &right) const {
    assert(left.degree() < _n);
    assert(right.degree() < _n);
    return (left - right).modified(_p);
}

Polynomial PolynomialField::multiply(const Polynomial &left, const Polynomial &right) const {
    assert(left.degree() < _n);
    assert(right.degree() < _n);

    auto cached_result = detail::FieldMultiplicationCache::instance().getResult(_p, _irreducible, left, right);

    if (cached_result != std::nullopt) {
        return cached_result.value();
    }

    Polynomial result = (left * right).modified(_p);

    auto irreducible_coefs = _irreducible.coefficients();
    irreducible_coefs.pop_back();
    static Polynomial from_irreducible = -1 * Polynomial{irreducible_coefs};

    while (result.degree() >= _n) {
        auto tmp = result.coefficients().back() * Polynomial::x(result.degree() - _n);

        auto result_coefs = result.coefficients();
        result_coefs.pop_back();

        result = Polynomial{result_coefs} + (from_irreducible * tmp);
    }

    result = result.modified(_p);

    detail::FieldMultiplicationCache::instance().setResult(_p, _irreducible, left, right, result);
    return result;
}

} // namespace lab
