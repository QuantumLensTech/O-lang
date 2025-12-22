#ifndef O_CORE_HPP
#define O_CORE_HPP

/**
 * O Language - Core Types
 * 
 * Fundamental multi-state types for N-state computing.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include <cstdint>
#include <array>
#include <stdexcept>
#include <type_traits>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// FUNDAMENTAL TYPE: octo<N>
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief N-state fundamental type
 * 
 * Represents a value with N discrete states: {0, 1, ..., N-1}
 * 
 * @tparam N Number of states (must be in [2, 256])
 * 
 * @example
 * octo<2> binary_flag = 1;       // Binary: 0 or 1
 * octo<3> ternary_trit = 2;      // Ternary: 0, 1, or 2
 * octo<8> octovalent = 5;        // Octovalent: 0-7
 */
template<uint8_t N>
class octo {
    static_assert(N >= 2 && N <= 256, "N must be in [2, 256]");
    
private:
    uint8_t value_;  // Invariant: value_ ∈ [0, N-1]
    
public:
    // ───────────────────────────────────────────────────────────────────────
    // Type Properties
    // ───────────────────────────────────────────────────────────────────────
    
    static constexpr uint8_t num_states = N;
    
    // ───────────────────────────────────────────────────────────────────────
    // Constructors
    // ───────────────────────────────────────────────────────────────────────
    
    /**
     * @brief Default constructor (initializes to state 0)
     */
    constexpr octo() noexcept : value_(0) {}
    
    /**
     * @brief Explicit constructor from integer
     * @param v Value (will be taken modulo N)
     */
    constexpr explicit octo(uint8_t v) noexcept : value_(v % N) {}
    
    /**
     * @brief Copy constructor
     */
    constexpr octo(const octo& other) noexcept = default;
    
    // ───────────────────────────────────────────────────────────────────────
    // Accessors
    // ───────────────────────────────────────────────────────────────────────
    
    /**
     * @brief Get underlying value
     * @return Value in [0, N-1]
     */
    constexpr uint8_t value() const noexcept { return value_; }
    
    /**
     * @brief Implicit conversion to uint8_t
     */
    constexpr operator uint8_t() const noexcept { return value_; }
    
    /**
     * @brief Check if value is valid (always true due to invariant)
     */
    constexpr bool is_valid() const noexcept { 
        return value_ < N; 
    }
    
    // ───────────────────────────────────────────────────────────────────────
    // Assignment
    // ───────────────────────────────────────────────────────────────────────
    
    /**
     * @brief Copy assignment
     */
    constexpr octo& operator=(const octo& other) noexcept = default;
    
    /**
     * @brief Assign from integer (modulo N)
     */
    constexpr octo& operator=(uint8_t v) noexcept {
        value_ = v % N;
        return *this;
    }
    
    // ───────────────────────────────────────────────────────────────────────
    // Increment/Decrement
    // ───────────────────────────────────────────────────────────────────────
    
    /**
     * @brief Pre-increment (modular)
     */
    constexpr octo& operator++() noexcept {
        value_ = (value_ + 1) % N;
        return *this;
    }
    
    /**
     * @brief Post-increment (modular)
     */
    constexpr octo operator++(int) noexcept {
        octo temp = *this;
        ++(*this);
        return temp;
    }
    
    /**
     * @brief Pre-decrement (modular)
     */
    constexpr octo& operator--() noexcept {
        value_ = (value_ + N - 1) % N;
        return *this;
    }
    
