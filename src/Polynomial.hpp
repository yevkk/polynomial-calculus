#pragma once

#include <vector>
#include <string>
#include <optional>

namespace lab {

/**
 * @brief Class for holding polynomials with big integer coefficients
 */
template<uint64_t modulo>
class Polynomial
{
public:
    Polynomial(const Polynomial<modulo>& that) = default;

    Polynomial(std::initializer_list<int64_t> coefs);

    Polynomial();

    //Polynomial& operator=(const Polynomial<modulo>& that) TODO: implement me

    /*
     * @return the highest power of variable with non-zero coefficient
     */
    template <uint64_t mod>
    friend int degree(const Polynomial<mod>& pol);

    /*
     * @return the coefficient corresponding to x^power
     */
    template <uint64_t mod>
    friend int coefficient(const Polynomial<mod>& pol, unsigned power);

    /*
    * @brief Converts polynomial to string
    */
    template <uint64_t mod>
    friend std::string to_string(const Polynomial<mod>& pol, char var_ch, bool show_zero);

    /*
     * @brief Converts string to polynomial
     * @return Polynomial<modulo> object if string has correct format, otherwise - null
     */
    template <uint64_t mod>
    friend std::optional<Polynomial<mod>> from_string(std::string_view pol_str);

    template <uint64_t mod>
    friend bool operator==(const Polynomial<mod>& left, const Polynomial<mod>& right);
    template <uint64_t mod>
    friend bool operator!=(const Polynomial<mod>& left, const Polynomial<mod>& right);

    template <uint64_t mod>
    friend Polynomial<mod> operator+(const Polynomial<mod>& left, const Polynomial<mod>& right);
    //friend Polynomial operator-(const Polynomial& left, const Polynomial& right); TODO: implement me
    //friend Polynomial operator*(const Polynomial& left, const Polynomial& right); TODO: implement me
    //friend Polynomial operator*(const Polynomial& left, int right); TODO: implement me

    template <typename OStream, uint64_t mod>
    friend OStream& operator<<(OStream& os, const Polynomial<mod>& pol);

    //template<typename IStream>
    //friend IStream& operator>>(IStream& is, const Polynomial& pol); TODO: implement me

    template <uint64_t modSrc, uint64_t modRes>
    friend Polynomial<modRes> transform(const Polynomial<modSrc>& pol);

    //add, subtract, multiply

private:
    // Array of polynomial's coefficients
    std::vector<uint64_t> _coefs;
};

} // namespace lab

#include "Polynomial.hxx"