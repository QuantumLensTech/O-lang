#ifndef O_LOGIC_HPP
#define O_LOGIC_HPP

/**
 * @file logic.hpp
 * @brief Multi-State Logic Operations
 * 
 * Implements various logic systems for N-state types:
 * - Łukasiewicz logic (Min/Max)
 * - Product logic
 * - Gödel logic
 * - Custom logic tables
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <algorithm>
#include <cmath>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// ŁUKASIEWICZ LOGIC (MIN/MAX) - DEFAULT
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief AND operator (minimum)
 * 
 * Generalizes Boolean AND to N states using minimum.
 * This is the default logic for O<N>.
 */
template<uint8_t N>
constexpr O<N> operator&(O<N> a, O<N> b) noexcept {
    return O<N>(std::min(a.value(), b.value()));
}

/**
 * @brief OR operator (maximum)
 * 
 * Generalizes Boolean OR to N states using maximum.
 */
template<uint8_t N>
constexpr O<N> operator|(O<N> a, O<N> b) noexcept {
    return O<N>(std::max(a.value(), b.value()));
}

/**
 * @brief NOT operator (complement)
 * 
 * Generalizes Boolean NOT to N states.
 * NOT(x) = (N-1) - x
 */
template<uint8_t N>
constexpr O<N> operator~(O<N> a) noexcept {
    return O<N>(N - 1 - a.value());
}

/**
 * @brief XOR operator (cyclic addition)
 * 
 * XOR(a, b) = (a + b) % N
 */
template<uint8_t N>
constexpr O<N> operator^(O<N> a, O<N> b) noexcept {
    return O<N>((a.value() + b.value()) % N);
}

// ═══════════════════════════════════════════════════════════════════════════
// COMPOUND LOGICAL OPERATORS
// ═══════════════════════════════════════════════════════════════════════════

template<uint8_t N>
constexpr O<N>& operator&=(O<N>& a, O<N> b) noexcept {
    a = a & b;
    return a;
}

template<uint8_t N>
constexpr O<N>& operator|=(O<N>& a, O<N> b) noexcept {
    a = a | b;
    return a;
}

template<uint8_t N>
constexpr O<N>& operator^=(O<N>& a, O<N> b) noexcept {
    a = a ^ b;
    return a;
}

// ═══════════════════════════════════════════════════════════════════════════
// ŁUKASIEWICZ IMPLICATION
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Łukasiewicz implication
 * 
 * IMP(a, b) = min(N-1, N-1 - a + b)
 * 
 * Generalizes Boolean implication (a → b) to N states.
 */
template<uint8_t N>
constexpr O<N> implies(O<N> a, O<N> b) noexcept {
    uint16_t result = static_cast<uint16_t>(N - 1) - a.value() + b.value();
    return O<N>(std::min(result, static_cast<uint16_t>(N - 1)));
}

/**
 * @brief Łukasiewicz equivalence
 * 
 * EQ(a, b) = 1 - |a - b|/(N-1)
 */
template<uint8_t N>
constexpr O<N> equivalent(O<N> a, O<N> b) noexcept {
    uint8_t diff = (a.value() > b.value()) ? (a.value() - b.value()) : (b.value() - a.value());
    return O<N>(N - 1 - diff);
}

// ═══════════════════════════════════════════════════════════════════════════
// PRODUCT LOGIC
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Product logic AND
 * 
 * AND_prod(a, b) = (a * b) / (N-1)
 * 
 * Alternative conjunction operator using multiplication.
 */
template<uint8_t N>
constexpr O<N> and_product(O<N> a, O<N> b) noexcept {
    return O<N>((a.value() * b.value()) / (N - 1));
}

/**
 * @brief Product logic OR
 * 
 * OR_prod(a, b) = a + b - (a * b)/(N-1)
 */
template<uint8_t N>
constexpr O<N> or_product(O<N> a, O<N> b) noexcept {
    uint16_t product = (a.value() * b.value()) / (N - 1);
    uint16_t sum = a.value() + b.value();
    return O<N>(std::min(sum - product, static_cast<uint16_t>(N - 1)));
}

// ═══════════════════════════════════════════════════════════════════════════
// GÖDEL LOGIC
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Gödel negation
 * 
 * NOT_godel(x) = N-1 if x == 0, else 0
 * 
 * Binary-like negation in N-state space.
 */
template<uint8_t N>
constexpr O<N> not_godel(O<N> a) noexcept {
    return O<N>(a.value() == 0 ? N - 1 : 0);
}

/**
 * @brief Gödel implication
 * 
 * IMP_godel(a, b) = N-1 if a <= b, else b
 */
template<uint8_t N>
constexpr O<N> implies_godel(O<N> a, O<N> b) noexcept {
    return O<N>(a.value() <= b.value() ? N - 1 : b.value());
}

// ═══════════════════════════════════════════════════════════════════════════
// ARRAY OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Apply AND to entire arrays element-wise
 */
template<uint8_t N, size_t Size>
constexpr O_array<N, Size> operator&(const O_array<N, Size>& a, const O_array<N, Size>& b) noexcept {
    O_array<N, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = a[i] & b[i];
    }
    return result;
}

/**
 * @brief Apply OR to entire arrays element-wise
 */
template<uint8_t N, size_t Size>
constexpr O_array<N, Size> operator|(const O_array<N, Size>& a, const O_array<N, Size>& b) noexcept {
    O_array<N, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = a[i] | b[i];
    }
    return result;
}

/**
 * @brief Apply NOT to entire array element-wise
 */
template<uint8_t N, size_t Size>
constexpr O_array<N, Size> operator~(const O_array<N, Size>& a) noexcept {
    O_array<N, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = ~a[i];
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Threshold function
 * 
 * Returns N-1 if value >= threshold, else 0
 */
template<uint8_t N>
constexpr O<N> threshold(O<N> value, O<N> threshold_val) noexcept {
    return O<N>(value.value() >= threshold_val.value() ? N - 1 : 0);
}

/**
 * @brief Clamp value between min and max
 */
template<uint8_t N>
constexpr O<N> clamp(O<N> value, O<N> min, O<N> max) noexcept {
    return O<N>(std::clamp(value.value(), min.value(), max.value()));
}

/**
 * @brief Linear interpolation
 * 
 * lerp(a, b, t) = a + t*(b-a) where t ∈ [0, N-1]
 */
template<uint8_t N>
constexpr O<N> lerp(O<N> a, O<N> b, O<N> t) noexcept {
    uint16_t range = (b.value() > a.value()) ? (b.value() - a.value()) : 0;
    uint16_t result = a.value() + (range * t.value()) / (N - 1);
    return O<N>(std::min(result, static_cast<uint16_t>(N - 1)));
}

} // namespace o

#endif // O_LOGIC_HPP
