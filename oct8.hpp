// oct8.hpp - Octovalent Type for 3ODS Multi-State Systems
// Part of Language O - C++ Extensions for Octovalent Computing
//
// Copyright (c) 2025 Jean-Christophe Ané
// License: CC BY-NC-SA 4.0
//
// This header provides the fundamental oct8 type representing
// 8 spatial octants in 3D Euclidean space, along with geometric
// operations (rotations, reflections) and distance calculations.

#ifndef O_OCT8_HPP
#define O_OCT8_HPP

#include <cstdint>
#include <cmath>
#include <array>
#include <random>
#include <stdexcept>
#include <string>

namespace o {

// ============================================================================
// oct8 - Octovalent Type (8 states representing 3D octants)
// ============================================================================

/**
 * @brief Represents one of 8 spatial octants in 3D Euclidean space.
 * 
 * Octant encoding (binary xyz):
 *   0 (000) -> (-, -, -) South-West-Low
 *   1 (001) -> (+, -, -) South-East-Low
 *   2 (010) -> (-, +, -) North-West-Low
 *   3 (011) -> (+, +, -) North-East-Low
 *   4 (100) -> (-, -, +) South-West-High
 *   5 (101) -> (+, -, +) South-East-High
 *   6 (110) -> (-, +, +) North-West-High
 *   7 (111) -> (+, +, +) North-East-High
 * 
 * where bit 0 = X sign, bit 1 = Y sign, bit 2 = Z sign
 */
class oct8 {
private:
    uint8_t value_;  // Always in range [0, 7]
    
public:
    // Construction
    constexpr oct8() noexcept : value_(0) {}
    constexpr explicit oct8(uint8_t v) noexcept : value_(v & 0x7) {}  // Mask to ensure [0,7]
    
    // Conversion
    constexpr operator uint8_t() const noexcept { return value_; }
    constexpr uint8_t value() const noexcept { return value_; }
    
