#include "PolynomialField.hpp"

#include <cassert>
#include <cmath>
#include <iostream> // TODO: delete include!!!

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

    Polynomial x{0, 1};
    std::vector<Polynomial> tmp;
    for (int64_t n = _n - 2; n >= 0; n--) {
        tmp = _elements;
        _elements.clear();
        for (auto& item : tmp) {
            for (int64_t j = 0; j < _p; j++) {
                _elements.push_back(item * x + Polynomial{j});
            }
        }
    }

    for (auto& item : _elements) {
        std::cout << item << std::endl;
    }
}

/*
 * @return elements of field
 */
std::vector<Polynomial> PolynomialField::elements() const {
    return _elements;
}

} // namespace lab
