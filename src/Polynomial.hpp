#pragma once

#include <optional>
#include <vector>
#include <string>

namespace lab {

/**
 * @brief Class for holding polynomials with uint coefficients
 */
class Polynomial {
public:
    using coefficient_type = int64_t;

    Polynomial();
    Polynomial(std::vector<coefficient_type> coefs);
    Polynomial(std::initializer_list<coefficient_type> coefs);

    Polynomial(const Polynomial& that) = default;
    Polynomial& operator=(const Polynomial& that) = default;
    Polynomial(Polynomial&& that) noexcept = default;
    Polynomial& operator=(Polynomial&& that) noexcept = default;

    /**
     * @return the highest power of variable with non-zero coefficient
     */
    [[nodiscard]]
    size_t degree() const;

    /**
     * @return the coefficient corresponding to x^power
     */
    [[nodiscard]]
    int64_t coefficient(size_t power) const;

    /**
     * @brief calculates all coefficients by modulo
     */
    [[nodiscard]]
    Polynomial modified(int64_t modulo) const;

    /**
     * @brief calculates all coefficients by modulo
     */
    [[nodiscard]]
    Polynomial unpowered(int64_t modulo) const;

    /**
     * @return the vector of coefficients
     */
    [[nodiscard]]
    const std::vector<coefficient_type>& coefficients() const;

    static std::optional<Polynomial> from_string(std::string str);

    friend bool operator==(const Polynomial& left, const Polynomial& right);
    friend bool operator!=(const Polynomial& left, const Polynomial& right);
    friend bool operator<(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator+(const Polynomial& left, const Polynomial& right);
    friend Polynomial operator-(const Polynomial& left, const Polynomial& right);

    friend Polynomial operator*(const Polynomial& left, const Polynomial& right);
    friend Polynomial operator*(const Polynomial& left, int64_t right);
    friend Polynomial operator*(int64_t left, const Polynomial& right);

    /**
     * @brief Calculates derivative from polynomial
     */
    [[nodiscard]]
    Polynomial derivate() const;

    /**
     * @brief Evaluates polynomial in point
     */
    [[nodiscard]]
    coefficient_type evaluate(coefficient_type point) const;

    template <typename OStream>
    friend OStream& operator<<(OStream& os, const Polynomial& polynomial);

    [[nodiscard]]
    static Polynomial x(size_t power);

private:
    // Array of polynomial's coefficients
    std::vector<int64_t> _coefs;

    /**
     * @brief removes extra 0 from back of coefficients vector
     */
    void finalize();
};

/**
 * @brief Converts polynomial to string
 */
std::string to_string(const Polynomial& polynomial, char var = 'x', bool show_zero = false);

template <typename OStream>
inline OStream& operator<<(OStream& os, const Polynomial& polynomial)
{
    os << to_string(polynomial);
    return os;
}

} // namespace lab
