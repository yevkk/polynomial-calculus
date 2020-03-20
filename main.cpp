#include <Polynomial.hpp>

int main () {
    const int m = 11;
    lab::Polynomial<m> p;
    lab::from_string<m>("123 + 122  + 121");
    return 0;
}