#ifndef O_CORE_HPP
#define O_CORE_HPP

/**
 * @file core.hpp
 * @brief O Language Core - Universal Multi-State Type
 * 
 * Defines the fundamental O<N> type for N-state computing.
 * This is the foundation of the entire O language.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include <cstdint>
#include <type_traits>
#include <limits>
#include <stdexcept>
#include <array>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// CORE TYPE: O<N> - Universal Multi-State Value
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Universal multi-state type
 * 
 * O<N> represents a value that can take one of N discrete states,
 * where N can be any positive integer from 2 to 256.
 * 
 * Examples:
 * - O<2>   : Binary (classical computing)
 * - O<3>   : Ternary (balanced ternary processors)
 * - O<8>   : Octovalent (3ODS, 3-qubit quantum)
 * - O<16>  : Hexadecimal
 * - O<256> : Full byte range
 * 
 * @tparam N Number of states (2-256)
 */
template<uint8_t N>
class O {
    static_assert(N >= 2 && N <= 256, "N must be between 2 and 256");
    
private:
    uint8_t value_;
    
public:
    // ═══════════════════════════════════════════════════════════════════════
    // CONSTRUCTORS
    // ═══════════════════════════════════════════════════════════════════════
    
    /**
     * @brief Default constructor - initializes to state 0
     */
    constexpr O() noexcept : value_(0) {}
    
    /**
     * @brief Construct from integer value
     * @param val Initial value (must be in range [0, N-1])
     * @throws std::out_of_range if val >= N
     */
    explicit constexpr O(uint8_t val) : value_(val) {
        if (val >= N) {
            throw std::out_of_range("O<N> value out of range");
        }
    }
    
    /**
     * @brief Copy constructor
     */
    constexpr O(const O&) noexcept = default;
    
    /**
     * @brief Move constructor
     */
    constexpr O(O&&) noexcept = default;
    
    // ═══════════════════════════════════════════════════════════════════════
    // ASSIGNMENT
    // ═══════════════════════════════════════════════════════════════════════
    
    constexpr O& operator=(const O&) noexcept = default;
    constexpr O& operator=(O&&) noexcept = default;
    
    constexpr O& operator=(uint8_t val) {
        if (val >= N) {
            throw std::out_of_range("O<N> value out of range");
        }
        value_ = val;
        return *this;
    }
    
    // ═══════════════════════════════════════════════════════════════════════
    // ACCESSORS
    // ═══════════════════════════════════════════════════════════════════════
    
    /**
     * @brief Get the current value
     * @return Current state value (0 to N-1)
     */
    constexpr uint8_t value() const noexcept { return value_; }
    
    /**
     * @brief Get the number of possible states
     * @return N
     */
    static constexpr uint8_t num_states() noexcept { return N; }
    
    /**
     * @brief Check if this is the minimum value (state 0)
     */
    constexpr bool is_min() const noexcept { return value_ == 0; }
    
    /**
     * @brief Check if this is the maximum value (state N-1)
     */
    constexpr bool is_max() const noexcept { return value_ == N - 1; }
    
    // ═══════════════════════════════════════════════════════════════════════
    // STATIC FACTORY METHODS
    // ═══════════════════════════════════════════════════════════════════════
    
    static constexpr O min_value() noexcept { return O(0); }
    static constexpr O max_value() noexcept { return O(N - 1); }
    
    /**
     * @brief Create O<N> from normalized float [0.0, 1.0]
     */
    static constexpr O from_normalized(float f) noexcept {
        if (f <= 0.0f) return min_value();
        if (f >= 1.0f) return max_value();
        return O(static_cast<uint8_t>(f * (N - 1) + 0.5f));
    }
    
    /**
     * @brief Convert to normalized float [0.0, 1.0]
     */
    constexpr float to_normalized() const noexcept {
        return static_cast<float>(value_) / static_cast<float>(N - 1);
    }
    
    // ═══════════════════════════════════════════════════════════════════════
    // COMPARISON OPERATORS
    // ═══════════════════════════════════════════════════════════════════════
    
    constexpr bool operator==(const O& other) const noexcept {
        return value_ == other.value_;
    }
    
    constexpr bool operator!=(const O& other) const noexcept {
        return value_ != other.value_;
    }
    
    constexpr bool operator<(const O& other) const noexcept {
        return value_ < other.value_;
    }
    
