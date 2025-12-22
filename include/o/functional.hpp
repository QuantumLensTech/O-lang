#ifndef O_FUNCTIONAL_HPP
#define O_FUNCTIONAL_HPP

/**
 * @file functional.hpp
 * @brief Functional Programming Utilities
 * 
 * Map, filter, reduce and other functional operations for O<N> types.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <vector>
#include <functional>
#include <optional>

namespace o {
namespace functional {

// ═══════════════════════════════════════════════════════════════════════════
// MAP
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Map function over all states
 */
template<uint8_t N, typename Func>
auto map(Func f) -> std::vector<decltype(f(O<N>()))> {
    std::vector<decltype(f(O<N>()))> result;
    result.reserve(N);
    for (uint8_t i = 0; i < N; ++i) {
        result.push_back(f(O<N>(i)));
    }
    return result;
}

/**
 * @brief Map over array
 */
template<uint8_t N, size_t Size, typename Func>
auto map_array(const O_array<N, Size>& arr, Func f) -> std::array<decltype(f(O<N>())), Size> {
    std::array<decltype(f(O<N>())), Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = f(arr[i]);
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// FILTER
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Filter states matching predicate
 */
template<uint8_t N, typename Pred>
std::vector<O<N>> filter(Pred pred) {
    std::vector<O<N>> result;
    for (uint8_t i = 0; i < N; ++i) {
        O<N> state(i);
        if (pred(state)) {
            result.push_back(state);
        }
    }
    return result;
}

/**
 * @brief Filter array
 */
template<uint8_t N, size_t Size, typename Pred>
std::vector<O<N>> filter_array(const O_array<N, Size>& arr, Pred pred) {
    std::vector<O<N>> result;
    for (size_t i = 0; i < Size; ++i) {
        if (pred(arr[i])) {
            result.push_back(arr[i]);
        }
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// REDUCE / FOLD
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Left fold (reduce)
 */
template<uint8_t N, size_t Size, typename T, typename Func>
T foldl(const O_array<N, Size>& arr, T init, Func f) {
    T acc = init;
    for (size_t i = 0; i < Size; ++i) {
        acc = f(acc, arr[i]);
    }
    return acc;
}

/**
 * @brief Right fold
 */
template<uint8_t N, size_t Size, typename T, typename Func>
T foldr(const O_array<N, Size>& arr, T init, Func f) {
    T acc = init;
    for (size_t i = Size; i > 0; --i) {
        acc = f(arr[i-1], acc);
    }
    return acc;
}

/**
 * @brief Reduce all states with binary operation
 */
template<uint8_t N, typename Func>
O<N> reduce(Func f) {
    O<N> acc(0);
    for (uint8_t i = 1; i < N; ++i) {
        acc = f(acc, O<N>(i));
    }
    return acc;
}

// ═══════════════════════════════════════════════════════════════════════════
// ZIP
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Zip two arrays with binary function
 */
template<uint8_t N, size_t Size, typename Func>
auto zip_with(const O_array<N, Size>& a, 
              const O_array<N, Size>& b, 
              Func f) -> std::array<decltype(f(O<N>(), O<N>())), Size> {
    std::array<decltype(f(O<N>(), O<N>())), Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = f(a[i], b[i]);
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// COMPOSITION
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Function composition: (f ∘ g)(x) = f(g(x))
 */
template<typename F, typename G>
auto compose(F f, G g) {
    return [f, g](auto x) { return f(g(x)); };
}

/**
 * @brief Pipe operator: x |> f = f(x)
 */
template<typename T, typename Func>
auto operator|(T value, Func f) -> decltype(f(value)) {
    return f(value);
}

// ═══════════════════════════════════════════════════════════════════════════
// PARTIAL APPLICATION
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Curry binary function
 */
template<typename Func>
auto curry(Func f) {
    return [f](auto x) {
        return [f, x](auto y) {
            return f(x, y);
        };
    };
}

// ═══════════════════════════════════════════════════════════════════════════
// MONADIC OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Optional monad for O<N>
 */
template<uint8_t N>
class Maybe {
    std::optional<O<N>> value_;
    
public:
    Maybe() : value_(std::nullopt) {}
    Maybe(O<N> val) : value_(val) {}
    
    bool has_value() const { return value_.has_value(); }
    O<N> value() const { return *value_; }
    O<N> value_or(O<N> default_val) const { 
        return value_.value_or(default_val); 
    }
    
    template<typename Func>
    auto map(Func f) -> Maybe<N> {
        if (!value_) return Maybe<N>();
        return Maybe<N>(f(*value_));
    }
    
    template<typename Func>
    auto flat_map(Func f) -> Maybe<N> {
        if (!value_) return Maybe<N>();
        return f(*value_);
    }
};

} // namespace functional
} // namespace o

#endif // O_FUNCTIONAL_HPP
