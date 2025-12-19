// phase12.hpp - Temporal Phase Type for 3ODS Multi-State Systems
// Part of Language O - C++ Extensions for Octovalent Computing
//
// Copyright (c) 2025 Jean-Christophe Ané
// License: CC BY-NC-SA 4.0
//
// This header provides the phase12 type representing 12 temporal phases
// corresponding to the 12 edges of a cube in 3ODS architecture.

#ifndef O_PHASE12_HPP
#define O_PHASE12_HPP

#include <cstdint>
#include <string>
#include <array>
#include <random>
#include <stdexcept>

namespace o {

// ============================================================================
// phase12 - Temporal Phase Type (12 phases like clock hours)
// ============================================================================

/**
 * @brief Represents one of 12 temporal phases in 3ODS.
 * 
 * The 12 phases correspond to the 12 edges of a cube, representing
 * temporal transitions between spatial octants.
 * 
 * Phase encoding:
 *   0-3   : X-parallel edges (transitions along X-axis)
 *   4-7   : Y-parallel edges (transitions along Y-axis)
 *   8-11  : Z-parallel edges (transitions along Z-axis)
 * 
 * Conceptually like a 12-hour clock where phase 0 = "midnight"
 * and the cycle repeats every 12 phases.
 */
class phase12 {
private:
    uint8_t value_;  // Always in range [0, 11]
    
public:
    // Construction
    constexpr phase12() noexcept : value_(0) {}
    constexpr explicit phase12(uint8_t v) noexcept : value_(v % 12) {}
    
    // Conversion
    constexpr operator uint8_t() const noexcept { return value_; }
    constexpr uint8_t value() const noexcept { return value_; }
    
    // Comparison
    constexpr bool operator==(phase12 other) const noexcept { return value_ == other.value_; }
    constexpr bool operator!=(phase12 other) const noexcept { return value_ != other.value_; }
    constexpr bool operator<(phase12 other) const noexcept { return value_ < other.value_; }
    constexpr bool operator<=(phase12 other) const noexcept { return value_ <= other.value_; }
    constexpr bool operator>(phase12 other) const noexcept { return value_ > other.value_; }
    constexpr bool operator>=(phase12 other) const noexcept { return value_ >= other.value_; }
    
    // Arithmetic (modulo 12 - circular)
    constexpr phase12 operator+(int delta) const noexcept {
        return phase12(static_cast<uint8_t>((value_ + delta + 12) % 12));
    }
    constexpr phase12 operator-(int delta) const noexcept {
        return phase12(static_cast<uint8_t>((value_ - delta + 12) % 12));
    }
    
    phase12& operator+=(int delta) noexcept {
        value_ = (value_ + delta + 12) % 12;
        return *this;
    }
    phase12& operator-=(int delta) noexcept {
        value_ = (value_ - delta + 12) % 12;
        return *this;
    }
    
    // Pre/post increment (circular)
    phase12& operator++() noexcept {
        value_ = (value_ + 1) % 12;
        return *this;
    }
    phase12 operator++(int) noexcept {
        phase12 tmp = *this;
        ++(*this);
        return tmp;
    }
    phase12& operator--() noexcept {
        value_ = (value_ == 0) ? 11 : value_ - 1;
        return *this;
    }
    phase12 operator--(int) noexcept {
        phase12 tmp = *this;
        --(*this);
        return tmp;
    }
    
    // Axis classification
    constexpr bool is_x_parallel() const noexcept { return value_ < 4; }
    constexpr bool is_y_parallel() const noexcept { return value_ >= 4 && value_ < 8; }
    constexpr bool is_z_parallel() const noexcept { return value_ >= 8; }
    
    // Temporal quadrant (group of 3 phases)
    constexpr uint8_t quadrant() const noexcept { return value_ / 3; }  // 0-3
    
    // String representation
    std::string to_string() const {
        return "Phase_" + std::to_string(value_);
    }
    
