#pragma once

#include <cassert>
#include <cctype>
#include <algorithm>
#include <tuple>
#include <iostream>

namespace lab {
    namespace detail {
        constexpr inline std::tuple<int64_t, int64_t, int64_t> gcdex(int64_t a, int64_t b) {
            if (b == 0) {
                return std::make_tuple(a, 1, 0);
            } else {
                auto [g, x, y] = gcdex(b, a % b);
                return std::make_tuple(g, y, x - y * (a / b));
            }
        }
        
        template<uint64_t modulo>
        constexpr uint64_t reverse(uint64_t number) {
            auto [g, x, y] = gcdex(number, modulo);
            
            assert(g == 1);
            x = (x + (abs(x / modulo) + 1) * modulo) % modulo;
            return x;
        }
    }

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
        _coefs.push_back(item);
    }

    if (coefs.size() == 0) {
        _coefs.push_back(0);
    }

    while (!_coefs.back() && (_coefs.begin() + 1 != _coefs.end())) {
        _coefs.pop_back();
    }
}

/*
 * @return the highest power of variable with non-zero coefficient
 */
template<uint64_t modulo>
int Polynomial<modulo>::degree() const {
    return _coefs.size() - 1;
}

/*
 * @return the coefficient corresponding to x^power
 */
template<uint64_t modulo>
uint64_t Polynomial<modulo>::coefficient(unsigned power) const {
    return power < _coefs.size() ? _coefs[power] : 0;
}

/*
 * @brief Converts polynomial to string
 */
template<uint64_t modulo>
std::string Polynomial<modulo>::to_string(char var_ch, bool show_zero) const {
    //TODO: (yevkk) 1*x^n = x^n
    std::string result;
    bool flag = false;

    if (!std::isalpha(var_ch)) { //TODO: maybe change too assert()
        var_ch = 'x';
    }

    for (auto i = degree(); i > 0; i--) {
         if (_coefs[i] || show_zero) {
             if (flag) {
                 result += " + ";
             } else {
                 flag = true;
             }
             result += std::to_string(_coefs[i]) + '*' + var_ch + '^' + std::to_string(i);
         }
    }

    if (show_zero) {
        result += (result.empty() ? "" : " + ") + std::to_string(_coefs[0]) + '*' + var_ch + "^0";
    } else if (_coefs[0]){
        result +=  (result.empty() ? "" : " + ") + std::to_string(_coefs[0]);
    }

    return (result.empty() ? "0" : result);
}

template<uint64_t modulo>
std::vector<uint64_t> Polynomial<modulo>::coefficients() const {
    return _coefs;
}

/*
 * @brief Converts string to polynomial
 * @return Polynomial<modulo> object if string has correct format, otherwise - std::nullopt
 */
