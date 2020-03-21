#include <Polynomial.hpp>

int main () {
    const int m = 11;
    lab::Polynomial<m> p{12, 11, 13};
    std::cout << lab::to_string(p, 'A');
    return 0;
}