    // Clock-like string (0="00:00", 11="11:00")
    std::string to_clock() const {
        return (value_ < 10 ? "0" : "") + std::to_string(value_) + ":00";
    }
};

// ============================================================================
// Phase Operations
// ============================================================================

/**
 * @brief Advance to next phase (circular).
 */
inline constexpr phase12 next(phase12 p) noexcept {
    return p + 1;
}

/**
 * @brief Go to previous phase (circular).
 */
inline constexpr phase12 prev(phase12 p) noexcept {
    return p - 1;
}

/**
 * @brief Advance by N phases (can be negative).
 */
inline constexpr phase12 advance(phase12 p, int n) noexcept {
    return p + n;
}

/**
 * @brief Distance between two phases (shortest circular distance).
 * @return Distance in range [0, 6]
 */
inline constexpr uint8_t phase_distance(phase12 a, phase12 b) noexcept {
    int diff = static_cast<int>(b.value()) - static_cast<int>(a.value());
    diff = (diff + 12) % 12;  // Normalize to [0, 11]
    return (diff <= 6) ? diff : (12 - diff);  // Shorter circular distance
}

/**
 * @brief Check if two phases are opposite (6 phases apart).
 */
inline constexpr bool are_opposite(phase12 a, phase12 b) noexcept {
    return phase_distance(a, b) == 6;
}

/**
 * @brief Check if phases are adjacent (1 phase apart).
 */
inline constexpr bool are_adjacent(phase12 a, phase12 b) noexcept {
    return phase_distance(a, b) == 1;
}

// ============================================================================
// Temporal Utilities
// ============================================================================

/**
 * @brief Convert time (seconds) to phase given cycle period.
 * @param time_s Time in seconds
 * @param cycle_period_s Duration of full 12-phase cycle in seconds
 * @return Current phase [0-11]
 */
inline phase12 time_to_phase(float time_s, float cycle_period_s) noexcept {
    float phase_duration = cycle_period_s / 12.0f;
    uint8_t p = static_cast<uint8_t>(time_s / phase_duration) % 12;
    return phase12(p);
}

/**
 * @brief Convert phase to time offset within cycle.
 * @param p Phase
 * @param cycle_period_s Duration of full cycle
 * @return Time offset in seconds [0, cycle_period_s)
 */
inline float phase_to_time(phase12 p, float cycle_period_s) noexcept {
    return (p.value() * cycle_period_s) / 12.0f;
}

/**
 * @brief Calculate progress within a phase (0.0 to 1.0).
 * @param time_s Current time in seconds
 * @param cycle_period_s Full cycle duration
 * @return Progress [0.0, 1.0) within current phase
 */
inline float phase_progress(float time_s, float cycle_period_s) noexcept {
    float phase_duration = cycle_period_s / 12.0f;
    float time_in_phase = std::fmod(time_s, phase_duration);
    return time_in_phase / phase_duration;
}

/**
 * @brief Generate random phase.
 */
inline phase12 random_phase(uint32_t seed = 0) {
    static std::mt19937 rng(seed ? seed : std::random_device{}());
    static std::uniform_int_distribution<uint8_t> dist(0, 11);
    return phase12(dist(rng));
}

// ============================================================================
// Phase Sequences
// ============================================================================

/**
 * @brief Generate sequence of N consecutive phases starting from p.
 */
inline std::array<phase12, 12> phase_cycle(phase12 start = phase12(0)) noexcept {
    std::array<phase12, 12> result;
    for (uint8_t i = 0; i < 12; i++) {
        result[i] = start + i;
    }
    return result;
}

/**
 * @brief Get all phases in a given axis group.
 */
inline std::array<phase12, 4> x_phases() noexcept {
    return {phase12(0), phase12(1), phase12(2), phase12(3)};
}

inline std::array<phase12, 4> y_phases() noexcept {
    return {phase12(4), phase12(5), phase12(6), phase12(7)};
}

inline std::array<phase12, 4> z_phases() noexcept {
    return {phase12(8), phase12(9), phase12(10), phase12(11)};
}

// ============================================================================
// Constants
// ============================================================================

namespace phases {
    // Individual phase constants
    constexpr phase12 phase_0  = phase12(0);   // Midnight
    constexpr phase12 phase_1  = phase12(1);
    constexpr phase12 phase_2  = phase12(2);
    constexpr phase12 phase_3  = phase12(3);   // End of night
    constexpr phase12 phase_4  = phase12(4);   // Dawn
    constexpr phase12 phase_5  = phase12(5);
    constexpr phase12 phase_6  = phase12(6);   // Noon
    constexpr phase12 phase_7  = phase12(7);
    constexpr phase12 phase_8  = phase12(8);   // Afternoon
    constexpr phase12 phase_9  = phase12(9);
    constexpr phase12 phase_10 = phase12(10);
    constexpr phase12 phase_11 = phase12(11);  // Evening
    
    // Semantic aliases
    constexpr phase12 midnight = phase_0;
    constexpr phase12 dawn     = phase_4;
    constexpr phase12 noon     = phase_6;
    constexpr phase12 dusk     = phase_9;
}

// ============================================================================
// Edge Mapping (phase12 ↔ cube edges)
// ============================================================================

/**
 * @brief Cube edge represented by two octant endpoints.
 */
struct CubeEdge {
    uint8_t from_octant;  // Starting octant [0-7]
    uint8_t to_octant;    // Ending octant [0-7]
    char axis;            // 'X', 'Y', or 'Z'
};

/**
 * @brief Get cube edge corresponding to a phase.
 * 
 * Each of the 12 phases maps to one of the 12 edges of a cube.
 */
inline constexpr CubeEdge phase_to_edge(phase12 p) noexcept {
    // Lookup table: [phase] = {from, to, axis}
    constexpr CubeEdge edges[12] = {
        // X-parallel edges (phases 0-3)
        {0, 1, 'X'},  // Phase 0: South-Low edge
        {2, 3, 'X'},  // Phase 1: North-Low edge
        {4, 5, 'X'},  // Phase 2: South-High edge
        {6, 7, 'X'},  // Phase 3: North-High edge
        
        // Y-parallel edges (phases 4-7)
        {0, 2, 'Y'},  // Phase 4: West-Low edge
        {1, 3, 'Y'},  // Phase 5: East-Low edge
        {4, 6, 'Y'},  // Phase 6: West-High edge
        {5, 7, 'Y'},  // Phase 7: East-High edge
        
        // Z-parallel edges (phases 8-11)
        {0, 4, 'Z'},  // Phase 8: SW vertical
        {1, 5, 'Z'},  // Phase 9: SE vertical
        {2, 6, 'Z'},  // Phase 10: NW vertical
        {3, 7, 'Z'}   // Phase 11: NE vertical
    };
    
    return edges[p.value()];
}

/**
 * @brief Find phase connecting two octants.
 * @return phase12 if edge exists, throws if octants not adjacent
 */
inline phase12 edge_to_phase(uint8_t from_octant, uint8_t to_octant) {
    // Search through all 12 edges
    for (uint8_t p = 0; p < 12; p++) {
        CubeEdge edge = phase_to_edge(phase12(p));
        if (edge.from_octant == from_octant && edge.to_octant == to_octant) {
            return phase12(p);
        }
        // Check reverse direction too
        if (edge.from_octant == to_octant && edge.to_octant == from_octant) {
            return phase12(p);
        }
    }
    
    throw std::invalid_argument("Octants are not adjacent (no edge connects them)");
}

} // namespace o

#endif // O_PHASE12_HPP
