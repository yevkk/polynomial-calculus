#pragma once

#include <BigNum.hpp>
#include <vector>

namespace lab {

/**
 * @brief Class for holding polynomials with big integer coefficients
 */
class Polynomial
{
public:
    //Polynomial(const Polynomial& that) = default;

    //explicit Polynomial(std::string_view pol_str);

    //Polynomial() = default;

    //Polynomial& operator=(const Polynomial& pol)

    //friend bool operator==(const Polynomial& left, const Polynomial& right);
    //friend bool operator!=(const Polynomial& left, const Polynomial& right);

    //friend Polynomial operator+(const Polynomial& left, const Polynomial& right);
    //friend Polynomial operator-(const Polynomial& left, const Polynomial& right);
    //friend Polynomial operator*(const Polynomial& left, const Polynomial& right);
    //friend Polynomial operator*(const Polynomial& left, const BigNum& right);
    //friend Polynomial operator*(const Polynomial& left, int right);

    //template<typename OStream>
    //friend OStream& operator<<(OStream& os, const Polynomial& pol);

    //template<typename IStream>
    //friend IStream& operator>>(IStream& is, const Polynomial& pol);

    /*
     * @return the highest power of variable with non-zero coefficient
     */
    //int degree();

    //friend void modify(Polynomial& pol, const BigNum& mod);

    //add, subtract, multiply

private:
    BigNum _field_modulo;

    // Array of polynomial's coefficients
    std::vector<BigNum> _coefs;
};

} // namespace lab

