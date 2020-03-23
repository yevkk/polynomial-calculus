#pragma once

#include <cassert>
#include <cctype>
#include <algorithm>
#include <iostream>

namespace lab {

template<uint64_t modulo>
Polynomial<modulo>::Polynomial() {
    _coefs.emplace_back(0);
}

template<uint64_t modulo>
Polynomial<modulo>::Polynomial(std::initializer_list<int64_t> coefs) {
    for(auto item:coefs) {
        while (item < 0) {
            item += modulo;
        }
        item = item % modulo;
        _coefs.emplace_back(item);
    }

    if (!coefs.size()) {
        _coefs.emplace_back();
    }

    while (!_coefs.back() && _coefs.begin() + 1  != _coefs.end()) {
        _coefs.pop_back();
    }
}

/*
 * @return the highest power of variable with non-zero coefficient
 */
template<uint64_t mod>
int degree(const Polynomial<mod>& pol) {
    return pol._coefs.size() - 1;
}

/*
 * @return the coefficient corresponding to x^power
 */
template<uint64_t mod>
int coefficient(const Polynomial<mod>& pol, unsigned power) {
    return power < pol._coefs.size() ? pol._coefs[power] : 0;
}

/*
 * @brief Converts polynomial to string
 */
template<uint64_t mod>
std::string to_string(const Polynomial<mod>& pol, char var_ch = 'x', bool show_zero = false) {
    //TODO: 1*x^n = x^n
    std::string result;
    bool flag = false;

    if (!std::isalpha(var_ch)) { //TODO: maybe change too assert() ???
        var_ch = 'x';
    }

    for (auto i = degree(pol); i > 0; i--) {
         if (pol._coefs[i] || show_zero) {
             if (flag) {
                 result += " + ";
             } else {
                 flag = true;
             }
             result += std::to_string(pol._coefs[i]) + '*' + var_ch + '^' + std::to_string(i);
         }
    }

    if (show_zero) {
        result += (result.empty() ? "" : " + ") + std::to_string(pol._coefs[0]) + '*' + var_ch + "^0";
    } else if (pol._coefs[0]){
        result +=  (result.empty() ? "" : " + ") + std::to_string(pol._coefs[0]);
    }

    return (result.empty() ? "0" : result);
}

/*
 * @brief Converts string to polynomial
 * @return Polynomial<modulo> object if string has correct format, otherwise - null
 */
template<uint64_t mod>
std::optional<Polynomial<mod>> from_string(std::string_view pol_str) {
    Polynomial<mod> result;
    result._coefs.push_back(0);

    static auto increase_coef = [&](unsigned index, uint64_t value)
    {
        result._coefs[0] = 15;
        //TODO: implement this
    };

    /*
     * Possible states:
     *  0 - coefficient with spaces before and after
     *  1 - space between '*' and variable char
     *  2 - space between variable char and '^'
     *  3 - degree value with spaces before and after
     */
    int state = 0;
    std::string coef_str;
    std::string degree_str;
    char var_ch = ' ';
    bool digit_met = false;
    bool space_met = false;

    for (const auto& c:pol_str) {

        switch (state) {
            case 0:
                if (std::isdigit(c)) {
                    if (!digit_met) {
                        digit_met = true;
                    } else {
                        if (space_met) {
                            return std::nullopt;
                        }
                    }
                    coef_str += c;
                } else if (c == '*') {
                    state = 1;
                    digit_met = space_met = false;
                } else if (c == '+') {
                    increase_coef(0, std::stoull(coef_str));
                    coef_str.clear();
                    digit_met = space_met = false;
                } else if (c == ' ') {
                    if (digit_met) {
                        space_met = true;
                    }
                } else if (std::isalpha(c)) {
                    if (var_ch == ' ') {
                        var_ch = c;
                    } else {
                        if (var_ch != c) {
                            return std::nullopt;
                        }
                    }
                    state = 2;
                } else {
                    return std::nullopt;
                }
                break;

            case 1:
                if (std::isalpha(c)) {
                    if (var_ch == ' ') {
                        var_ch = c;
                    } else {
                        if (var_ch != c) {
                            return std::nullopt;
                        }
                    }
                    state = 2;
                } else if (c != ' ') {
                    return std::nullopt;
                }
                break;

            case 2:
                if (c == '^') {
                    state = 3;
                } else if (c == '+') {
                    increase_coef(1, std::stoull(coef_str));
                    coef_str.clear();
                    digit_met = space_met = false;
                } else if (c != ' ') {
                    return std::nullopt;
                }
                break;

            case 3:
                if (std::isdigit(c)) {
                    if (!digit_met) {
                        digit_met = true;
                    } else {
                        if (space_met) {
                            return std::nullopt;
                        }
                    }
                    degree_str += c;
                } else if (c == '+') {
                    increase_coef(std::stoi(degree_str), std::stoull(coef_str));
                    degree_str.clear();
                    coef_str.clear();
                    digit_met = space_met = false;
                    state = 0;
                } else if (c == ' ') {
                    if (digit_met) {
                        space_met = true;
                    }
                } else {
                    return std::nullopt;
                }
                break;

            default:
                break;
        }
    }

    switch (state) {
        case 0:
            increase_coef(0, std::stoull(coef_str));
            break;

        case 1:
            return std::nullopt;

        case 2:
            increase_coef(1, std::stoull(coef_str));
            break;

        case 3:
            increase_coef(std::stoi(degree_str), std::stoull(coef_str));
            break;

        default:
            break;
    }

    std::cout << result._coefs[0]; //TODO: clear
    return result;
}

template<uint64_t mod>
bool operator==(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    if (degree(left) - degree(right)) return false;

    for (unsigned i = 0; i < left._coefs.size(); i++) {
        if (left._coefs[i] - right._coefs[i]) return false;
    }

    return true;
}

template<uint64_t mod>
bool operator!=(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    return !(left == right);
}

template<typename OStream, uint64_t mod>
OStream& operator<<(OStream& os, const Polynomial<mod>& pol) {
    os << to_string(pol);
    return os;
}

template<uint64_t mod>
Polynomial<mod> operator+(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    auto res_degree = std::max(degree(left), degree(right));

    result._coefs[0] = (coefficient(left, 0) + coefficient(right, 0)) % mod;
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.emplace_back((coefficient(left, i) + coefficient(right, i)) % mod);
    }

