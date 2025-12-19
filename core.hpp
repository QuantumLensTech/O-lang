// O Language — Universal Multi-State Extension of C++
// core.hpp — Main header file
// 
// Copyright (C) 2025 Jean-Christophe Ané
// License: CC BY-NC-SA 4.0
//
// Version: 1.0.0-alpha
// Requires: C++17 or later

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <stdexcept>
#include <algorithm>
#include <limits>

namespace o {

// ============================================================================
// VERSION INFORMATION
// ============================================================================

constexpr int VERSION_MAJOR = 1;
constexpr int VERSION_MINOR = 0;
constexpr int VERSION_PATCH = 0;
constexpr const char* VERSION_STRING = "1.0.0-alpha";

// ============================================================================
// TYPE TRAITS AND CONCEPTS
// ============================================================================

#if __cplusplus >= 202002L
// C++20 concepts
template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept OStateLike = requires(T a) {
    { a.value } -> std::convertible_to<uint64_t>;
    { T::num_states } -> std::convertible_to<size_t>;
    { T::min_value() } -> std::same_as<T>;
    { T::max_value() } -> std::same_as<T>;
};
#else
// C++17 fallback
template<typename T, typename = void>
struct is_o_state : std::false_type {};

template<typename T>
struct is_o_state<T, std::void_t<
    decltype(std::declval<T>().value),
    decltype(T::num_states)
>> : std::true_type {};

template<typename T>
constexpr bool is_o_state_v = is_o_state<T>::value;
#endif

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================

template<size_t N> class O_state;
template<size_t N> class O_channel;
template<size_t N, typename Logic> class O_logic_adapter;

// ============================================================================
// O_STATE<N> — FUNDAMENTAL N-STATE TYPE
// ============================================================================

template<size_t N>
class O_state {
    static_assert(N > 0, "O_state: N must be positive");
    static_assert(N <= UINT64_MAX, "O_state: N too large for uint64_t");
    
public:
    // ========================================================================
    // TYPE DEFINITIONS
    // ========================================================================
    
    using value_type = uint64_t;
    using size_type = size_t;
    
    static constexpr size_type num_states = N;
    
    // ========================================================================
    // CONSTRUCTORS
    // ========================================================================
    
    // Default constructor: zero state
    constexpr O_state() noexcept : value_(0) {}
    
    // Explicit constructor from value (with modulo)
    constexpr explicit O_state(value_type v) noexcept : value_(v % N) {}
    
    // Copy constructor
    constexpr O_state(const O_state&) noexcept = default;
    
    // Move constructor
    constexpr O_state(O_state&&) noexcept = default;
    
    // ========================================================================
    // ASSIGNMENT
    // ========================================================================
    
    constexpr O_state& operator=(const O_state&) noexcept = default;
    constexpr O_state& operator=(O_state&&) noexcept = default;
    
    // ========================================================================
    // ACCESSORS
    // ========================================================================
    
    constexpr value_type value() const noexcept { return value_; }
    constexpr size_type states() const noexcept { return N; }
    
    // ========================================================================
    // STATIC FACTORIES
    // ========================================================================
    
    static constexpr O_state min_value() noexcept { return O_state(0); }
    static constexpr O_state max_value() noexcept { return O_state(N - 1); }
    static constexpr O_state zero() noexcept { return O_state(0); }
    
    // ========================================================================
    // ARITHMETIC OPERATORS (MODULAR)
    // ========================================================================
    