template<uint64_t modulo>
std::optional<Polynomial<modulo>> Polynomial<modulo>::from_string(std::string_view pol_str) {
    Polynomial<modulo> result;
    result._coefs.push_back(0);

    constexpr static auto increase_coef = [&](unsigned index, uint64_t value)
    {
        result._coefs[0] = 15;
        //TODO: (yevkk) implement this
    };

    /*
     * Possible states:
     *  0 - coefficient with spaces before and after
     *  1 - space between '*' and variable char
     *  2 - space between variable char and '^'
     *  3 - degree value with spaces before and after
     */
    int state = 0; //TODO: (yevkk) change to enum
    std::string coef_str;
    std::string degree_str;
    char var_ch = ' ';
    bool digit_met = false;
    bool space_met = false;

    for (const auto& c : pol_str) {

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

    return result;
}

template<uint64_t mod>
bool operator==(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    return left._coefs == right._coefs;
}

template<uint64_t mod>
bool operator!=(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    return !(left == right);
}

template<typename OStream, uint64_t mod>
OStream& operator<<(OStream& os, const Polynomial<mod>& pol) {
    os << pol.to_string();
    return os;
}

template<uint64_t mod>
Polynomial<mod> operator+(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    auto res_degree = std::max(left.degree(), right.degree());

    result._coefs[0] = (left.coefficient(0) + right.coefficient(0)) % mod;
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.emplace_back((left.coefficient(i) + right.coefficient(i)) % mod);
    }

    while (!result._coefs.back() && (result._coefs.begin() + 1 != result._coefs.end())) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator-(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    auto res_degree = std::max(left.degree(), right.degree());

    result._coefs[0] = (left.coefficient(0) - right.coefficient(0) + mod) % mod;
    for (unsigned i = 1; i <= res_degree; i++) {
        result._coefs.emplace_back((left.coefficient(i) - right.coefficient(i) + mod) % mod);
    }

    while (!result._coefs.back() && (result._coefs.begin() + 1 != result._coefs.end())) {
        result._coefs.pop_back();
    }

    return result;
}

template <uint64_t mod>
Polynomial<mod> operator*(const Polynomial<mod>& left, const Polynomial<mod>& right) {
    Polynomial<mod> result{};
    result._coefs = std::vector<uint64_t>(left.degree() + right.degree() + 1, 0);

    for (unsigned i = 0; i <= left.degree(); i++) {
        for (unsigned j = 0; j <= right.degree(); j++) {
            result._coefs[i + j] += left.coefficient(i) * right.coefficient(j);
        }
    }

    for (auto& item:result._coefs) {
        item %= mod;
    }

    while (!result._coefs.back() && (result._coefs.begin() + 1 != result._coefs.end())) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator*(const Polynomial<mod>& left, uint64_t right) {
    Polynomial<mod> result(left);

    for (auto& item : result._coefs) {
        item = (item * right) % mod;
    }

    while (!result._coefs.back() && (result._coefs.begin() + 1 != result._coefs.end())) {
        result._coefs.pop_back();
    }

    return result;
}

template<uint64_t mod>
Polynomial<mod> operator*(uint64_t left, const Polynomial<mod>& right) {
    return right * left;
}

template<uint64_t modSrc, uint64_t modRes>
Polynomial<modRes> transform(const Polynomial<modSrc>& pol) {
    Polynomial<modRes> result{};

    result._coefs = pol._coefs;
    for (auto& item : result._coefs) {
        item %= modRes;
    }

    return result;
}

template<uint64_t modulo>
void normalize(Polynomial<modulo> &origin) {
    uint64_t reverse = detail::reverse<modulo>(origin.coefficient(origin.degree()));
    for (size_t curr_degree = 0; curr_degree <= origin.degree(); ++curr_degree) {
        origin._coefs[curr_degree] = (origin._coefs[curr_degree] * reverse) % modulo;
    }
}

template<uint64_t modulo>
Polynomial<modulo> Polynomial<modulo>::normalized() const{
    Polynomial<modulo> poly_copy(*this);
    normalize(poly_copy);
    return poly_copy;
}

template<uint64_t modulo>
uint64_t Polynomial<modulo>::calculateIn(uint64_t point) const {
    uint64_t powered_point = 1;
    uint64_t result = 0;
    
    for (size_t curr_degree = 0; curr_degree <= this->degree(); ++curr_degree) {
        result += powered_point * this->coefficient(curr_degree);
        result %= modulo;
        
        powered_point *= point;
        powered_point %= modulo;
    }
    
    return result;
}

template<uint64_t modulo>
Polynomial<modulo> derivative(const Polynomial<modulo>& origin) {
    Polynomial<modulo> result;
    result._coefs.resize( (origin.degree() < 1) ? 1 : origin.degree());
    result._coefs[0] = 0;
    
    for (size_t curr_degree = 1; curr_degree <= origin.degree(); ++curr_degree) {
        result._coefs[curr_degree - 1] = (curr_degree * origin.coefficient(curr_degree)) % modulo;
    }
    
    while (!result._coefs.back() && (result._coefs.size() > 1)) {
        result._coefs.pop_back();
    }
    
    return result;
}

} // namespace lab
