#pragma once

#include <vector>
#include <string>
#include <optional>

namespace lab {
class Polynomial;

/**
 * @brief Converts polynomial to string
 */
std::string to_string(const Polynomial& polynomial, char var_ch = 'x', bool show_zero = false);

/**
 * @brief Class for holding polynomials with uint coefficients
 */
class Polynomial {
public:
    Polynomial(const Polynomial& that) = default;

    Polynomial(std::initializer_list<int64_t> coefs);

    Polynomial(std::vector<int64_t> coefs);

    Polynomial();

    Polynomial& operator=(const Polynomial& that) = default;

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
     * @return the vector of coefficients
     */
    [[nodiscard]]
    const std::vector<int64_t>& coefficients() const;

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
    Polynomial derivate() const;
    
    /**
     * @brief Evaluates polynomial in point
     */
    int64_t evaluate(int64_t point) const;
    

    template <typename OStream>
    friend inline OStream& operator<<(OStream& os, const Polynomial& polynomial) {
        os << to_string(polynomial);
        return os;
    }

    static Polynomial x(size_t power);

private:
    // Array of polynomial's coefficients
    std::vector<int64_t> _coefs;

    /*
     * @brief removes extra 0 from back of coefficients vector
     */
    void finilize();
};

} // namespace lab
