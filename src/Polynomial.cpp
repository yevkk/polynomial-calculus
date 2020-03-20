#include <Polynomial.hpp>
#include <cassert>
#include <cctype>

namespace lab {

template<uint64_t modulo>
std::optional<Polynomial<modulo>> from_string(std::string_view pol_str)
{
    auto error = [](){ assert(0 && "Incorrect input"); };

    /*
     * Possible states:
     *  0 - BigNum coefficient
     *  1 - space before '*'
     *  2 - space after '*'
     *  3 - space before '^'
     *  4 - space after '^'
     *  5 - int degree
     *  6 - space before '+'
     *  7 - space after '+'
     */
    int state = 0;
    std::string coef_str;
    std::string degree_str;
    char var_ch = ' ';

    for (int i = 0; i < pol_str.size(); i++) {
        char c = pol_str[i];

        switch (state)
        {
        case 0:
            if (std::isdigit(c)) {
                coef_str += c;
            } else if (c == '*') {
                state = 2;
            } else if (c == '+') {
                _coefs[0]  = _coefs[0] + BigNum(coef_str); //TODO: fix empty _coefs case
                coef_str.clear();
                state = 7;
            } else if (c == '^') {
                error;
            } else if (c == ' ') {
                state = 1;
            } else if (std::isalpha(c)) {
                if (var_ch == ' ') {
                    var_ch = c;
                } else {
                    if (var_ch != c) {
                        error;
                    }
                }
                state = 3;
            }
        continue;

        case 1: {
            break;
        }
        case 2: {
            break;
        }
        case 3: {
            break;
        }
//            case 4: {
//                break;
//            }
//            case 5: {
//                break;
//            }
//            case 6: {
//                break;
//            }

        }
    }
}

} // namespace lab