    constexpr O_state& operator+=(const O_state& rhs) noexcept {
        value_ = (value_ + rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state& operator-=(const O_state& rhs) noexcept {
        value_ = (value_ + N - rhs.value_ % N) % N;
        return *this;
    }
    
    constexpr O_state& operator*=(const O_state& rhs) noexcept {
        value_ = (value_ * rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state& operator/=(const O_state& rhs) {
        if (rhs.value_ == 0) {
            throw std::domain_error("O_state: division by zero");
        }
        // Simplified: integer division with modulo
        value_ = (value_ / rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state& operator%=(const O_state& rhs) {
        if (rhs.value_ == 0) {
            throw std::domain_error("O_state: modulo by zero");
        }
        value_ = value_ % rhs.value_;
        return *this;
    }
    
    // ========================================================================
    // INCREMENT/DECREMENT (CYCLIC)
    // ========================================================================
    
    constexpr O_state& operator++() noexcept {
        value_ = (value_ + 1) % N;
        return *this;
    }
    
    constexpr O_state operator++(int) noexcept {
        O_state tmp = *this;
        ++(*this);
        return tmp;
    }
    
    constexpr O_state& operator--() noexcept {
        value_ = (value_ + N - 1) % N;
        return *this;
    }
    
    constexpr O_state operator--(int) noexcept {
        O_state tmp = *this;
        --(*this);
        return tmp;
    }
    
    // ========================================================================
    // UNARY OPERATORS
    // ========================================================================
    
    constexpr O_state operator+() const noexcept { return *this; }
    constexpr O_state operator-() const noexcept { return O_state(N - value_); }
    
    // ========================================================================
    // COMPARISON OPERATORS
    // ========================================================================
    
    constexpr bool operator==(const O_state& rhs) const noexcept {
        return value_ == rhs.value_;
    }
    
    constexpr bool operator!=(const O_state& rhs) const noexcept {
        return value_ != rhs.value_;
    }
    
    constexpr bool operator<(const O_state& rhs) const noexcept {
        return value_ < rhs.value_;
    }
    
    constexpr bool operator<=(const O_state& rhs) const noexcept {
        return value_ <= rhs.value_;
    }
    
    constexpr bool operator>(const O_state& rhs) const noexcept {
        return value_ > rhs.value_;
    }
    
    constexpr bool operator>=(const O_state& rhs) const noexcept {
        return value_ >= rhs.value_;
    }
    
    // ========================================================================
    // CONVERSION OPERATORS
    // ========================================================================
    
    explicit constexpr operator value_type() const noexcept { return value_; }
    explicit constexpr operator bool() const noexcept { return value_ != 0; }
    
    // ========================================================================
    // BITWISE OPERATORS (operate on underlying representation)
    // ========================================================================
    
    constexpr O_state& operator&=(const O_state& rhs) noexcept {
        value_ = (value_ & rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state& operator|=(const O_state& rhs) noexcept {
        value_ = (value_ | rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state& operator^=(const O_state& rhs) noexcept {
        value_ = (value_ ^ rhs.value_) % N;
        return *this;
    }
    
    constexpr O_state operator~() const noexcept {
        return O_state(~value_);
    }
    
    constexpr O_state& operator<<=(size_type shift) noexcept {
        value_ = (value_ << shift) % N;
        return *this;
    }
    
    constexpr O_state& operator>>=(size_type shift) noexcept {
        value_ = (value_ >> shift) % N;
        return *this;
    }

private:
    value_type value_;
};

// ============================================================================
// BINARY OPERATORS (NON-MEMBER)
// ============================================================================

template<size_t N>
constexpr O_state<N> operator+(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs += rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator-(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs -= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator*(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs *= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator/(O_state<N> lhs, const O_state<N>& rhs) {
    lhs /= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator%(O_state<N> lhs, const O_state<N>& rhs) {
    lhs %= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator&(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs &= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator|(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs |= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator^(O_state<N> lhs, const O_state<N>& rhs) noexcept {
    lhs ^= rhs;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator<<(O_state<N> lhs, size_t shift) noexcept {
    lhs <<= shift;
    return lhs;
}

template<size_t N>
constexpr O_state<N> operator>>(O_state<N> lhs, size_t shift) noexcept {
    lhs >>= shift;
    return lhs;
}

// ============================================================================
// CONVENIENT ALIASES
// ============================================================================

using O_binary = O_state<2>;    // Binary (0-1)
using O_ternary = O_state<3>;   // Ternary (0-2)
using O_quaternary = O_state<4>; // Quaternary (0-3)
using O_quinary = O_state<5>;   // Quinary (0-4)
using O_senary = O_state<6>;    // Senary (0-5)
using O_septenary = O_state<7>; // Septenary (0-6)
using O_octal = O_state<8>;     // Octal (0-7)
using O_nonary = O_state<9>;    // Nonary (0-8)
using O_decimal = O_state<10>;  // Decimal (0-9)
using O_duodec = O_state<12>;   // Duodecimal (0-11)
using O_hex = O_state<16>;      // Hexadecimal (0-15)
using O8 = O_state<256>;        // 8-bit (0-255)
using O16 = O_state<65536>;     // 16-bit (0-65535)

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Get number of states
template<size_t N>
constexpr size_t num_states() noexcept {
    return N;
}

// Min/max values
template<size_t N>
constexpr O_state<N> min_state() noexcept {
    return O_state<N>::min_value();
}

template<size_t N>
constexpr O_state<N> max_state() noexcept {
    return O_state<N>::max_value();
}

// ============================================================================
// STANDARD LIBRARY SPECIALIZATIONS
// ============================================================================

} // namespace o

// std::hash specialization
namespace std {
    template<size_t N>
    struct hash<o::O_state<N>> {
        size_t operator()(const o::O_state<N>& s) const noexcept {
            return std::hash<uint64_t>{}(s.value());
        }
    };
    
    // std::numeric_limits specialization
    template<size_t N>
    struct numeric_limits<o::O_state<N>> {
        static constexpr bool is_specialized = true;
        static constexpr o::O_state<N> min() noexcept { return o::O_state<N>::min_value(); }
        static constexpr o::O_state<N> max() noexcept { return o::O_state<N>::max_value(); }
        static constexpr o::O_state<N> lowest() noexcept { return o::O_state<N>::min_value(); }
        static constexpr int digits = 0; // Not applicable
        static constexpr bool is_signed = false;
        static constexpr bool is_integer = true;
        static constexpr bool is_exact = true;
        static constexpr bool is_modulo = true;
    };
}

// ============================================================================
// INCLUDE SUB-MODULES
// ============================================================================

#include "o/logic.hpp"
#include "o/channel.hpp"
#include "o/runtime.hpp"
#include "o/compose.hpp"

// End of core.hpp
