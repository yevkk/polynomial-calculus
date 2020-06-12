#include "PolynomialField.hpp"
#include "FieldMultiplicationCache.hpp"

#include <cassert>
#include <cmath>
#include <optional>
#include <numeric>
#include <algorithm>

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

Polynomial PolynomialField::_reduceDegree(Polynomial polynomial) const {
    while (polynomial.degree() >= _n) {
            auto tmp = polynomial.coefficients().back() * Polynomial::x(polynomial.degree() - _n);

            auto polynomial_coefs = polynomial.coefficients();
            polynomial_coefs.pop_back();

            polynomial = Polynomial{ polynomial_coefs} + (_from_irreducible * tmp);
    }

    polynomial = polynomial.modified(getP());

    return polynomial;
}

Polynomial PolynomialField::multiply(const Polynomial &left, const Polynomial &right) const {
    utils::assert_(left, _n);
    utils::assert_(right, _n);

    auto cached_result = detail::FieldMultiplicationCache::instance().getResult(getP(), _irreducible, left, right);

    if (cached_result.has_value()) {
        return cached_result.value();
    }

    Polynomial result = (left * right).modified(getP());

    result = _reduceDegree(result);

    detail::FieldMultiplicationCache::instance().setResult(getP(), _irreducible, left, right, result);
    return result;
}

Polynomial PolynomialField::_gcdExtended(const Polynomial& a, const Polynomial& b, Polynomial& x, Polynomial& y) const {
    if (a == Polynomial({ 0 })) {
        x = Polynomial({ 0 });
        y = Polynomial({ 1 });
        return b;
    }

    Polynomial x1, y1;
    const auto&[quotient, remainder] = div_mod(b, a);
    Polynomial GCD = _gcdExtended(remainder, a, x1, y1);

    Polynomial tmp = _reduceDegree(PolynomialRing::multiply(quotient, x1));
    x = subtract(y1, tmp);
    y = x1;

    return GCD;
}



Polynomial PolynomialField::inverted(const Polynomial& polynomial) const {
    Polynomial p, result, tmp, div;
    p = _reduceDegree(polynomial);

    div = _gcdExtended(p, _irreducible, result, tmp);
    if (div.coefficient(0) != 1) {
        result = divide(result, Polynomial({ div.coefficient(0) }));
    }
    return result;
}


Polynomial PolynomialField::pow(const Polynomial& poly, uint64_t power) const {
    if (power == 1){
        return poly;
    }
    if (power % 2 == 1){
        return multiply(pow(poly, power - 1), poly);
    }
    const auto poly2 = pow(poly, power / 2);
    return multiply(poly2, poly2);
}

/*
 * @brief checks if element is a field generator
 */
bool PolynomialField:: isGenerator(const Polynomial &element) const {
    utils::assert_(element, _n);

    auto polynomial = element;
    auto power_counter = 1;

    while (polynomial != Polynomial{1} && power_counter != _elements.size()) {
        polynomial = multiply(polynomial, element);
        power_counter++;
    }

    return power_counter == _elements.size() - 1;
}
/*
 * @return vector of field generators
 */
std::vector<Polynomial> PolynomialField::getGenerators() const {
    std::vector<Polynomial> result;

    for (const auto& item : _elements) {
        if (isGenerator(item)) {
            result.push_back(item);
        }
    }

    return result;
}

} // namespace lab