    /**
     * @brief Post-decrement (modular)
     */
    constexpr octo operator--(int) noexcept {
        octo temp = *this;
        --(*this);
        return temp;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// COMPARISON OPERATORS
// ═══════════════════════════════════════════════════════════════════════════

template<uint8_t N>
constexpr bool operator==(octo<N> a, octo<N> b) noexcept {
    return a.value() == b.value();
}

template<uint8_t N>
constexpr bool operator!=(octo<N> a, octo<N> b) noexcept {
    return !(a == b);
}

template<uint8_t N>
constexpr bool operator<(octo<N> a, octo<N> b) noexcept {
    return a.value() < b.value();
}

template<uint8_t N>
constexpr bool operator>(octo<N> a, octo<N> b) noexcept {
    return b < a;
}

template<uint8_t N>
constexpr bool operator<=(octo<N> a, octo<N> b) noexcept {
    return !(a > b);
}

template<uint8_t N>
constexpr bool operator>=(octo<N> a, octo<N> b) noexcept {
    return !(a < b);
}

// ═══════════════════════════════════════════════════════════════════════════
// ARRAY TYPE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Fixed-size array of N-state values
 * 
 * @tparam N Number of states per element
 * @tparam M Array size
 */
template<uint8_t N, size_t M>
class octo_array {
private:
    std::array<octo<N>, M> data_;
    
public:
    // ───────────────────────────────────────────────────────────────────────
    // Constructors
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr octo_array() noexcept = default;
    
    constexpr octo_array(std::initializer_list<uint8_t> init) {
        size_t i = 0;
        for (auto val : init) {
            if (i >= M) break;
            data_[i++] = octo<N>(val);
        }
    }
    
    // ───────────────────────────────────────────────────────────────────────
    // Element Access
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr octo<N>& operator[](size_t i) noexcept {
        return data_[i];
    }
    
    constexpr const octo<N>& operator[](size_t i) const noexcept {
        return data_[i];
    }
    
    constexpr octo<N>& at(size_t i) {
        if (i >= M) throw std::out_of_range("octo_array index out of range");
        return data_[i];
    }
    
    constexpr const octo<N>& at(size_t i) const {
        if (i >= M) throw std::out_of_range("octo_array index out of range");
        return data_[i];
    }
    
    // ───────────────────────────────────────────────────────────────────────
    // Size & Capacity
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr size_t size() const noexcept { return M; }
    constexpr bool empty() const noexcept { return M == 0; }
    
    // ───────────────────────────────────────────────────────────────────────
    // Iterators
    // ───────────────────────────────────────────────────────────────────────
    
    auto begin() noexcept { return data_.begin(); }
    auto end() noexcept { return data_.end(); }
    auto begin() const noexcept { return data_.begin(); }
    auto end() const noexcept { return data_.end(); }
    
    // ───────────────────────────────────────────────────────────────────────
    // Fill
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr void fill(octo<N> value) noexcept {
        for (auto& elem : data_) {
            elem = value;
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// MATRIX TYPE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Matrix of N-state values
 * 
 * @tparam N Number of states per element
 * @tparam R Number of rows
 * @tparam C Number of columns
 */
template<uint8_t N, size_t R, size_t C>
class octo_matrix {
private:
    std::array<octo<N>, R * C> data_;  // Row-major storage
    
public:
    // ───────────────────────────────────────────────────────────────────────
    // Constructors
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr octo_matrix() noexcept = default;
    
    // ───────────────────────────────────────────────────────────────────────
    // Element Access
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr octo<N>& operator()(size_t i, size_t j) noexcept {
        return data_[i * C + j];
    }
    
    constexpr const octo<N>& operator()(size_t i, size_t j) const noexcept {
        return data_[i * C + j];
    }
    
    constexpr octo<N>& at(size_t i, size_t j) {
        if (i >= R || j >= C) {
            throw std::out_of_range("octo_matrix index out of range");
        }
        return data_[i * C + j];
    }
    
    constexpr const octo<N>& at(size_t i, size_t j) const {
        if (i >= R || j >= C) {
            throw std::out_of_range("octo_matrix index out of range");
        }
        return data_[i * C + j];
    }
    
    // ───────────────────────────────────────────────────────────────────────
    // Size
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr size_t rows() const noexcept { return R; }
    constexpr size_t cols() const noexcept { return C; }
    constexpr size_t size() const noexcept { return R * C; }
    
    // ───────────────────────────────────────────────────────────────────────
    // Fill
    // ───────────────────────────────────────────────────────────────────────
    
    constexpr void fill(octo<N> value) noexcept {
        for (auto& elem : data_) {
            elem = value;
        }
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// TYPE TRAITS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Check if type is octo<N>
 */
template<typename T>
struct is_octo : std::false_type {};

template<uint8_t N>
struct is_octo<octo<N>> : std::true_type {};

template<typename T>
inline constexpr bool is_octo_v = is_octo<T>::value;

/**
 * @brief Extract N from octo<N>
 */
template<typename T>
struct octo_num_states;

template<uint8_t N>
struct octo_num_states<octo<N>> {
    static constexpr uint8_t value = N;
};

template<typename T>
inline constexpr uint8_t octo_num_states_v = octo_num_states<T>::value;

// ═══════════════════════════════════════════════════════════════════════════
// UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Check if all values are equal
 */
template<uint8_t N, typename... Args>
constexpr bool octo_all_equal(octo<N> first, Args... rest) {
    return ((first == rest) && ...);
}

/**
 * @brief Check if any value equals target
 */
template<uint8_t N>
constexpr bool octo_any_equal(octo<N> target, 
                               std::initializer_list<octo<N>> values) {
    for (auto v : values) {
        if (v == target) return true;
    }
    return false;
}

/**
 * @brief Minimum of two octo values
 */
template<uint8_t N>
constexpr octo<N> octo_min(octo<N> a, octo<N> b) noexcept {
    return (a < b) ? a : b;
}

/**
 * @brief Maximum of two octo values
 */
template<uint8_t N>
constexpr octo<N> octo_max(octo<N> a, octo<N> b) noexcept {
    return (a > b) ? a : b;
}

} // namespace o

#endif // O_CORE_HPP
