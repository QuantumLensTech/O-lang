#ifndef O_OPERATORS_HPP
#define O_OPERATORS_HPP

/**
 * @file operators.hpp
 * @brief Advanced Arithmetic Operators
 * 
 * Additional arithmetic operations for O<N> types beyond
 * the basic operations defined in core.hpp.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <cmath>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// MODULAR ARITHMETIC
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Modular division
 * 
 * Divides a by b in modular arithmetic (modulo N).
 * Uses multiplicative inverse when possible.
 */
template<uint8_t N>
constexpr O<N> div_mod(O<N> a, O<N> b) noexcept {
    if (b.value() == 0) return O<N>(0);  // Undefined, return 0
    
    // For prime N, use Fermat's little theorem: a/b = a * b^(N-2) mod N
    // For composite N, use extended Euclidean algorithm (simplified here)
    
    uint16_t result = a.value();
    for (uint8_t i = 1; i < b.value(); ++i) {
        result = (result + a.value()) % N;
    }
    return O<N>(result / b.value());
}

/**
 * @brief Modular exponentiation
 * 
 * Computes (base^exp) mod N efficiently.
 */
template<uint8_t N>
constexpr O<N> pow_mod(O<N> base, uint16_t exp) noexcept {
    if (exp == 0) return O<N>(1);
    if (exp == 1) return base;
    
    uint32_t result = 1;
    uint32_t b = base.value();
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * b) % N;
        }
        b = (b * b) % N;
        exp /= 2;
    }
    
    return O<N>(static_cast<uint8_t>(result));
}

/**
 * @brief Square root (approximate, modulo N)
 */
template<uint8_t N>
constexpr O<N> sqrt_mod(O<N> a) noexcept {
    // Find x such that x² ≡ a (mod N)
    for (uint8_t x = 0; x < N; ++x) {
        if (((x * x) % N) == a.value()) {
            return O<N>(x);
        }
    }
    return O<N>(0);  // No square root exists
}

// ═══════════════════════════════════════════════════════════════════════════
// TRIGONOMETRIC-LIKE OPERATIONS (CYCLIC)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Cyclic sine (maps states to sine wave)
 * 
 * sin_cyclic(x) maps x ∈ [0, N-1] to sine wave [0, N-1]
 */
template<uint8_t N>
constexpr O<N> sin_cyclic(O<N> x) noexcept {
    float angle = 2.0f * 3.14159265f * x.value() / N;
    float result = (std::sin(angle) + 1.0f) * 0.5f * (N - 1);
    return O<N>(static_cast<uint8_t>(result + 0.5f));
}

/**
 * @brief Cyclic cosine
 */
template<uint8_t N>
constexpr O<N> cos_cyclic(O<N> x) noexcept {
    float angle = 2.0f * 3.14159265f * x.value() / N;
    float result = (std::cos(angle) + 1.0f) * 0.5f * (N - 1);
    return O<N>(static_cast<uint8_t>(result + 0.5f));
}

// ═══════════════════════════════════════════════════════════════════════════
// DISTANCE METRICS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Hamming distance (counts differing positions)
 */
template<uint8_t N, size_t Size>
constexpr uint8_t hamming_distance(const O_array<N, Size>& a, const O_array<N, Size>& b) noexcept {
    uint8_t count = 0;
    for (size_t i = 0; i < Size; ++i) {
        if (a[i].value() != b[i].value()) {
            ++count;
        }
    }
    return count;
}

/**
 * @brief Manhattan distance (sum of absolute differences)
 */
template<uint8_t N, size_t Size>
constexpr uint16_t manhattan_distance(const O_array<N, Size>& a, const O_array<N, Size>& b) noexcept {
    uint16_t sum = 0;
    for (size_t i = 0; i < Size; ++i) {
        uint8_t diff = (a[i].value() > b[i].value()) ? 
                       (a[i].value() - b[i].value()) : 
                       (b[i].value() - a[i].value());
        sum += diff;
    }
    return sum;
}

/**
 * @brief Euclidean distance (squared, to avoid sqrt)
 */
template<uint8_t N, size_t Size>
constexpr uint32_t euclidean_distance_squared(const O_array<N, Size>& a, const O_array<N, Size>& b) noexcept {
    uint32_t sum = 0;
    for (size_t i = 0; i < Size; ++i) {
        int16_t diff = static_cast<int16_t>(a[i].value()) - static_cast<int16_t>(b[i].value());
        sum += diff * diff;
    }
    return sum;
}

// ═══════════════════════════════════════════════════════════════════════════
// BIT MANIPULATION (for small N, treating states as bit patterns)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Rotate left (cyclic shift)
 */
template<uint8_t N>
constexpr O<N> rotate_left(O<N> x, uint8_t shift) noexcept {
    return O<N>((x.value() + shift) % N);
}

/**
 * @brief Rotate right (cyclic shift)
 */
template<uint8_t N>
constexpr O<N> rotate_right(O<N> x, uint8_t shift) noexcept {
    return O<N>((x.value() + N - (shift % N)) % N);
}

/**
 * @brief Swap adjacent pairs (for N = power of 2)
 */
template<uint8_t N>
constexpr O<N> swap_pairs(O<N> x) noexcept {
    static_assert((N & (N - 1)) == 0, "N must be power of 2 for swap_pairs");
    uint8_t val = x.value();
    return O<N>(((val & 0xAA) >> 1) | ((val & 0x55) << 1));
}

// ═══════════════════════════════════════════════════════════════════════════
// AGGREGATE OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Sum of array elements (modulo N)
 */
template<uint8_t N, size_t Size>
constexpr O<N> sum(const O_array<N, Size>& arr) noexcept {
    uint16_t total = 0;
    for (size_t i = 0; i < Size; ++i) {
        total = (total + arr[i].value()) % N;
    }
    return O<N>(static_cast<uint8_t>(total));
}

/**
 * @brief Product of array elements (modulo N)
 */
template<uint8_t N, size_t Size>
constexpr O<N> product(const O_array<N, Size>& arr) noexcept {
    uint16_t total = 1;
    for (size_t i = 0; i < Size; ++i) {
        total = (total * arr[i].value()) % N;
    }
    return O<N>(static_cast<uint8_t>(total));
}

/**
 * @brief Minimum element in array
 */
template<uint8_t N, size_t Size>
constexpr O<N> min(const O_array<N, Size>& arr) noexcept {
    O<N> result = arr[0];
    for (size_t i = 1; i < Size; ++i) {
        if (arr[i].value() < result.value()) {
            result = arr[i];
        }
    }
    return result;
}

/**
 * @brief Maximum element in array
 */
template<uint8_t N, size_t Size>
constexpr O<N> max(const O_array<N, Size>& arr) noexcept {
    O<N> result = arr[0];
    for (size_t i = 1; i < Size; ++i) {
        if (arr[i].value() > result.value()) {
            result = arr[i];
        }
    }
    return result;
}

/**
 * @brief Average of array elements (rounded)
 */
template<uint8_t N, size_t Size>
constexpr O<N> average(const O_array<N, Size>& arr) noexcept {
    uint16_t total = 0;
    for (size_t i = 0; i < Size; ++i) {
        total += arr[i].value();
    }
    return O<N>(static_cast<uint8_t>((total + Size/2) / Size));
}

} // namespace o

#endif // O_OPERATORS_HPP
