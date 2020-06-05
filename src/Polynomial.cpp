#include "Polynomial.hpp"

#include <algorithm>
#include <utility>
#include <cctype>
#include <regex>

namespace lab {

Polynomial::Polynomial() {
    _coefs.push_back(0);
}

Polynomial::Polynomial(std::initializer_list<int64_t> coefs) : _coefs{coefs} {
    if (_coefs.empty()) {
        _coefs.push_back(0);
    }

    finilize();
}

Polynomial::Polynomial(std::vector<int64_t> coefs) : _coefs{coefs} {
    if (_coefs.empty()) {
        _coefs.push_back(0);
    }

    finilize();
}

/**
 * @return the highest power of variable with non-zero coefficient
 */
size_t Polynomial::degree() const {
    return _coefs.size() - 1;
}

/**
 * @return the coefficient corresponding to x^power
 */
int64_t Polynomial::coefficient(size_t power) const {
    return power < _coefs.size() ? _coefs[power] : 0;
}

/**
 * @return the vector of coefficients
 */
const std::vector<int64_t>& Polynomial::coefficients() const {
    return _coefs;
}

/*
 * @brief removes extra 0 from back of coefficients vector
 */
void Polynomial::finilize() {
    while ((_coefs.back() == 0) && (_coefs.begin() + 1 != _coefs.end())) {
        _coefs.pop_back();
    }
}

/*
 * @brief calculates all coefficients by modulo
 */
Polynomial Polynomial::modified(int64_t modulo) const {
    Polynomial result = *this;

    for(auto& item : result._coefs) {
        while (item < 0) {
            item += modulo;
        }
        item = item % modulo;
    }

    result.finilize();

    return result;
}

Polynomial Polynomial::unpowered(const int64_t modulo) const
{
    auto result = *this;
    for (int i = 0; i <= degree(); ++i) {
        result._coefs[i] = 0;
        if (i % modulo == 0) {
            result._coefs[i / modulo] = _coefs[i];
        }
    }

    result.finilize();
    return result;
}

bool operator==(const Polynomial &left, const Polynomial &right) {
    return left._coefs == right._coefs;
}

bool operator!=(const Polynomial &left, const Polynomial &right) {
    return !(left == right);
}

bool operator<(const Polynomial &left, const Polynomial &right) {
    if (left.degree() != right.degree()) {
        return left.degree() < right.degree();
    }
    return std::lexicographical_compare(left.coefficients().rbegin(), left.coefficients().rend(), right.coefficients().rbegin(), right.coefficients().rend());
}

Polynomial operator+(const Polynomial &left, const Polynomial &right) {
    Polynomial result{};
    auto res_degree = std::max(left.degree(), right.degree());
    result._coefs.reserve(res_degree + 1);

    result._coefs[0] = left.coefficient(0) + right.coefficient(0);
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.push_back(left.coefficient(i) + right.coefficient(i));
    }

    result.finilize();

    return result;
}

Polynomial operator-(const Polynomial &left, const Polynomial &right) {
    Polynomial result{};
    auto res_degree = std::max(left.degree(), right.degree());
    result._coefs.reserve(res_degree + 1);

    result._coefs[0] = left.coefficient(0) - right.coefficient(0);
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.push_back(left.coefficient(i) - right.coefficient(i));
    }

    result.finilize();

    return result;
}

Polynomial operator*(const Polynomial &left, const Polynomial &right) {
    Polynomial result{};
    result._coefs = std::vector<int64_t>(left.degree() + right.degree() + 1, 0);

    for (unsigned i = 0; i <= left.degree(); i++) {
        for (unsigned j = 0; j <= right.degree(); j++) {
            result._coefs[i + j] += left.coefficient(i) * right.coefficient(j);
        }
    }

    result.finilize();

    return result;
}

Polynomial operator*(const Polynomial &left, int64_t right) {
    Polynomial result(left);

    for (auto &item : result._coefs) {
        item = item * right;
    }

    result.finilize();

    return result;
}

Polynomial operator*(int64_t left, const Polynomial &right) {
    return right * left;
}

Polynomial Polynomial::x(size_t power) {
    std::vector<int64_t> coefs(power, 0);
    coefs.push_back(1);

    return Polynomial{coefs};
}

/**
 * @brief Converts polynomial to string
 */
std::string to_string(const Polynomial &polynomial, char var, const bool show_zero) {
    //TODO: (yevkk) 1*x^n = x^n
    std::string result;

    if (!std::isalpha(var)) {
        var = 'x';
    }

    for (auto i = polynomial.degree(); i > 0; i--) {
        if (polynomial.coefficient(i) || show_zero) {

            if (!result.empty()) {
                result += (polynomial.coefficient(i) >= 0) ? " +" : " ";
            }

            result += std::to_string(polynomial.coefficient(i)) + '*' + var + '^' + std::to_string(i);
        }
    }

    if (show_zero) {
        if (!result.empty()) {
            result += (polynomial.coefficient(0) >= 0) ? " +" : " ";
        }
        result += std::to_string(polynomial.coefficient(0)) + '*' + var + "^0";
    } else if (polynomial.coefficient(0)) {
        if (!result.empty()) {
            result += (polynomial.coefficient(0) >= 0) ? " +" : " ";
        }
        result += std::to_string(polynomial.coefficient(0));
    }

    return (result.empty() ? "0" : result);
}

std::optional<Polynomial> Polynomial::from_string(const std::string_view str)
{
    /// <coefficient>*(optional)<variable>^<power>
    const std::regex r{R"(([-+]?\b\d*)(\*?)(\w+)(\^)(\d+\b)|([+-]?\b\d*))"};
    
    struct Match {

    };

    return {};
}

Polynomial Polynomial::derivate() const {
    Polynomial derivate(*this);
    if (derivate._coefs.size() == 1) {
        derivate._coefs[0] = 0;
    } else {
        for (size_t power = 1; power < derivate._coefs.size(); ++power) {
            derivate._coefs[power - 1] = derivate._coefs[power] * power;
        }
        derivate._coefs.pop_back();
    } 
    return derivate;
}

int64_t Polynomial::evaluate(int64_t point) const {
    int64_t result = 0;
    int64_t point_power = 1;
    for (size_t power = 0; power < _coefs.size(); ++power) {
        result += _coefs[power] * point_power;
        point_power *= point;
    }
    return result;
}

} // namespace lab