    while (!result._coefs.back() && result._coefs.begin() + 1  != result._coefs.end()) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator-(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    auto res_degree = std::max(degree(left), degree(right));

    result._coefs[0] = (coefficient(left, 0) - coefficient(right, 0) + mod) % mod;
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.emplace_back((coefficient(left, i) - coefficient(right, i) + mod) % mod);
    }

    while (!result._coefs.back() && result._coefs.begin() + 1 != result._coefs.end()) {
        result._coefs.pop_back();
    }

    return result;
}

template <uint64_t mod>
Polynomial<mod> operator*(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    result._coefs = std::vector<uint64_t>(degree(left) + degree(right) + 1, 0);

    for (unsigned i = 0; i <= degree(left); i++) {
        for (unsigned j = 0; j <= degree(right); j++) {
            result._coefs[i + j] += coefficient(left, i) * coefficient(right, j);
        }
    }

    for (auto& item:result._coefs) {
        item %= mod;
    }

    while (!result._coefs.back() && result._coefs.begin() + 1 != result._coefs.end()) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator*(const Polynomial<mod>& left, uint64_t right) {
    Polynomial<mod> result(left);

    for (auto& item:result._coefs) {
        item = (item * right) % mod;
    }

    while (!result._coefs.back() && result._coefs.begin() + 1 != result._coefs.end()) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator*(uint64_t left, const Polynomial<mod>& right) {
    return right * left;
}

template<uint64_t modSrc, uint64_t modRes>
Polynomial <modRes> transform(const Polynomial<modSrc>& pol) {
    Polynomial<modRes> result{};

    result._coefs = pol._coefs;
    for (auto& item:result._coefs) {
        item %= modRes;
    }

    return result;
}

} // namespace lab
