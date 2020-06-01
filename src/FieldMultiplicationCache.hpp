#pragma once

#include "Polynomial.hpp"

#include <utility>
#include <queue>
#include <map>

namespace lab {

/*
 * @brief class for storage multiplication structures of polynomial fields
 */
class FieldMultiplicationCache {
    using MultiplicationStructure = std::map<std::pair<Polynomial, Polynomial>, Polynomial>;
public:
    FieldMultiplicationCache (const FieldMultiplicationCache& that) = delete;
    FieldMultiplicationCache (FieldMultiplicationCache&& that) = delete;

    FieldMultiplicationCache& operator=(const FieldMultiplicationCache& that) = delete;
    FieldMultiplicationCache& operator=(FieldMultiplicationCache&& that) = delete;

    static FieldMultiplicationCache& instance() {
        static FieldMultiplicationCache _instance;
        return _instance;
    }

    auto operator[](const std::pair<uint64_t, Polynomial>& field_params) {
        auto [it, success] = cache_map.insert({field_params, MultiplicationStructure{}});

        if (success) {
            if (cache_map.size() > CAPACITY) {
                cache_map.erase(cache_iterators.front());
                cache_iterators.pop();
                cache_iterators.push(it);
            }
        }

        return cache_map[field_params];
    }

private:
    FieldMultiplicationCache() = default;

    const size_t CAPACITY = 20;

    std::queue<std::map<std::pair<uint64_t, Polynomial>, MultiplicationStructure>::iterator> cache_iterators;
    
    std::map<std::pair<uint64_t, Polynomial>, MultiplicationStructure> cache_map;
};

} // namespace lab
