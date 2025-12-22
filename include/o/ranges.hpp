#ifndef O_RANGES_HPP
#define O_RANGES_HPP

/**
 * @file ranges.hpp
 * @brief Range Utilities for O<N> Types
 * 
 * Provides iteration and range-based operations for N-state systems.
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <iterator>
#include <vector>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// RANGE ITERATOR
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Iterator for range of N states
 * 
 * Allows iterating through all possible states of O<N>.
 */
template<uint8_t N>
class O_range_iterator {
    uint8_t current_;
    
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = O<N>;
    using difference_type = std::ptrdiff_t;
    using pointer = const O<N>*;
    using reference = O<N>;
    
    explicit constexpr O_range_iterator(uint8_t start) noexcept 
        : current_(start) {}
    
    constexpr O<N> operator*() const noexcept {
        return O<N>(current_);
    }
    
    constexpr O_range_iterator& operator++() noexcept {
        ++current_;
        return *this;
    }
    
    constexpr O_range_iterator operator++(int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    constexpr bool operator==(const O_range_iterator& other) const noexcept {
        return current_ == other.current_;
    }
    
    constexpr bool operator!=(const O_range_iterator& other) const noexcept {
        return !(*this == other);
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// RANGE CLASS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Range of all possible states [0, N-1]
 * 
 * Usage:
 * ```cpp
 * for (auto state : O_range<8>()) {
 *     // Iterate through all 8 states
 * }
 * ```
 */
template<uint8_t N>
class O_range {
public:
    using iterator = O_range_iterator<N>;
    
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

/**
 * @brief Factory function for range
 */
template<uint8_t N>
constexpr O_range<N> all_states() noexcept {
    return O_range<N>();
}

// ═══════════════════════════════════════════════════════════════════════════
// SUBRANGE (PARTIAL ITERATION)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Iterator for subrange [start, end)
 */
template<uint8_t N>
class O_subrange_iterator {
    uint8_t current_;
    uint8_t end_;
    
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = O<N>;
    using difference_type = std::ptrdiff_t;
    using pointer = const O<N>*;
    using reference = O<N>;
    
    constexpr O_subrange_iterator(uint8_t start, uint8_t end) noexcept 
        : current_(start), end_(end) {}
    
    constexpr O<N> operator*() const noexcept {
        return O<N>(current_);
    }
    
    constexpr O_subrange_iterator& operator++() noexcept {
        if (current_ < end_) ++current_;
        return *this;
    }
    
    constexpr O_subrange_iterator operator++(int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    constexpr bool operator==(const O_subrange_iterator& other) const noexcept {
        return current_ == other.current_;
    }
    
    constexpr bool operator!=(const O_subrange_iterator& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief Subrange [start, end)
 */
template<uint8_t N>
class O_subrange {
    uint8_t start_;
    uint8_t end_;
    
public:
    using iterator = O_subrange_iterator<N>;
    
    constexpr O_subrange(O<N> start, O<N> end) noexcept 
        : start_(start.value()), end_(end.value()) {}
    
    constexpr iterator begin() const noexcept {
        return iterator(start_, end_);
    }
    
    constexpr iterator end() const noexcept {
        return iterator(end_, end_);
    }
    
    constexpr size_t size() const noexcept {
        return (end_ > start_) ? (end_ - start_) : 0;
    }
};

/**
 * @brief Factory function for subrange
 */
template<uint8_t N>
constexpr O_subrange<N> range(O<N> start, O<N> end) noexcept {
    return O_subrange<N>(start, end);
}

// ═══════════════════════════════════════════════════════════════════════════
// STRIDE ITERATOR
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Iterator with custom stride
 * 
 * Allows iterating with step size > 1.
 */
template<uint8_t N>
class O_stride_iterator {
    uint8_t current_;
    uint8_t stride_;
    
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = O<N>;
    using difference_type = std::ptrdiff_t;
    using pointer = const O<N>*;
    using reference = O<N>;
    
    constexpr O_stride_iterator(uint8_t start, uint8_t stride) noexcept 
        : current_(start), stride_(stride) {}
    
    constexpr O<N> operator*() const noexcept {
        return O<N>(current_);
    }
    
    constexpr O_stride_iterator& operator++() noexcept {
        current_ = (current_ + stride_) % N;
        if (current_ == 0 && stride_ > 0) {
            current_ = N;  // End marker
        }
        return *this;
    }
    
    constexpr O_stride_iterator operator++(int) noexcept {
        auto temp = *this;
        ++(*this);
        return temp;
    }
    
    constexpr bool operator==(const O_stride_iterator& other) const noexcept {
        return current_ == other.current_;
    }
    
    constexpr bool operator!=(const O_stride_iterator& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief Strided range
 */
template<uint8_t N>
class O_strided_range {
    uint8_t stride_;
    
public:
    using iterator = O_stride_iterator<N>;
    
    explicit constexpr O_strided_range(uint8_t stride) noexcept 
        : stride_(stride) {}
    
    constexpr iterator begin() const noexcept {
        return iterator(0, stride_);
    }
    
    constexpr iterator end() const noexcept {
        return iterator(N, stride_);
    }
};

/**
 * @brief Factory function for strided range
 */
template<uint8_t N>
constexpr O_strided_range<N> stride(uint8_t step) noexcept {
    return O_strided_range<N>(step);
}

// ═══════════════════════════════════════════════════════════════════════════
// ARRAY ITERATION
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Generate all possible arrays of given size
 * 
 * Warning: Generates N^Size combinations, can be huge!
 */
template<uint8_t N, size_t Size>
class O_array_generator {
    std::vector<O_array<N, Size>> combinations_;
    
    void generate(O_array<N, Size>& current, size_t pos) {
        if (pos == Size) {
            combinations_.push_back(current);
            return;
        }
        
        for (uint8_t i = 0; i < N; ++i) {
            current[pos] = O<N>(i);
            generate(current, pos + 1);
        }
    }
    
public:
    O_array_generator() {
        O_array<N, Size> temp;
        generate(temp, 0);
    }
    
    const std::vector<O_array<N, Size>>& all() const {
        return combinations_;
    }
    
    size_t size() const {
        return combinations_.size();
    }
};

/**
 * @brief Generate all arrays (use with caution for large Size or N)
 */
template<uint8_t N, size_t Size>
O_array_generator<N, Size> all_arrays() {
    return O_array_generator<N, Size>();
}

// ═══════════════════════════════════════════════════════════════════════════
// FILTER OPERATIONS
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Filter states matching predicate
 */
template<uint8_t N, typename Predicate>
std::vector<O<N>> filter(Predicate pred) {
    std::vector<O<N>> result;
    for (auto state : O_range<N>()) {
        if (pred(state)) {
            result.push_back(state);
        }
    }
    return result;
}

/**
 * @brief Count states matching predicate
 */
template<uint8_t N, typename Predicate>
size_t count_if(Predicate pred) {
    size_t count = 0;
    for (auto state : O_range<N>()) {
        if (pred(state)) {
            ++count;
        }
    }
    return count;
}

} // namespace o

#endif // O_RANGES_HPP
