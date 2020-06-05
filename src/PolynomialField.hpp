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
    Polynomial inverted(const Polynomial& polynomial) const;

    [[nodiscard]]
    Polynomial pow(const Polynomial& num, uint64_t pow) const;

    [[nodiscard]]
    int64_t order_of_irreducible(const Polynomial& polynomial) const;

    [[nodiscard]]
    std::vector<std::pair<Polynomial, std::size_t>> berlekampFactorization(Polynomial polynomial) const;

private:
    void _generateElements();
    
    /*
     * @note gcd(a,b) = x*a + y*b
     */
    [[nodiscard]] 
    Polynomial _gcdExtended(const Polynomial& a, const Polynomial& b, Polynomial& x, Polynomial& y) const;

    /*
     * @note transforms of any polynomial to polynomial which belongs to field
     */
    [[nodiscard]]
    Polynomial _reduceDegree(Polynomial polynomial) const;

    uint64_t _n;
    Polynomial _irreducible;
    Polynomial _from_irreducible;
    std::vector<Polynomial> _elements;
};

} // namespace lab
