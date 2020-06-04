#pragma once

#include <vector>

namespace lab::utils {

    /**
     *  @return All divisors of n
     */
    std::vector<int> get_divisors(int64_t n) {
        std::vector<int> result;

        for (int64_t i = 2; i * i <= n; ++i) {

            while (n % i == 0) {
                result.push_back(i);
                n /= i;
            }
        }
        if (n != 1) {
            result.push_back(n);
        }
        return result;
    }
}