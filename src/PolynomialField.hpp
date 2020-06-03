#pragma once

#include "Polynomial.hpp"
#include "PolynomialRing.hpp"
#include <vector>

namespace lab {

/*
 * @brief class for extension of field Fp to Fq
 * @note q = p^n, where n is a degree of irreducible polynomial in Fp
 */
class PolynomialField : public PolynomialRing {
public:
    PolynomialField(const PolynomialField& that) = default;

    /*
     * @note irreducible polynomial should be normalized
     */
    PolynomialField(uint64_t p, const Polynomial& irreducible);

    /*
     * @return elements of field
     */
    [[nodiscard]]
    const std::vector<Polynomial>& elements() const;

    [[nodiscard]]
    uint64_t getN() const;

    [[nodiscard]]
    const Polynomial& getIrreducible() const;

    [[nodiscard]]
    Polynomial add(const Polynomial& left, const Polynomial& right) const final;

    [[nodiscard]]
    Polynomial subtract(const Polynomial& left, const Polynomial& right) const final;

    [[nodiscard]]
    Polynomial multiply(const Polynomial& left, const Polynomial& right) const final;

    [[nodiscard]]
    Polynomial pow(const Polynomial& num, uint64_t pow) const;

private:
    void _generateElements();

    uint64_t _n;
    Polynomial _irreducible;
    Polynomial _from_irreducible;
    std::vector<Polynomial> _elements;
};

} // namespace lab
