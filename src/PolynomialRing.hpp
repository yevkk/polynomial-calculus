#pragma once

#include "Polynomial.hpp"

namespace lab {

    namespace detail{
        std::vector<uint64_t> sieveOfEratosthenes (uint64_t n);

        int8_t moebiusFunction (uint64_t n);

        /**
         * @brief Finds auxiliary polynomials for cyclotomic factorization algorithm
         */
        Polynomial rPolynom(uint64_t i, uint64_t order, uint64_t polyMod);

        std::vector<uint64_t> integerFactorization(uint64_t n);

        int rankOfMatrix(std::vector<std::vector<uint64_t>> matrix);
    }//namespace detail

    class PolynomialRing {
    public:
        PolynomialRing(const PolynomialRing& that) = default;

        explicit PolynomialRing(uint64_t p);

        [[nodiscard]]
        uint64_t getP() const;

        [[nodiscard]]
        virtual Polynomial add(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        virtual Polynomial subtract(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        virtual Polynomial multiply(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        Polynomial multiply(const Polynomial& polynomial, const uint64_t & num) const;

        [[nodiscard]]
        Polynomial multiply(const uint64_t & num, const Polynomial& polynomial) const;


        [[nodiscard]]
        Polynomial divide(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        Polynomial mod(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        std::pair<Polynomial, Polynomial> div_mod(const Polynomial& left, const Polynomial& right) const;

        [[nodiscard]]
        Polynomial gcd(Polynomial left, Polynomial right) const;

        [[nodiscard]]
        Polynomial cyclotomicPolinomial(uint64_t order) const;

        [[nodiscard]]
        std::vector<Polynomial> cyclotomicFactorization(uint64_t order) const;

        [[nodiscard]]
        std::vector<Polynomial> irreducibleOfOrder(uint64_t order) const;

        [[nodiscard]]
        Polynomial pow(const Polynomial& num, uint64_t pow) const;

        /**
         * @brief Finds normalized polynomial in field
         */
        [[nodiscard]] Polynomial normalize(const Polynomial &polynomial) const;

        /**
         * @brief Evaluates polynomial in point
         */
        [[nodiscard]] uint64_t evaluate(Polynomial &polynomial, uint64_t point) const;

        /**
         * @brief Calculates derivative from polynomial
         */
        [[nodiscard]] Polynomial derivate(Polynomial &polynomial) const;

        /**
         * @brief Checks if polynomial is irreducible over the field by modulo
         */
        [[nodiscard]] bool isIrreducible(const Polynomial &polynomial) const;

        /**
         *  @return Order of irreducible polynomial
         */
        [[nodiscard]]
        int order_of_irreducible (const Polynomial& polynomial) const;

        /**
         * @return Order of polynomial
         */
         [[nodiscard]]
         int order(const Polynomial& polynomial) const;

        /**
         *  @return vector of roots
         */
        [[nodiscard]] std::vector<uint64_t> roots(Polynomial &polynomial) const;

        [[nodiscard]] std::vector<uint64_t> returnRoots(Polynomial& gPoly, Polynomial& toMod) const;


        /**
         * @brief GCD method requires only GCD function,
         *        Matrix method - uses matrix rank calculation
         */
        enum class CountPolicy {
            GCD,
            Matrix
        };

        /**
         * @return the count of roots of equation P(x) = 0
         */
        [[nodiscard]] int countRoots(const Polynomial &polynomial, PolynomialRing::CountPolicy policy = PolynomialRing::CountPolicy::GCD) const;

        /**
         * @return vector of pair - <multiplicity of root, count of roots>
         */
        [[nodiscard]] std::vector<std::pair<int, uint64_t>> countMultipleRoots(const Polynomial &polynomial) const;

        [[nodiscard]]
        std::vector<std::pair<Polynomial, std::size_t>> berlekampFactorization(Polynomial polynomial) const;

    private:
        uint64_t _p;
        std::vector <std::vector <uint64_t>> _dividing_table;
        [[nodiscard]] uint64_t _divide_coefficients(uint64_t a, uint64_t b) const;
        void _create_dividing_table(int field);
    };

} // namespace lab
