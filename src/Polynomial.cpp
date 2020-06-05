#include "Polynomial.hpp"

#include <cctype>
#include <algorithm>
#include <utility>

namespace lab {

Polynomial::Polynomial() {
    _coefs.push_back(0);
}

Polynomial::Polynomial(std::initializer_list<int64_t> coefs) : _coefs{coefs} {
    if (_coefs.empty()) {
        _coefs.push_back(0);
    }

    finalize();
}

Polynomial::Polynomial(std::vector<int64_t> coefs) : _coefs{coefs} {
    if (_coefs.empty()) {
        _coefs.push_back(0);
    }

    finalize();
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
void Polynomial::finalize() {
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

        result.finalize();

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

    result.finalize();

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

    result.finalize();

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

    result.finalize();

    return result;
}

Polynomial operator*(const Polynomial &left, int64_t right) {
    Polynomial result(left);

    for (auto &item : result._coefs) {
        item = item * right;
    }

    result.finalize();

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
std::string to_string(const Polynomial &polynomial, char var_ch, bool show_zero) {
    std::string result = "";

    if (!std::isalpha(var_ch)) {
        var_ch = 'x';
    }

    for (auto i = polynomial.degree(); i > 0; i--) {
        if (polynomial.coefficient(i) || show_zero) {

            if (!result.empty()) {
                result += (polynomial.coefficient(i) >= 0) ? " +" : " ";
            }

            if (polynomial.coefficient(i) == 1) {
                result.push_back(var_ch);
            } else if  (polynomial.coefficient(i) == - 1) {
                result += "-";
                result.push_back(var_ch);
            } else {
                result += std::to_string(polynomial.coefficient(i)) + '*' + var_ch;
            }

            if (i != 1) {
                result.push_back('^');
                result += std::to_string(i);
            }
        }
    }

    if (show_zero) {
        if (!result.empty()) {
            result += (polynomial.coefficient(0) >= 0) ? " +" : " ";
        }
        result += std::to_string(polynomial.coefficient(0)) + '*' + var_ch + "^0";
    } else if (polynomial.coefficient(0)) {
        if (!result.empty()) {
            result += (polynomial.coefficient(0) >= 0) ? " +" : " ";
        }
        result += std::to_string(polynomial.coefficient(0));
    }

    return (result.empty() ? "0" : result);
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

std::optional<Polynomial> Polynomial::from_string(std::string str) {
    std::vector<int64_t> coefs = {0};

    enum ReadingStates {
        COEF, POWER
    };

    std::string coef_str = "";
    std::string power_str = "";
    bool x_met = false;

    auto increase_coef = [&]() {
        if (x_met) {
            if (coef_str == "+") {
                coef_str = "1";
            } else if (coef_str == "-") {
                coef_str = "-1";
            }

            if (coef_str.empty()) {
                coef_str = "1";
            }

            if (power_str.empty()) {
                power_str = "1";
            }
        } else {
            if (coef_str == "+" || coef_str == "-") {
                coef_str = "0";
            }

            if (coef_str.empty()) {
                coef_str = "0";
            }

            if (power_str.empty()) {
                power_str = "0";
            }
        }

        size_t index = std::stoi(power_str);
        int64_t value = std::stoi(coef_str);

        while (index >= coefs.size()) {
            coefs.push_back(0);
        }

        coefs[index] += value;

        coef_str.clear();
        power_str.clear();
        x_met = false;
    };


    size_t i = 0;
    while (i < str.size()) {
        if (str[i] == ' ' || str[i] == '*') {
            str.erase(str.begin() + i);
        } else {
            i++;
        }
    }

    auto state = COEF;
    i = 0;
    while (i <= str.size()) {
        switch (state) {
            case COEF: {
                if (i == str.size()) {
                    increase_coef();
                    i++;
                } else if (std::isdigit(str[i])) {
                    coef_str.push_back(str[i]);
                    i++;
                } else if (str[i] == 'x') {
                    i++;
                    x_met = true;
                    if (i == str.size()) {
                        if (power_str.empty()) {
                            power_str = "1";
                        }
                        increase_coef();
                        i++;
                    } else if (str[i] == '^') {
                        i++;
                        state = POWER;
                    } else if (str[i] == '+' || str[i] == '-') {
                        increase_coef();
                        coef_str.push_back(str[i]);
                        i++;
                        state = COEF;
                    } else {
                        return std::nullopt;
                    }
                } else if (str[i] == '+' || str[i] == '-') {
                    increase_coef();
                    coef_str.push_back(str[i]);
                    i++;
                    state = COEF;
                } else {
                    return std::nullopt;
                }
                break;
            }

            case POWER: {
                if (i == str.size()) {
                    increase_coef();
                    i++;
                } else if (std::isdigit(str[i])) {
                    power_str.push_back(str[i]);
                    i++;
                } else if (str[i] == '+' || str[i] == '-') {
                    increase_coef();
                    coef_str.push_back(str[i]);
                    i++;
                    state = COEF;
                } else {
                    return std::nullopt;
                }
                break;
            }
        }
    }

    return Polynomial{coefs};
}

} // namespace lab
