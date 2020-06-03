#include "PolynomialField.hpp"
#include "FieldMultiplicationCache.hpp"

#include <cassert>
#include <cmath>
#include <optional>

namespace lab {

PolynomialField::PolynomialField(uint64_t p, const Polynomial &irreducible) :
        PolynomialRing{p},
        _n{irreducible.degree()},
        _irreducible{irreducible} {
    _generateElements();

    auto irreducible_coefs = _irreducible.coefficients();
    irreducible_coefs.pop_back();
    _from_irreducible = -1 * Polynomial{irreducible_coefs};
}

void PolynomialField::_generateElements() {
    _elements.reserve(std::pow(getP(), _n));
    for (int64_t i = 0; i < getP(); i++) {
        _elements.push_back(Polynomial{i});
    }

    std::vector<Polynomial> tmp;
    for (int64_t n = _n - 2; n >= 0; n--) {
        tmp = std::move(_elements);
        _elements.clear();
        for (const auto& item : tmp) {
            for (int64_t j = 0; j < getP(); j++) {
                _elements.push_back(item * Polynomial::x(1) + Polynomial{j});
            }
        }
    }
}

/*
 * @return elements of field
 */
const std::vector<Polynomial>& PolynomialField::elements() const {
    return _elements;
}

uint64_t PolynomialField::getN() const {
    return _n;
}

const Polynomial& PolynomialField::getIrreducible() const {
    return _irreducible;
}

namespace utils{
    void assert_(const Polynomial& polynomial, uint64_t n) {
        assert(polynomial.degree() < n && "polynomial is not in the field");
    }
}

Polynomial PolynomialField::add(const Polynomial &left, const Polynomial &right) const {
    utils::assert_(left, _n);
    utils::assert_(right, _n);
    return (left + right).modified(getP());
}

Polynomial PolynomialField::subtract(const Polynomial &left, const Polynomial &right) const {
    utils::assert_(left, _n);
    utils::assert_(right, _n);
    return (left - right).modified(getP());
}

Polynomial PolynomialField::multiply(const Polynomial &left, const Polynomial &right) const {
    utils::assert_(left, _n);
    utils::assert_(right, _n);

    auto cached_result = detail::FieldMultiplicationCache::instance().getResult(getP(), _irreducible, left, right);

    if (cached_result.has_value()) {
        return cached_result.value();
    }

    Polynomial result = (left * right).modified(getP());

    while (result.degree() >= _n) {
        auto tmp = result.coefficients().back() * Polynomial::x(result.degree() - _n);

        auto result_coefs = result.coefficients();
        result_coefs.pop_back();

        result = Polynomial{result_coefs} + (_from_irreducible * tmp);
    }

    result = result.modified(getP());

    detail::FieldMultiplicationCache::instance().setResult(getP(), _irreducible, left, right, result);
    return result;
}

Polynomial PolynomialField::pow(const Polynomial& poly, uint64_t power) const {
    if (power == 1){
        return poly;
    }
    if (power % 2 == 1){
        return multiply(pow(poly, power - 1), poly);
    }
    auto poly2 = pow(poly, power / 2);
    return multiply(poly2, poly2);
}

} // namespace lab
