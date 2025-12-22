#ifndef O_OPERATORS_HPP
#define O_OPERATORS_HPP

/**
 * O Language - Arithmetic and Logical Operators
 * 
 * Multi-state operations for N-state computing.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <algorithm>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// ARITHMETIC OPERATORS (Modular Arithmetic)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Modular addition
 */
template<uint8_t N>
constexpr octo<N> operator+(octo<N> a, octo<N> b) noexcept {
    return octo<N>((a.value() + b.value()) % N);
}

/**
 * @brief Modular subtraction
 */
template<uint8_t N>
constexpr octo<N> operator-(octo<N> a, octo<N> b) noexcept {
    return octo<N>((a.value() + N - b.value()) % N);
}

/**
 * @brief Modular multiplication
 */
template<uint8_t N>
constexpr octo<N> operator*(octo<N> a, octo<N> b) noexcept {
    return octo<N>((a.value() * b.value()) % N);
}

/**
 * @brief Modular division (when N is prime, multiplicative inverse exists)
 */
template<uint8_t N>
constexpr octo<N> operator/(octo<N> a, octo<N> b) {
    if (b.value() == 0) {
        throw std::domain_error("Division by zero in octo<N>");
    }
    // Simplified: just divide values (proper would compute modular inverse)
    return octo<N>(a.value() / b.value());
}

/**
 * @brief Modulo operation
 */
template<uint8_t N>
constexpr octo<N> operator%(octo<N> a, octo<N> b) {
    if (b.value() == 0) {
        throw std::domain_error("Modulo by zero in octo<N>");
    }
    return octo<N>(a.value() % b.value());
}

// ═══════════════════════════════════════════════════════════════════════════
// COMPOUND ASSIGNMENT OPERATORS
// ═══════════════════════════════════════════════════════════════════════════

template<uint8_t N>
constexpr octo<N>& operator+=(octo<N>& a, octo<N> b) noexcept {
    a = a + b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator-=(octo<N>& a, octo<N> b) noexcept {
    a = a - b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator*=(octo<N>& a, octo<N> b) noexcept {
    a = a * b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator/=(octo<N>& a, octo<N> b) {
    a = a / b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator%=(octo<N>& a, octo<N> b) {
    a = a % b;
    return a;
}

// ═══════════════════════════════════════════════════════════════════════════
// LOGICAL OPERATORS (N-valued Logic)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief N-valued AND (minimum)
 * 
 * Generalizes Boolean AND to N states.
 * Truth table: returns minimum of two values.
 */
template<uint8_t N>
constexpr octo<N> operator&(octo<N> a, octo<N> b) noexcept {
    return octo<N>(std::min(a.value(), b.value()));
}

/**
 * @brief N-valued OR (maximum)
 * 
 * Generalizes Boolean OR to N states.
 * Truth table: returns maximum of two values.
 */
template<uint8_t N>
constexpr octo<N> operator|(octo<N> a, octo<N> b) noexcept {
    return octo<N>(std::max(a.value(), b.value()));
}

/**
 * @brief N-valued NOT (complement)
 * 
 * Generalizes Boolean NOT to N states.
 * NOT(x) = (N-1) - x
 */
template<uint8_t N>
constexpr octo<N> operator~(octo<N> a) noexcept {
    return octo<N>(N - 1 - a.value());
}

/**
 * @brief N-valued XOR
 * 
 * XOR(a, b) = (a + b) % N
 */
template<uint8_t N>
constexpr octo<N> operator^(octo<N> a, octo<N> b) noexcept {
    return octo<N>((a.value() + b.value()) % N);
}

// ═══════════════════════════════════════════════════════════════════════════
// COMPOUND LOGICAL OPERATORS
// ═══════════════════════════════════════════════════════════════════════════

template<uint8_t N>
constexpr octo<N>& operator&=(octo<N>& a, octo<N> b) noexcept {
    a = a & b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator|=(octo<N>& a, octo<N> b) noexcept {
    a = a | b;
    return a;
}

template<uint8_t N>
constexpr octo<N>& operator^=(octo<N>& a, octo<N> b) noexcept {
    a = a ^ b;
    return a;
}

// ═══════════════════════════════════════════════════════════════════════════
// ADVANCED LOGICAL OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Łukasiewicz implication
 * 
 * IMP(a, b) = min(N-1, N-1 - a + b)
 */
template<uint8_t N>
constexpr octo<N> octo_implies(octo<N> a, octo<N> b) noexcept {
    uint16_t result = static_cast<uint16_t>(N - 1) - a.value() + b.value();
    return octo<N>(std::min(result, static_cast<uint16_t>(N - 1)));
}

/**
 * @brief Product logic conjunction
 * 
 * AND_prod(a, b) = (a * b) / (N-1)
 */
template<uint8_t N>
constexpr octo<N> octo_and_product(octo<N> a, octo<N> b) noexcept {
    return octo<N>((a.value() * b.value()) / (N - 1));
}

/**
 * @brief Gödel negation
 * 
 * NOT_godel(x) = 0 if x > 0, else N-1
 */
template<uint8_t N>
constexpr octo<N> octo_not_godel(octo<N> a) noexcept {
    return octo<N>(a.value() > 0 ? 0 : N - 1);
}

} // namespace o

#endif // O_OPERATORS_HPP
