// matrix12x8.hpp - 12×8 Configuration Matrix for 3ODS
// Part of Language O - C++ Extensions for Octovalent Computing
//
// Copyright (c) 2025 Jean-Christophe Ané
// License: CC BY-NC-SA 4.0
//
// This header provides the matrix12x8 type representing the complete
// spatiotemporal configuration space: 12 temporal phases × 8 spatial octants
// = 96 possible system states.

#ifndef O_MATRIX12X8_HPP
#define O_MATRIX12X8_HPP

#include "oct8.hpp"
#include "phase12.hpp"
#include <array>
#include <algorithm>
#include <functional>
#include <numeric>

namespace o {

// ============================================================================
// matrix12x8 - 12×8 Spatiotemporal Configuration Matrix
// ============================================================================

/**
 * @brief 12×8 matrix representing complete 3ODS state space.
 * 
 * Organization: matrix[phase][octant]
 * - Dimension 1 (rows): 12 temporal phases
 * - Dimension 2 (cols): 8 spatial octants
 * - Total cells: 96 configurations
 * 
 * Template parameter T is the value type stored in each cell.
 * Common instantiations:
 * - matrix12x8<uint8_t>  : Numeric states
 * - matrix12x8<float>    : Continuous values
 * - matrix12x8<bool>     : Binary flags
 */
template<typename T = uint8_t>
class matrix12x8 {
private:
    std::array<std::array<T, 8>, 12> data_;
    
public:
    // ========================================================================
    // Construction
    // ========================================================================
    
    /**
     * @brief Default constructor (zero-initialized for numeric types).
     */
    constexpr matrix12x8() noexcept : data_{} {}
    
    /**
     * @brief Construct with uniform value.
     */
    explicit constexpr matrix12x8(const T& value) noexcept {
        fill(value);
    }
    
    /**
     * @brief Construct from initializer list (row-major order).
     * Example: matrix12x8<int> m({0,1,2,3,4,5,6,7, ...96 values...});
     */
    explicit matrix12x8(std::initializer_list<T> values) {
        if (values.size() != 96) {
            throw std::invalid_argument("matrix12x8 requires exactly 96 values");
        }
        auto it = values.begin();
        for (uint8_t p = 0; p < 12; p++) {
            for (uint8_t o = 0; o < 8; o++) {
                data_[p][o] = *it++;
            }
        }
    }
    
    // ========================================================================
    // Access
    // ========================================================================
    
    /**
     * @brief Access element at (phase, octant).
     */
    T& operator()(phase12 p, oct8 o) noexcept {
        return data_[p.value()][o.value()];
    }
    
    const T& operator()(phase12 p, oct8 o) const noexcept {
        return data_[p.value()][o.value()];
    }
    
    /**
     * @brief Access by raw indices.
     */
    T& at(uint8_t phase, uint8_t octant) {
        if (phase >= 12 || octant >= 8) {
            throw std::out_of_range("matrix12x8 index out of range");
        }
        return data_[phase][octant];
    }
    
    const T& at(uint8_t phase, uint8_t octant) const {
        if (phase >= 12 || octant >= 8) {
            throw std::out_of_range("matrix12x8 index out of range");
        }
        return data_[phase][octant];
    }
    
    /**
     * @brief Access entire row (all 8 octants at given phase).
     */
    std::array<T, 8>& row(phase12 p) noexcept {
        return data_[p.value()];
    }
    
    const std::array<T, 8>& row(phase12 p) const noexcept {
        return data_[p.value()];
    }
    
    /**
     * @brief Get column (all 12 phases at given octant).
     */
    std::array<T, 12> column(oct8 o) const noexcept {
        std::array<T, 12> col;
        for (uint8_t p = 0; p < 12; p++) {
            col[p] = data_[p][o.value()];
        }
        return col;
    }
    
    /**
     * @brief Direct array access (for advanced use).
     */
    std::array<std::array<T, 8>, 12>& data() noexcept { return data_; }
    const std::array<std::array<T, 8>, 12>& data() const noexcept { return data_; }
    
    // ========================================================================
    // Properties
    // ========================================================================
    
    /**
     * @brief Total number of cells (always 96).
     */
    constexpr size_t size() const noexcept { return 96; }
    
    /**
     * @brief Number of rows (phases, always 12).
     */
    constexpr size_t rows() const noexcept { return 12; }
    
