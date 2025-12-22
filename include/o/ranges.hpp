#ifndef O_RANGES_HPP
#define O_RANGES_HPP

/**
 * O Language - Range Utilities
 * 
 * Iteration and range-based operations for N-state systems.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <iterator>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// RANGE OF ALL N STATES
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Iterator for N-state range
 */
template<uint8_t N>
class octo_range_iterator {
    uint8_t current_;
    
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = octo<N>;
    using difference_type = std::ptrdiff_t;
    using pointer = const octo<N>*;
    using reference = octo<N>;
    
    explicit constexpr octo_range_iterator(uint8_t start) noexcept 
        : current_(start) {}
    
    constexpr octo<N> operator*() const noexcept {
        return octo<N>(current_);
    }
    
    constexpr octo_range_iterator& operator++() noexcept {
        ++current_;
        return *this;
    }
    
    constexpr octo_range_iterator operator++(int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    constexpr bool operator==(const octo_range_iterator& other) const noexcept {
        return current_ == other.current_;
    }
    
    constexpr bool operator!=(const octo_range_iterator& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief Range object for iterating all N states
 * 
 * @example
 * for (auto state : octo_range<8>()) {
 *     // state takes values octo<8>(0), octo<8>(1), ..., octo<8>(7)
 * }
 */
template<uint8_t N>
class octo_range {
public:
    using iterator = octo_range_iterator<N>;
    
    constexpr iterator begin() const noexcept {
        return iterator(0);
    }
    
    constexpr iterator end() const noexcept {
        return iterator(N);
    }
    
    constexpr size_t size() const noexcept {
        return N;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// TRANSFORMATION UTILITIES
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Apply function to all N states
 * 
 * @param func Function to apply (signature: void(octo<N>))
 * 
 * @example
 * octo_for_each<8>([](auto state) {
 *     std::cout << state.value() << " ";
 * });
 * // Output: 0 1 2 3 4 5 6 7
 */
template<uint8_t N, typename Func>
constexpr void octo_for_each(Func func) {
    for (uint8_t i = 0; i < N; ++i) {
        func(octo<N>(i));
    }
}

/**
 * @brief Transform range of states
 * 
 * @param func Transform function (signature: R(octo<N>))
 * @return Array of transformed values
 * 
 * @example
 * auto doubled = octo_transform<4>([](auto x) { 
 *     return x.value() * 2; 
 * });
 * // doubled = {0, 2, 4, 6}
 */
template<uint8_t N, typename Func>
constexpr auto octo_transform(Func func) {
    using R = decltype(func(octo<N>(0)));
    std::array<R, N> result;
    
    for (uint8_t i = 0; i < N; ++i) {
        result[i] = func(octo<N>(i));
    }
    
    return result;
}

/**
 * @brief Filter states by predicate
 * 
 * @param pred Predicate function (signature: bool(octo<N>))
 * @return Vector of states satisfying predicate
 * 
 * @example
 * auto evens = octo_filter<8>([](auto x) { 
 *     return x.value() % 2 == 0; 
 * });
 * // evens = {0, 2, 4, 6}
 */
template<uint8_t N, typename Pred>
constexpr std::array<octo<N>, N> octo_filter(Pred pred) {
    std::array<octo<N>, N> result{};
    size_t count = 0;
    
    for (uint8_t i = 0; i < N; ++i) {
        octo<N> state(i);
        if (pred(state)) {
            result[count++] = state;
        }
    }
    
    // Fill remaining with default (state 0)
    for (size_t i = count; i < N; ++i) {
        result[i] = octo<N>(0);
    }
    
    return result;
}

/**
 * @brief Fold (reduce) over all N states
 * 
 * @param init Initial value
 * @param func Fold function (signature: T(T, octo<N>))
 * @return Accumulated result
 * 
 * @example
 * auto sum = octo_fold<4>(0, [](int acc, auto x) { 
 *     return acc + x.value(); 
 * });
 * // sum = 0 + 1 + 2 + 3 = 6
 */
template<uint8_t N, typename T, typename Func>
constexpr T octo_fold(T init, Func func) {
    T result = init;
    
    for (uint8_t i = 0; i < N; ++i) {
        result = func(result, octo<N>(i));
    }
    
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// COUNTING UTILITIES
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Count states satisfying predicate
 * 
 * @example
 * auto num_even = octo_count_if<8>([](auto x) { 
 *     return x.value() % 2 == 0; 
 * });
 * // num_even = 4
 */
template<uint8_t N, typename Pred>
constexpr size_t octo_count_if(Pred pred) {
    size_t count = 0;
    
    for (uint8_t i = 0; i < N; ++i) {
        if (pred(octo<N>(i))) {
            ++count;
        }
    }
    
    return count;
}

/**
 * @brief Check if any state satisfies predicate
 */
template<uint8_t N, typename Pred>
constexpr bool octo_any_of(Pred pred) {
    for (uint8_t i = 0; i < N; ++i) {
        if (pred(octo<N>(i))) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if all states satisfy predicate
 */
template<uint8_t N, typename Pred>
constexpr bool octo_all_of(Pred pred) {
    for (uint8_t i = 0; i < N; ++i) {
        if (!pred(octo<N>(i))) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check if no states satisfy predicate
 */
template<uint8_t N, typename Pred>
constexpr bool octo_none_of(Pred pred) {
    return !octo_any_of<N>(pred);
}

} // namespace o

#endif // O_RANGES_HPP
