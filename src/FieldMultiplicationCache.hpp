#pragma once

#include "Polynomial.hpp"

#include <utility>
#include <queue>
#include <map>
#include <optional>

namespace lab {

namespace detail {
/*
 * @brief class for storage multiplication structures of polynomial fields
 */
class FieldMultiplicationCache {
    using MultiplicationStructure = std::map<std::pair<Polynomial, Polynomial>, Polynomial>;
public:
    FieldMultiplicationCache(const FieldMultiplicationCache &that) = delete;
    FieldMultiplicationCache(FieldMultiplicationCache &&that) = delete;

    FieldMultiplicationCache &operator=(const FieldMultiplicationCache &that) = delete;
    FieldMultiplicationCache &operator=(FieldMultiplicationCache &&that) = delete;

    static FieldMultiplicationCache &instance() {
        static FieldMultiplicationCache _instance;
        return _instance;
    }

    std::optional<Polynomial> getResult(uint64_t p, const Polynomial& irreducible, const Polynomial& left, const Polynomial& right) {
        const auto cache_item = cache_map.find({p, irreducible});
        if (cache_item != cache_map.end()) {
            if (cache_item->second.find({left, right}) != cache_item->second.end()) {
                return cache_item->second[{left, right}];
            }
            if (cache_item->second.find({right, left}) != cache_item->second.end()) {
                return cache_item->second[{right, left}];
            }
        }

        return std::nullopt;
    }

    /*
     * @note supposed that cache doesn't have results for either left * right or right * left
     */
    void setResult(uint64_t p, const Polynomial& irreducible, const Polynomial& left, const Polynomial& right, const Polynomial& result) {
        const auto cache_item = cache_map.find({p, irreducible});
        if (cache_item == cache_map.end()) {
            auto it = cache_map.emplace(std::pair{p, irreducible}, MultiplicationStructure()).first;

            if (cache_map.size() > CAPACITY) {
                cache_map.erase(cache_iterators.front());
                cache_iterators.pop();
                cache_iterators.push(it);
            }
        }

        cache_map[{p, irreducible}][{left, right}] = result;
    }

    bool contains(uint64_t p, const Polynomial& irreducible, const Polynomial& left, const Polynomial& right) {
        const auto cache_item = cache_map.find({p, irreducible});
        return cache_item != cache_map.end()
               && (cache_item->second.find({left, right}) != cache_item->second.end()
                   || cache_item->second.find({right, left}) != cache_item->second.end());
    }

private:
    FieldMultiplicationCache() = default;

    static inline constexpr size_t CAPACITY = 20;

    std::queue<std::map<std::pair<uint64_t, Polynomial>, MultiplicationStructure>::iterator> cache_iterators;

    std::map<std::pair<uint64_t, Polynomial>, MultiplicationStructure> cache_map;
};
}

} // namespace lab
