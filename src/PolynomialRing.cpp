#include "PolynomialRing.hpp"

#include <cmath>
#include <cassert>
#include <numeric>

namespace lab {


    namespace {
        bool prime(const uint64_t& n){
            for(uint64_t i = 2; i <= sqrt(n); i++) {
                if (n % i == 0) {
                    return false;
                }
            }
            return true;
        }

        int64_t modulusPow(int64_t number, uint64_t power, uint64_t modulus) {
            if (power == 0) {
                return 1;
            }
            int64_t temp = modulusPow(number, power / 2, modulus) % modulus;
            int64_t result = 1;
            if (power % 2 != 0) {
                result = number % modulus;
            }
            return (temp * temp * result) % modulus;
        }
    } // namespace


uint64_t PolynomialRing::_divide_coefficients(uint64_t a, uint64_t b) const{
    return _dividing_table[a][b];
}

void PolynomialRing::_create_dividing_table(int field) {
    _dividing_table.resize(field, std::vector<uint64_t>(field, 1));
    for(int i = 1; i < field; i++){
        for(int j = 1; j < field; j++){
            int64_t res = (i * j) % field;
            _dividing_table[res][i] = j;
            _dividing_table[res][j] = i;
        }
    }
}

PolynomialRing::PolynomialRing(uint64_t p) : _p{p} {

    assert(prime(p) && "p should be prime");
    _create_dividing_table(p);
}

uint64_t PolynomialRing::getP() const {
    return _p;
}

Polynomial PolynomialRing::add(const Polynomial &left, const Polynomial &right) const {
    return (left + right).modified(_p);
}

Polynomial PolynomialRing::subtract(const Polynomial &left, const Polynomial &right) const {
    return (left - right).modified(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &left, const Polynomial &right) const {
    return (left * right).modified(_p);
}

Polynomial PolynomialRing::multiply(const Polynomial &polynomial, const uint64_t &num) const {
    return (polynomial * num).modified(_p);
}

Polynomial PolynomialRing::multiply(const uint64_t &num, const Polynomial &polynomial) const {
    return multiply(polynomial, num);
}

/*
 * @brief calculates the result of left polynomial divided by right one in terms of a ring or a field
 * @return a pair - the value of division and the remainder
 */
std::pair <Polynomial, Polynomial> PolynomialRing::div_mod(const Polynomial &left, const Polynomial &right) const {

    assert(right != Polynomial{0});
    Polynomial divided = left.modified(_p);
    Polynomial divisor = right.modified(_p);

    const auto PolyLen = divisor.degree();
    const auto PolyDiff = divided.degree() - divisor.degree();

    if(divided.degree() < divisor.degree())
        return {Polynomial{0}, divided};
    std::vector <int64_t> div(PolyDiff+1);
    std::vector <int64_t> mod;
    auto rest = divided.coefficients();



    for (int i = static_cast<int>(divided.degree()); i >= divisor.degree() && i >= 0; i--) {
        uint64_t higher_divided = rest[i];
        if(higher_divided == 0){
            div[i - divisor.degree()] = 0;
            continue;
        }
        uint64_t higher_divisor = divisor.coefficient(PolyLen);
        uint64_t next_coefficient = _divide_coefficients(higher_divided, higher_divisor);
        div[i - divisor.degree()] = next_coefficient;
        for(int j = static_cast<int>(i); j >= i - divisor.degree() && j >= 0; j--){
            rest[j] = rest[j] - ((divisor.coefficient(PolyLen - (i - j)) * next_coefficient) % _p);
            while(rest[j] < 0) {
                rest[j] += _p;
            }
        }
        assert(rest[i] == 0 && "Division coefficients are incorrect");
    }

        return {Polynomial{div}.modified(_p), Polynomial{rest}.modified(_p)};
}

/*
 * @brief calculates the result of left polynomial divided by right one in terms of a ring or a field
 * @return the result of division
 */
Polynomial PolynomialRing::divide(const Polynomial &left, const Polynomial &right) const{
    return div_mod(left, right).first;
}

/*
 * @brief calculates the remainder of left polynomial divided by right
 */
Polynomial PolynomialRing::mod(const Polynomial &left, const Polynomial &right) const{
    return div_mod(left, right).second;
}

Polynomial PolynomialRing::normalize(const Polynomial &polynomial) const {
    Polynomial result(polynomial.modified(_p));
    uint64_t normalizator = 1;
    if (_p > 2) {
        normalizator = modulusPow(polynomial.coefficient(polynomial.degree()), _p - 2, _p);
    }
    return (result * normalizator).modified(_p);
}

    uint64_t PolynomialRing::evaluate(Polynomial &polynomial, uint64_t point) const {
        polynomial = polynomial.modified(_p);
        uint64_t result = 0;
        int64_t point_power = 1;
        for (size_t power = 0; power <= polynomial.degree(); ++power) {
            result += (polynomial.coefficient(power) * point_power) % _p;
            point_power *= point;
        }
        result %= _p;
        return result;
    }

    Polynomial PolynomialRing::derivate(Polynomial &polynomial) const {
        return polynomial.derivate().modified(_p);
    }

    Polynomial PolynomialRing::gcd(Polynomial left, Polynomial right) const{
        while (left != Polynomial{0} && right != Polynomial{0}) {
            left = mod(left, right);
            std::swap(left, right);
        }
        if (left == Polynomial{0})
            return right;
        return left;
    }

Polynomial PolynomialRing::cyclotomicPolinomial(uint64_t order) const {
    assert((order % _p) && "_p can`t be a divider of order");
    if (order == 1){
        auto poly = Polynomial{-1, 1};
        return normalize(poly);
    }
    auto polynomial1 = Polynomial{1};
    auto polynomial2 = Polynomial{1};
    for (uint64_t i = 1; i <= static_cast<uint64_t>(sqrt(order)); i++){
        if (order % i == 0) {
            std::vector<int64_t> poly1(i, 0);
            poly1[0] = -1;
            poly1.push_back(1);
            auto pow = detail::moebiusFunction(order / i);
            if (pow == 1) {
                polynomial1 = multiply(polynomial1, poly1);
            } else if (pow == -1) {
                polynomial2 = multiply(polynomial2, poly1);
            }


            std::vector<int64_t> poly2(order / i, 0);
            poly2[0] = -1;
            poly2.push_back(1);
            pow = detail::moebiusFunction(i);
            if (pow == 1) {
                polynomial1 = multiply(polynomial1, poly2);
            } else if (pow == -1) {
                polynomial2 = multiply(polynomial2, poly2);
            }
        }
    }
    return divide(polynomial1, polynomial2);
}

std::vector<Polynomial> PolynomialRing::cyclotomicFactorization(uint64_t order) const {
    uint64_t factorDegree = 1,
                tmp = getP();
    while (tmp % order != 1) {
        factorDegree++;
        tmp *= getP();
    }

    Polynomial cyclotomic = cyclotomicPolinomial(order);
    std::vector<Polynomial> factors{cyclotomic};

    int64_t i = 1;
    Polynomial factorizationR = detail::rPolynom(i, order, getP());
    bool factorized = false;

    while (!factorized && i < order) {
        while (mod(factorizationR, cyclotomic).degree() == 0 && i < order - 1){
            factorizationR = detail::rPolynom(++i, order, getP());
        }

        std::vector <Polynomial> updatedFactors;
        factorized = true;

        for (auto &item: factors){
            if (item.degree() > factorDegree){
                for(int64_t c = 0; c < getP(); c++){
                    Polynomial f = gcd(item, factorizationR + Polynomial{c});
                    if (f.degree())
                        updatedFactors.push_back(f);
                    if (f.degree() > factorDegree)
                        factorized = false;
                }
            } else{
                updatedFactors.push_back(item);
            }
        }
        factors = std::move(updatedFactors);
        if (i < order - 1) {
            factorizationR = detail::rPolynom(++i, order, getP());
        }
    }

    for (auto &it: factors){
        it = normalize(it);
    }

    return factors;
}

std::vector<Polynomial> PolynomialRing::irreducibleOfOrder(uint64_t order) const {
    // expression = p^order - 1
    std::vector<uint64_t> expression_divisors = detail::integerFactorization(std::pow(_p, order) - 1);
    std::vector<uint64_t> n_divisors = detail::integerFactorization(order);

    // needed numbers -- such m that p^order - 1 % m = 0 and p^t - 1 % m != 0 for each t < order
    std::vector<uint64_t> needed;


    for(auto expression_divisor : expression_divisors){
        bool need = true;
        for(auto n_divisor : n_divisors){
            if ((uint64_t)std::pow(_p, n_divisor) % expression_divisor == 0){
                need = false;
            }
        }

        if (need){
            needed.push_back(expression_divisor);
        }
    }


    std::vector<Polynomial> irreducible;

    for(auto& cyclotomic_polynomial_order : needed){
        for(auto& irreducible_polynomial : cyclotomicFactorization(cyclotomic_polynomial_order)){
            if (irreducible_polynomial.degree() == order){
                irreducible.push_back(irreducible_polynomial);
            }
        }
    }

    return irreducible;
}


    bool PolynomialRing::isIrreducible(const Polynomial &polynomial) const {
        if(polynomial.degree() == 0)
            return false;
        auto f = normalize(polynomial);
        for(int i = 1; i <= f.degree() / 2; i++) {
            auto g = subtract(Polynomial::x(std::pow(_p, i)), Polynomial{0, 1});
            g = mod(g, f);
            if(gcd(g, f).degree() > 0)
                return false;
        }
        return true;
    }

int PolynomialRing::countRoots(const Polynomial &polynomial, CountPolicy policy) const {
    if (policy == PolynomialRing::CountPolicy::GCD) {
        auto temp = subtract(Polynomial::x(this->getP()), Polynomial{0,1}); //creating temp: x^mod - x
        temp = gcd(polynomial, temp);
        return temp.degree();
    } else {
        auto result = 0;

        // check 0 is root or not:
        if(polynomial.coefficient(0) == 0)
            result++;

        auto temp = subtract(Polynomial::x(this->getP() - 1), Polynomial{1}); //creating temp: x^mod - x

        temp = gcd(polynomial, temp);

        // b^mod-1 = 1, b in Fmod
        if(temp.degree() == this->getP() - 1) {
            int max_coef = temp.coefficient(temp.degree());
            temp = subtract(temp, multiply(Polynomial::x(temp.degree()), max_coef));
            temp = add(temp, Polynomial{max_coef});
        }

        // create circular matrix of coefs
        std::vector<std::vector<uint64_t>> matrix;

        for(int i = 0; i <= temp.degree(); i++) {
            std::vector<uint64_t> tmp_vec;
            for(int j = 0; j <= temp.degree(); j++) {
                tmp_vec.push_back(temp.coefficient((i + j) % (temp.degree() + 1)));
            }
            matrix.push_back(tmp_vec);
        }

        result += this->getP() - 1 - detail::rankOfMatrix(matrix);

        return result;
    }
}

namespace detail {
    std::vector<uint64_t> sieveOfEratosthenes(uint64_t n) {
        std::vector<char> prime(n + 1, true);
        prime[0] = prime[1] = false;
        for (uint64_t i = 2; i <= n; ++i) {
            if (prime[i]) {
                for (uint64_t j = i * i; j <= n; j += i)
                    prime[j] = false;
            }
        }
        std::vector<uint64_t> result(0);
        for (uint64_t number = 2; number <= n; ++number){
            if (prime[number]){
                result.push_back(number);
            }
        }
        return result;
    }

    int8_t moebiusFunction(uint64_t n) {
        if (n == 1)
            return 1;
        if (prime(n))
            return -1;
        auto primes = sieveOfEratosthenes(static_cast<uint64_t>(n));
        auto pow = true;
        for (const auto &prime: primes) {
            if (n % prime == 0) {
                pow = !pow;
                if (n % (prime * prime) == 0) {
                    return 0;
                }
            }
        }

        return pow ? 1 : -1;
    }

    Polynomial rPolynom(uint64_t i, uint64_t order, uint64_t polyMod){
        uint64_t m = 1, modulo = order / std::gcd(order, i), tmp = polyMod;
        while(tmp % modulo != 1){
            tmp *= polyMod;
            m++;
        }

        std::vector<int64_t> rCoefs(i * (tmp / polyMod) + 1);
        tmp = 1;
        for (uint64_t polyModPow = 0; polyModPow < m; polyModPow++, tmp *= polyMod){
            rCoefs[i * tmp] = 1;
        }

        return Polynomial(rCoefs);

    }

    std::vector<uint64_t> integerFactorization(uint64_t n){
        std::vector<uint64_t> result;

        for (uint64_t i = 1; i*i <= n; ++i) {
            if (n % i == 0) {
                result.push_back(i);
                if (i*i != n){
                    result.push_back(n/i);
                }
            }
        }

        return result;
    }


    void swap(std::vector<std::vector<uint64_t>> matrix, int row1, int row2, int col) {
        for (int i = 0; i < col; i++) {
            int temp = matrix[row1][i];
            matrix[row1][i] = matrix[row2][i];
            matrix[row2][i] = temp;
        }
    }

/* function for finding rank of matrix */
    int rankOfMatrix(std::vector<std::vector<uint64_t>> matrix) {
        int rank = matrix[0].size();

        for (int row = 0; row < rank; row++) {
            // Before we visit current row 'row', we make
            // sure that mat[row][0],....mat[row][row-1]
            // are 0.

            // Diagonal element is not zero
            if (matrix[row][row]) {
                for (int col = 0; col < matrix.size(); col++) {
                    if (col != row) {
                        // This makes all entries of current
                        // column as 0 except entry 'mat[row][row]'
                        double mult = (double)matrix[col][row] /
                                      matrix[row][row];
                        for (int i = 0; i < rank; i++)
                            matrix[col][i] -= mult * matrix[row][i];
                    }
                }
            }

                // Diagonal element is already zero. Two cases
                // arise:
                // 1) If there is a row below it with non-zero
                //    entry, then swap this row with that row
                //    and process that row
                // 2) If all elements in current column below
                //    mat[r][row] are 0, then remvoe this column
                //    by swapping it with last column and
                //    reducing number of columns by 1.
            else {
                bool reduce = true;

                /* Find the non-zero element in current
                    column  */
                for (int i = row + 1; i < matrix.size();  i++) {
                    // Swap the row with non-zero element
                    // with this row.
                    if (matrix[i][row]) {
                        swap(matrix, row, i, rank);
                        reduce = false;
                        break ;
                    }
                }

                // If we did not find any row with non-zero
                // element in current columnm, then all
                // values in this column are 0.
                if (reduce) {
                    // Reduce number of columns
                    rank--;

                    // Copy the last column here
                    for (auto & i : matrix)
                        i[row] = i[rank];
                }

                // Process this row again
                row--;
            }
        }
        return rank;
    }
}//namespace detail


} // namespace lab