    /**
     * @brief Number of columns (octants, always 8).
     */
    constexpr size_t cols() const noexcept { return 8; }
    
    // ========================================================================
    // Modification
    // ========================================================================
    
    /**
     * @brief Fill entire matrix with value.
     */
    constexpr void fill(const T& value) noexcept {
        for (auto& row : data_) {
            row.fill(value);
        }
    }
    
    /**
     * @brief Clear matrix (set to default value of T).
     */
    void clear() noexcept {
        fill(T{});
    }
    
    /**
     * @brief Apply function to each element.
     * Signature: void func(phase12 p, oct8 o, T& value)
     */
    void apply(std::function<void(phase12, oct8, T&)> func) {
        for (uint8_t p = 0; p < 12; p++) {
            for (uint8_t o = 0; o < 8; o++) {
                func(phase12(p), oct8(o), data_[p][o]);
            }
        }
    }
    
    /**
     * @brief Apply function to each element (const version).
     * Signature: void func(phase12 p, oct8 o, const T& value)
     */
    void apply(std::function<void(phase12, oct8, const T&)> func) const {
        for (uint8_t p = 0; p < 12; p++) {
            for (uint8_t o = 0; o < 8; o++) {
                func(phase12(p), oct8(o), data_[p][o]);
            }
        }
    }
    
    /**
     * @brief Transform each element.
     * Signature: T func(phase12 p, oct8 o, const T& value)
     */
    void transform(std::function<T(phase12, oct8, const T&)> func) {
        for (uint8_t p = 0; p < 12; p++) {
            for (uint8_t o = 0; o < 8; o++) {
                data_[p][o] = func(phase12(p), oct8(o), data_[p][o]);
            }
        }
    }
    
    // ========================================================================
    // Queries
    // ========================================================================
    
    /**
     * @brief Count elements satisfying predicate.
     */
    size_t count_if(std::function<bool(const T&)> pred) const {
        size_t count = 0;
        apply([&](phase12, oct8, const T& val) {
            if (pred(val)) count++;
        });
        return count;
    }
    
