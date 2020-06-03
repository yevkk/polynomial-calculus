#pragma once

#include "Polynomial.hpp"

namespace lab {

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




        /**
         * @brief Finds normalized polynomial in field
         */
        [[nodiscard]] Polynomial normalize(Polynomial &polynomial) const;

        /**
         * @brief Evaluates polynomial in point
         */
        [[nodiscard]] uint64_t evaluate(Polynomial &polynomial, uint64_t point) const;

        /**
         * @brief Calculates derivative from polynomial
         */
        [[nodiscard]] Polynomial derivate(Polynomial &polynomial) const;


    private:
        uint64_t _p;
        std::vector <std::vector <uint64_t>> _dividing_table;
        [[nodiscard]] uint64_t _divide_coefficients(uint64_t a, uint64_t b) const;
        void _create_dividing_table(int field);
    };

} // namespace lab