    constexpr bool operator<=(const O& other) const noexcept {
        return value_ <= other.value_;
    }
    
    constexpr bool operator>(const O& other) const noexcept {
        return value_ > other.value_;
    }
    
    constexpr bool operator>=(const O& other) const noexcept {
        return value_ >= other.value_;
    }
    
    // ═══════════════════════════════════════════════════════════════════════
    // ARITHMETIC OPERATORS (modulo N)
    // ═══════════════════════════════════════════════════════════════════════
    
    constexpr O operator+(const O& other) const noexcept {
        return O((value_ + other.value_) % N);
    }
    
    constexpr O operator-(const O& other) const noexcept {
        return O((value_ + N - other.value_) % N);
    }
    
    constexpr O operator*(const O& other) const noexcept {
        return O((value_ * other.value_) % N);
    }
    
    constexpr O& operator+=(const O& other) noexcept {
        value_ = (value_ + other.value_) % N;
        return *this;
    }
    
    constexpr O& operator-=(const O& other) noexcept {
        value_ = (value_ + N - other.value_) % N;
        return *this;
    }
    
    constexpr O& operator*=(const O& other) noexcept {
        value_ = (value_ * other.value_) % N;
        return *this;
    }
    
    // ═══════════════════════════════════════════════════════════════════════
    // INCREMENT/DECREMENT (cyclic)
    // ═══════════════════════════════════════════════════════════════════════
    
    constexpr O& operator++() noexcept {
        value_ = (value_ + 1) % N;
        return *this;
    }
    
    constexpr O operator++(int) noexcept {
        O temp = *this;
        ++(*this);
        return temp;
    }
    
    constexpr O& operator--() noexcept {
        value_ = (value_ + N - 1) % N;
        return *this;
    }
    
    constexpr O operator--(int) noexcept {
        O temp = *this;
        --(*this);
        return temp;
    }
    
    // ═══════════════════════════════════════════════════════════════════════
    // CONVERSION
    // ═══════════════════════════════════════════════════════════════════════
    
    explicit constexpr operator uint8_t() const noexcept { return value_; }
    explicit constexpr operator int() const noexcept { return static_cast<int>(value_); }
    explicit constexpr operator bool() const noexcept { return value_ != 0; }
};

// ═══════════════════════════════════════════════════════════════════════════
// TYPE ALIASES FOR COMMON CASES
// ═══════════════════════════════════════════════════════════════════════════

using O2 = O<2>;      ///< Binary
using O3 = O<3>;      ///< Ternary
using O4 = O<4>;      ///< Quaternary
using O8 = O<8>;      ///< Octovalent (3ODS)
using O16 = O<16>;    ///< Hexadecimal
using O32 = O<32>;    ///< 32-state
using O64 = O<64>;    ///< 64-state
using O256 = O<256>;  ///< Full byte

// Legacy alias for compatibility
template<uint8_t N>
using octo = O<N>;

// ═══════════════════════════════════════════════════════════════════════════
// ARRAY TYPE FOR MULTI-STATE REGISTERS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Array of O<N> values (register)
 * 
 * Represents a register of Size values, each with N states.
 * Useful for representing quantum registers, memory words, etc.
 */
template<uint8_t N, size_t Size>
using O_array = std::array<O<N>, Size>;

// Common register sizes
template<uint8_t N>
using O_reg8 = O_array<N, 8>;

template<uint8_t N>
using O_reg16 = O_array<N, 16>;

template<uint8_t N>
using O_reg32 = O_array<N, 32>;

template<uint8_t N>
using O_reg64 = O_array<N, 64>;

} // namespace o

// ═══════════════════════════════════════════════════════════════════════════
// STD::NUMERIC_LIMITS SPECIALIZATION
// ═══════════════════════════════════════════════════════════════════════════

namespace std {
    template<uint8_t N>
    struct numeric_limits<o::O<N>> {
        static constexpr bool is_specialized = true;
        static constexpr o::O<N> min() noexcept { return o::O<N>::min_value(); }
        static constexpr o::O<N> max() noexcept { return o::O<N>::max_value(); }
        static constexpr o::O<N> lowest() noexcept { return o::O<N>::min_value(); }
        static constexpr int digits = 0; // Not applicable
        static constexpr bool is_signed = false;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr bool is_modulo = true;
    };
}

#endif // O_CORE_HPP
