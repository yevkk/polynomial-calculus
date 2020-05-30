#pragma once

#include <vector>
#include <string>
#include <optional>

namespace lab {

/**
 * @brief Class for holding polynomials with uint coefficients
 */
class Polynomial {
public:
    Polynomial(const Polynomial& that) = default;

    Polynomial(std::initializer_list<int64_t> coefs);

    Polynomial();

    Polynomial& operator=(const Polynomial& that) = default;

    /**
     * @return the highest power of variable with non-zero coefficient
     */
    [[nodiscard]] size_t degree() const;

    /**
     * @return the coefficient corresponding to x^power
     */
    [[nodiscard]] int64_t coefficient(size_t power) const;

    /**
     * @return the vector of coefficients
     */
    [[nodiscard]] const std::vector<int64_t>& coefficients() const;

    friend bool operator==(const Polynomial& left, const Polynomial& right);
    friend bool operator!=(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator+(const Polynomial& left, const Polynomial& right);
    friend Polynomial operator-(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator*(const Polynomial& left, const Polynomial& right);
    friend Polynomial operator*(const Polynomial& left, int64_t right);
    friend Polynomial operator*(int64_t left, const Polynomial& right);

    template <typename OStream>
    friend OStream& operator<<(OStream& os, const Polynomial& polynomial);

private:
    // Array of polynomial's coefficients
    std::vector<int64_t> _coefs;
};

/**
* @brief Converts polynomial to string
*/
std::string to_string(const Polynomial& polynomial, char var_ch = 'x', bool show_zero = false);


} // namespace lab

#include "Polynomial.hxx"