    /**
     * @brief Check if any element satisfies predicate.
     */
    bool any_of(std::function<bool(const T&)> pred) const {
        for (const auto& row : data_) {
            if (std::any_of(row.begin(), row.end(), pred)) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * @brief Check if all elements satisfy predicate.
     */
    bool all_of(std::function<bool(const T&)> pred) const {
        for (const auto& row : data_) {
            if (!std::all_of(row.begin(), row.end(), pred)) {
                return false;
            }
        }
        return true;
    }
    
    // ========================================================================
    // Aggregation (for numeric types)
    // ========================================================================
    
    /**
     * @brief Sum all elements (requires T to support operator+).
     */
    T sum() const {
        T total = T{};
        apply([&](phase12, oct8, const T& val) {
            total = total + val;
        });
        return total;
    }
    
    /**
     * @brief Average of all elements (requires T convertible to float).
     */
    float average() const {
        float total = 0.0f;
        apply([&](phase12, oct8, const T& val) {
            total += static_cast<float>(val);
        });
        return total / 96.0f;
    }
    
    /**
     * @brief Find minimum element.
     */
    T min() const {
        T result = data_[0][0];
        apply([&](phase12, oct8, const T& val) {
            if (val < result) result = val;
        });
        return result;
    }
    
    /**
     * @brief Find maximum element.
     */
    T max() const {
        T result = data_[0][0];
        apply([&](phase12, oct8, const T& val) {
            if (val > result) result = val;
        });
        return result;
    }
    
    // ========================================================================
    // Comparison
    // ========================================================================
    
    bool operator==(const matrix12x8& other) const noexcept {
        return data_ == other.data_;
    }
    
    bool operator!=(const matrix12x8& other) const noexcept {
        return data_ != other.data_;
    }
};

// ============================================================================
// Factory Functions
// ============================================================================

/**
 * @brief Create matrix filled with zeros.
 */
template<typename T = uint8_t>
inline matrix12x8<T> zeros() {
    return matrix12x8<T>(T{});
}

/**
 * @brief Create matrix filled with ones.
 */
template<typename T = uint8_t>
inline matrix12x8<T> ones() {
    return matrix12x8<T>(T{1});
}

/**
 * @brief Create matrix with identity pattern (diagonal-like).
 * Sets (p, p%8) to 1, rest to 0.
 */
template<typename T = uint8_t>
inline matrix12x8<T> identity() {
    matrix12x8<T> m;
    for (uint8_t p = 0; p < 12; p++) {
        m(phase12(p), oct8(p % 8)) = T{1};
    }
    return m;
}

/**
 * @brief Create matrix with random values.
 */
template<typename T = uint8_t>
inline matrix12x8<T> random(T min_val = T{}, T max_val = T{7}, uint32_t seed = 0) {
    static std::mt19937 rng(seed ? seed : std::random_device{}());
    std::uniform_int_distribution<T> dist(min_val, max_val);
    
    matrix12x8<T> m;
    m.apply([&](phase12, oct8, T& val) {
        val = dist(rng);
    });
    return m;
}

// ============================================================================
// Specialized Operations
// ============================================================================

/**
 * @brief Extract temporal profile (12 values, one per phase at given octant).
 */
template<typename T>
inline std::array<T, 12> temporal_profile(const matrix12x8<T>& m, oct8 o) {
    return m.column(o);
}

/**
 * @brief Extract spatial profile (8 values, one per octant at given phase).
 */
template<typename T>
inline std::array<T, 8> spatial_profile(const matrix12x8<T>& m, phase12 p) {
    return m.row(p);
}

/**
 * @brief Rotate matrix temporally (shift phases).
 * @param shift Number of phases to shift (positive = forward, negative = backward)
 */
template<typename T>
inline matrix12x8<T> rotate_temporal(const matrix12x8<T>& m, int shift) {
    matrix12x8<T> result;
    for (uint8_t p = 0; p < 12; p++) {
        phase12 new_p = phase12(p) + shift;
        result.row(new_p) = m.row(phase12(p));
    }
    return result;
}

/**
 * @brief Mirror matrix spatially (reflect octants).
 * @param axis Axis to mirror across ('X', 'Y', 'Z', or 'A' for all)
 */
template<typename T>
inline matrix12x8<T> mirror_spatial(const matrix12x8<T>& m, char axis) {
    matrix12x8<T> result;
    
    for (uint8_t p = 0; p < 12; p++) {
        for (uint8_t o = 0; o < 8; o++) {
            oct8 orig(o);
            oct8 mirrored = orig;
            
            switch (axis) {
                case 'X': mirrored = reflect_yz(orig); break;
                case 'Y': mirrored = reflect_xz(orig); break;
                case 'Z': mirrored = reflect_xy(orig); break;
                case 'A': mirrored = invert(orig); break;  // All axes
                default: mirrored = orig;
            }
            
            result(phase12(p), mirrored) = m(phase12(p), orig);
        }
    }
    
    return result;
}

/**
 * @brief Compute correlation between two matrices (Pearson-like).
 * Returns value in [-1, 1] where 1 = identical, -1 = inverted, 0 = uncorrelated.
 */
template<typename T>
inline float correlation(const matrix12x8<T>& a, const matrix12x8<T>& b) {
    float mean_a = a.average();
    float mean_b = b.average();
    
    float numerator = 0.0f;
    float denom_a = 0.0f;
    float denom_b = 0.0f;
    
    a.apply([&](phase12 p, oct8 o, const T& val_a) {
        float diff_a = static_cast<float>(val_a) - mean_a;
        float diff_b = static_cast<float>(b(p, o)) - mean_b;
        
        numerator += diff_a * diff_b;
        denom_a += diff_a * diff_a;
        denom_b += diff_b * diff_b;
    });
    
    float denom = std::sqrt(denom_a * denom_b);
    return (denom > 0.0f) ? (numerator / denom) : 0.0f;
}

// ============================================================================
// Pretty Printing
// ============================================================================

/**
 * @brief Convert matrix to string representation (compact).
 */
template<typename T>
inline std::string to_string(const matrix12x8<T>& m) {
    std::string result = "matrix12x8 [\n";
    for (uint8_t p = 0; p < 12; p++) {
        result += "  [";
        for (uint8_t o = 0; o < 8; o++) {
            result += std::to_string(m(phase12(p), oct8(o)));
            if (o < 7) result += ", ";
        }
        result += "]";
        if (p < 11) result += ",\n";
    }
    result += "\n]";
    return result;
}

} // namespace o

#endif // O_MATRIX12X8_HPP