    // Comparison
    constexpr bool operator==(oct8 other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(oct8 other) const noexcept { return value_ != other.value_; }
    constexpr bool operator<(oct8 other) const noexcept { return value_ < other.value_; }
    constexpr bool operator<=(oct8 other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>(oct8 other) const noexcept { return value_ > other.value_; }
    constexpr bool operator>=(oct8 other) const noexcept { return value_ >= other.value_; }
    
    // Arithmetic (modulo 8)
    constexpr oct8 operator+(int delta) const noexcept {
        return oct8(static_cast<uint8_t>((value_ + delta) & 0x7));
    }
    constexpr oct8 operator-(int delta) const noexcept {
        return oct8(static_cast<uint8_t>((value_ - delta) & 0x7));
    }
    
    oct8& operator+=(int delta) noexcept {
        value_ = (value_ + delta) & 0x7;
        return *this;
    }
    oct8& operator-=(int delta) noexcept {
        value_ = (value_ - delta) & 0x7;
        return *this;
    }
    
    // Bit access (for spatial coordinates)
    constexpr bool x_sign() const noexcept { return (value_ & 0x1) != 0; }  // Bit 0
    constexpr bool y_sign() const noexcept { return (value_ & 0x2) != 0; }  // Bit 1
    constexpr bool z_sign() const noexcept { return (value_ & 0x4) != 0; }  // Bit 2
    
    // Spatial coordinates as {-1, +1}
    constexpr int x_coord() const noexcept { return x_sign() ? +1 : -1; }
    constexpr int y_coord() const noexcept { return y_sign() ? +1 : -1; }
    constexpr int z_coord() const noexcept { return z_sign() ? +1 : -1; }
    
    // String representation
    std::string to_string() const {
        return std::string(x_sign() ? "+" : "-") +
               std::string(y_sign() ? "+" : "-") +
               std::string(z_sign() ? "+" : "-");
    }
};

// ============================================================================
// Geometric Operations
// ============================================================================

/**
 * @brief Rotate octant around X-axis.
 * @param o Input octant
 * @param angle Rotation angle in degrees (90, 180, 270, -90, etc.)
 * @return Rotated octant
 * 
 * X-axis rotation leaves X unchanged, rotates Y and Z.
 * Positive angle = counter-clockwise when looking from +X.
 */
inline oct8 rotate_x(oct8 o, int angle) noexcept {
    // Normalize angle to [0, 360)
    angle = ((angle % 360) + 360) % 360;
    
    // X-axis rotation lookup table
    // Format: [input_octant][angle/90] = output_octant
    static constexpr uint8_t table[8][4] = {
        // 0°   90°  180° 270°
        {0,    2,    4,   6},  // Octant 0 (---)
        {1,    3,    5,   7},  // Octant 1 (+--)
        {2,    4,    6,   0},  // Octant 2 (-+-)
        {3,    5,    7,   1},  // Octant 3 (++-)
        {4,    6,    0,   2},  // Octant 4 (--+)
        {5,    7,    1,   3},  // Octant 5 (+-+)
        {6,    0,    2,   4},  // Octant 6 (-++)
        {7,    1,    3,   5}   // Octant 7 (+++)
    };
    
    uint8_t index = angle / 90;  // 0, 1, 2, 3 for 0°, 90°, 180°, 270°
    return oct8(table[o.value()][index]);
}

/**
 * @brief Rotate octant around Y-axis.
 * @param o Input octant
 * @param angle Rotation angle in degrees
 * @return Rotated octant
 * 
 * Y-axis rotation leaves Y unchanged, rotates X and Z.
 */
inline oct8 rotate_y(oct8 o, int angle) noexcept {
    angle = ((angle % 360) + 360) % 360;
    
    static constexpr uint8_t table[8][4] = {
        {0,    1,    4,   5},  // 0
        {1,    4,    5,   0},  // 1
        {2,    3,    6,   7},  // 2
        {3,    6,    7,   2},  // 3
        {4,    5,    0,   1},  // 4
        {5,    0,    1,   4},  // 5
        {6,    7,    2,   3},  // 6
        {7,    2,    3,   6}   // 7
    };
    
    uint8_t index = angle / 90;
    return oct8(table[o.value()][index]);
}

/**
 * @brief Rotate octant around Z-axis.
 * @param o Input octant
 * @param angle Rotation angle in degrees
 * @return Rotated octant
 * 
 * Z-axis rotation leaves Z unchanged, rotates X and Y.
 */
inline oct8 rotate_z(oct8 o, int angle) noexcept {
    angle = ((angle % 360) + 360) % 360;
    
    static constexpr uint8_t table[8][4] = {
        {0,    2,    3,   1},  // 0
        {1,    0,    2,   3},  // 1
        {2,    3,    1,   0},  // 2
        {3,    1,    0,   2},  // 3
        {4,    6,    7,   5},  // 4
        {5,    4,    6,   7},  // 5
        {6,    7,    5,   4},  // 6
        {7,    5,    4,   6}   // 7
    };
    
    uint8_t index = angle / 90;
    return oct8(table[o.value()][index]);
}

/**
 * @brief Reflect octant across XY plane (negate Z).
 */
inline constexpr oct8 reflect_xy(oct8 o) noexcept {
    return oct8(o.value() ^ 0x4);  // Flip bit 2 (Z)
}

/**
 * @brief Reflect octant across XZ plane (negate Y).
 */
inline constexpr oct8 reflect_xz(oct8 o) noexcept {
    return oct8(o.value() ^ 0x2);  // Flip bit 1 (Y)
}

/**
 * @brief Reflect octant across YZ plane (negate X).
 */
inline constexpr oct8 reflect_yz(oct8 o) noexcept {
    return oct8(o.value() ^ 0x1);  // Flip bit 0 (X)
}

/**
 * @brief Invert octant (negate all coordinates).
 */
inline constexpr oct8 invert(oct8 o) noexcept {
    return oct8(o.value() ^ 0x7);  // Flip all 3 bits
}

// ============================================================================
// Distance Metrics
// ============================================================================

/**
 * @brief Hamming distance between octants (number of differing bits).
 * @return Distance in range [0, 3]
 */
inline constexpr uint8_t hamming_distance(oct8 a, oct8 b) noexcept {
    uint8_t xor_val = a.value() ^ b.value();
    // Count set bits (popcount)
    return ((xor_val & 1) + ((xor_val >> 1) & 1) + ((xor_val >> 2) & 1));
}

/**
 * @brief Euclidean distance between octant vertices.
 * 
 * For unit cube (vertices at {-1, +1}³), distances are:
 *   Hamming 1 -> distance 2.0 (edge)
 *   Hamming 2 -> distance 2√2 ≈ 2.828 (face diagonal)
 *   Hamming 3 -> distance 2√3 ≈ 3.464 (space diagonal)
 * 
 * For half-unit cube (vertices at {-0.5, +0.5}³), divide by 2:
 *   Hamming 1 -> distance 1.0
 *   Hamming 2 -> distance √2 ≈ 1.414
 *   Hamming 3 -> distance √3 ≈ 1.732
 * 
 * @param unit_cube If true, use {-1,+1}³. If false, use {-0.5,+0.5}³.
 * @return Euclidean distance
 */
inline float euclidean_distance(oct8 a, oct8 b, bool unit_cube = false) noexcept {
    uint8_t h = hamming_distance(a, b);
    float base_distance = std::sqrt(static_cast<float>(h));
    return unit_cube ? (2.0f * base_distance) : base_distance;
}

/**
 * @brief Simplified distance (just use Hamming as proxy).
 * Faster, preserves order, but not true Euclidean.
 */
inline constexpr uint8_t distance(oct8 a, oct8 b) noexcept {
    return hamming_distance(a, b);
}

// ============================================================================
// Neighbors
// ============================================================================

/**
 * @brief Get all 3 adjacent neighbors (along cube edges).
 * @return Array of 3 neighbors differing by 1 bit (Hamming distance 1).
 */
inline std::array<oct8, 3> edge_neighbors(oct8 o) noexcept {
    return {
        oct8(o.value() ^ 0x1),  // Flip X
        oct8(o.value() ^ 0x2),  // Flip Y
        oct8(o.value() ^ 0x4)   // Flip Z
    };
}

/**
 * @brief Get all 3 face-diagonal neighbors (Hamming distance 2).
 */
inline std::array<oct8, 3> face_neighbors(oct8 o) noexcept {
    return {
        oct8(o.value() ^ 0x3),  // Flip X and Y
        oct8(o.value() ^ 0x5),  // Flip X and Z
        oct8(o.value() ^ 0x6)   // Flip Y and Z
    };
}

/**
 * @brief Get the space-diagonal neighbor (Hamming distance 3).
 */
inline constexpr oct8 opposite(oct8 o) noexcept {
    return invert(o);
}

// ============================================================================
// Utilities
// ============================================================================

/**
 * @brief Create octant from spatial signs.
 * @param x_positive X coordinate sign (true = +, false = -)
 * @param y_positive Y coordinate sign
 * @param z_positive Z coordinate sign
 */
inline constexpr oct8 from_signs(bool x_positive, bool y_positive, bool z_positive) noexcept {
    return oct8((z_positive ? 4 : 0) | (y_positive ? 2 : 0) | (x_positive ? 1 : 0));
}

/**
 * @brief Create octant from coordinates in range [-1, +1].
 * Values >= 0 map to positive, < 0 map to negative.
 */
inline oct8 from_coords(float x, float y, float z) noexcept {
    return from_signs(x >= 0.0f, y >= 0.0f, z >= 0.0f);
}

/**
 * @brief Generate random octant.
 */
inline oct8 random_octant(uint32_t seed = 0) {
    static std::mt19937 rng(seed ? seed : std::random_device{}());
    static std::uniform_int_distribution<uint8_t> dist(0, 7);
    return oct8(dist(rng));
}

/**
 * @brief Identity octant (origin, all negative).
 */
inline constexpr oct8 identity() noexcept {
    return oct8(0);
}

/**
 * @brief Maximum octant (all positive).
 */
inline constexpr oct8 maximum() noexcept {
    return oct8(7);
}

// ============================================================================
// Constants
// ============================================================================

namespace octants {
    constexpr oct8 origin       = oct8(0);  // (-, -, -)
    constexpr oct8 south_west_low  = oct8(0);
    constexpr oct8 south_east_low  = oct8(1);
    constexpr oct8 north_west_low  = oct8(2);
    constexpr oct8 north_east_low  = oct8(3);
    constexpr oct8 south_west_high = oct8(4);
    constexpr oct8 south_east_high = oct8(5);
    constexpr oct8 north_west_high = oct8(6);
    constexpr oct8 north_east_high = oct8(7);
    constexpr oct8 maximum_expansion = oct8(7);  // (+, +, +)
}

} // namespace o

#endif // O_OCT8_HPP
