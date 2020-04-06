#pragma once

#include <vector>
#include <string>
#include <optional>

namespace lab {

/**
 * @brief Class for holding polynomials with uint coefficients
 */
template<uint64_t modulo>
class Polynomial {
public:
    Polynomial(const Polynomial<modulo>& that) = default;

    Polynomial(std::initializer_list<int64_t> coefs);

    Polynomial();

    Polynomial& operator=(const Polynomial<modulo>& that) = default;

    /**
     * @return the highest power of variable with non-zero coefficient
     */
    int degree() const;

    /**
     * @return the coefficient corresponding to x^power
     */
    uint64_t coefficient(unsigned power) const;

    /**
     * @return the vector of coefficients
     */
    std::vector<uint64_t> coefficients() const;

    /**
    * @brief Converts polynomial to string
    */
    std::string to_string(char var_ch = 'x', bool show_zero = false) const;

    /**
     * @brief Converts string to polynomial
     * @return Polynomial<modulo> object if string has correct format, otherwise - null
     */
    static std::optional<Polynomial<modulo>> from_string(std::string_view pol_str);

    template<uint64_t mod>
    friend bool operator==(const Polynomial<mod>& left, const Polynomial<mod>& right);
    template<uint64_t mod>
    friend bool operator!=(const Polynomial<mod>& left, const Polynomial<mod>& right);

    /**
     * @note considering all coefficients are correct
     */
    template<uint64_t mod>
    friend Polynomial<mod> operator+(const Polynomial<mod>& left, const Polynomial<mod>& right);
    template<uint64_t mod>
    friend Polynomial<mod> operator-(const Polynomial<mod>& left, const Polynomial<mod>& right);
    template<uint64_t mod>
    friend Polynomial<mod> operator*(const Polynomial<mod>& left, const Polynomial<mod>& right);
    template<uint64_t mod>
    friend Polynomial<mod> operator*(const Polynomial<mod>& left, uint64_t right);
    template<uint64_t mod>
    friend Polynomial<mod> operator*(uint64_t left, const Polynomial<mod>& right);

    template <typename OStream, uint64_t mod>
    friend OStream& operator<<(OStream& os, const Polynomial<mod>& pol);

    //template<typename IStream>
    //friend IStream& operator>>(IStream& is, const Polynomial& pol); TODO: (yevkk) implement me

    template <uint64_t modSrc, uint64_t modRes>
    friend Polynomial<modRes> transform(const Polynomial<modSrc>& pol);

private:
    // Array of polynomial's coefficients
    std::vector<uint64_t> _coefs;
};

} // namespace lab

#include "Polynomial.hxx"