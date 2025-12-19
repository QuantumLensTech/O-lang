// test_o_integrated.cpp - Integrated test suite for Language O
//
// Tests oct8, phase12, and matrix12x8 working together
//
// Compile: g++ -std=c++17 -I. test_o_integrated.cpp -o test_o
// Run: ./test_o

#include "oct8.hpp"
#include "phase12.hpp"
#include "matrix12x8.hpp"
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace o;

// ============================================================================
// Test Functions
// ============================================================================

void test_phase12_basic() {
    std::cout << "Testing phase12 basics..." << std::endl;
    
    phase12 p0;              // Default: 0
    phase12 p5(5);
    phase12 p15(15);         // Wraps to 3 (15 % 12)
    
    assert(p0.value() == 0);
    assert(p5.value() == 5);
    assert(p15.value() == 3);
    
    // Circular arithmetic
    phase12 p = phase12(11);
    p++;                     // Wraps to 0
    assert(p.value() == 0);
    
    p = phase12(0);
    p--;                     // Wraps to 11
    assert(p.value() == 11);
    
    std::cout << "  ✓ phase12 basics work" << std::endl;
}

void test_phase12_distances() {
    std::cout << "Testing phase12 distances..." << std::endl;
    
    // Circular distance
    assert(phase_distance(phase12(0), phase12(3)) == 3);
    assert(phase_distance(phase12(0), phase12(9)) == 3);  // Shorter: 0→11→10→9
    assert(phase_distance(phase12(0), phase12(6)) == 6);  // Opposite
    
    // Adjacent
    assert(are_adjacent(phase12(5), phase12(6)));
    assert(are_adjacent(phase12(11), phase12(0)));  // Circular
    assert(!are_adjacent(phase12(0), phase12(5)));
    
    // Opposite
    assert(are_opposite(phase12(0), phase12(6)));
    assert(are_opposite(phase12(3), phase12(9)));
    
    std::cout << "  ✓ phase12 distances work" << std::endl;
}

void test_phase12_temporal() {
    std::cout << "Testing phase12 temporal conversions..." << std::endl;
    
    float cycle_period = 12.0f;  // 12 seconds cycle
    
    // Time to phase
    phase12 p0 = time_to_phase(0.0f, cycle_period);
    phase12 p6 = time_to_phase(6.0f, cycle_period);
    phase12 p12 = time_to_phase(12.0f, cycle_period);  // Full cycle
    
    assert(p0.value() == 0);
    assert(p6.value() == 6);
    assert(p12.value() == 0);  // Back to start
    
    // Phase to time
    float t6 = phase_to_time(phase12(6), cycle_period);
    assert(std::abs(t6 - 6.0f) < 0.01f);
    
    std::cout << "  ✓ Temporal conversions work" << std::endl;
}

void test_phase12_edges() {
    std::cout << "Testing phase12 edge mapping..." << std::endl;
    
    // Phase to edge
    CubeEdge e0 = phase_to_edge(phase12(0));
    assert(e0.from_octant == 0 && e0.to_octant == 1 && e0.axis == 'X');
    
    CubeEdge e8 = phase_to_edge(phase12(8));
    assert(e8.from_octant == 0 && e8.to_octant == 4 && e8.axis == 'Z');
    
    // Edge to phase
    phase12 p = edge_to_phase(0, 1);
    assert(p.value() == 0);
    
    std::cout << "  ✓ Edge mapping works" << std::endl;
}

void test_matrix_basic() {
    std::cout << "Testing matrix12x8 basics..." << std::endl;
    
    // Construction
    matrix12x8<uint8_t> m1;                    // Default: zeros
    matrix12x8<uint8_t> m2(42);               // Filled with 42
    
    assert(m1(phase12(0), oct8(0)) == 0);
    assert(m2(phase12(5), oct8(3)) == 42);
    
    // Access
    m1(phase12(3), oct8(7)) = 99;
    assert(m1(phase12(3), oct8(7)) == 99);
    
    // Properties
    assert(m1.size() == 96);
    assert(m1.rows() == 12);
    assert(m1.cols() == 8);
    
    std::cout << "  ✓ matrix12x8 basics work" << std::endl;
}

void test_matrix_operations() {
    std::cout << "Testing matrix12x8 operations..." << std::endl;
    
    matrix12x8<int> m;
    
    // Fill
    m.fill(5);
    assert(m(phase12(0), oct8(0)) == 5);
    assert(m(phase12(11), oct8(7)) == 5);
    
    // Apply
    m.apply([](phase12 p, oct8 o, int& val) {
        val = p.value() + o.value();  // val = phase + octant
    });
    
    assert(m(phase12(0), oct8(0)) == 0);
    assert(m(phase12(5), oct8(3)) == 8);
    assert(m(phase12(11), oct8(7)) == 18);
    
    // Transform
    m.transform([](phase12, oct8, const int& val) {
        return val * 2;
    });
    
    assert(m(phase12(5), oct8(3)) == 16);  // Was 8, now 16
    
    std::cout << "  ✓ Matrix operations work" << std::endl;
}

void test_matrix_queries() {
    std::cout << "Testing matrix12x8 queries..." << std::endl;
    
    matrix12x8<int> m;
    m.apply([](phase12 p, oct8 o, int& val) {
        val = (p.value() == o.value()) ? 1 : 0;  // Diagonal
    });
    
    // Count
    size_t count = m.count_if([](const int& val) { return val == 1; });
    assert(count == 8);  // 8 diagonal elements (phase 0-7 match octants 0-7)
    
    // Any/All
    assert(m.any_of([](const int& val) { return val == 1; }));
    assert(!m.all_of([](const int& val) { return val == 1; }));
    
    std::cout << "  ✓ Matrix queries work" << std::endl;
}

void test_matrix_aggregation() {
    std::cout << "Testing matrix12x8 aggregation..." << std::endl;
    
    matrix12x8<int> m;
    m.fill(10);
    
    // Sum
    int sum = m.sum();
    assert(sum == 960);  // 96 cells × 10
    
    // Average
    float avg = m.average();
    assert(std::abs(avg - 10.0f) < 0.01f);
    
    // Min/Max
    m(phase12(5), oct8(3)) = 1;
    m(phase12(7), oct8(6)) = 99;
    
    assert(m.min() == 1);
    assert(m.max() == 99);
    
    std::cout << "  ✓ Matrix aggregation works" << std::endl;
}

void test_integration() {
    std::cout << "Testing full integration (oct8 + phase12 + matrix12x8)..." << std::endl;
    
    matrix12x8<uint8_t> state;
    
    // Initialize: each cell = octant value
    state.apply([](phase12, oct8 o, uint8_t& val) {
        val = o.value();
    });
    
    // Simulate temporal evolution: rotate octants at each phase
    for (uint8_t p = 0; p < 12; p++) {
        phase12 ph(p);
        
        for (uint8_t o = 0; o < 8; o++) {
            oct8 original(o);
            
            // Apply rotation based on phase
            oct8 rotated = original;
            if (ph.is_x_parallel()) {
                rotated = rotate_x(original, 90);
            } else if (ph.is_y_parallel()) {
                rotated = rotate_y(original, 90);
            } else {
                rotated = rotate_z(original, 90);
            }
            
            state(ph, original) = rotated.value();
        }
    }
    
    // Verify some rotations
    assert(state(phase12(0), oct8(0)) == 2);  // X-rotation of octant 0
    assert(state(phase12(4), oct8(0)) == 1);  // Y-rotation of octant 0
    
    std::cout << "  ✓ Full integration works" << std::endl;
}

// ============================================================================
// Example Applications
// ============================================================================

void example_temporal_simulation() {
    std::cout << "\n=== Example: Temporal State Evolution ===" << std::endl;
    
    matrix12x8<uint8_t> system;
    
    // Initial state: all octants at value 0
    system.fill(0);
    
    std::cout << "Simulating 12-phase cycle..." << std::endl;
    
    // Evolve through phases
    for (uint8_t p = 0; p < 12; p++) {
        phase12 current_phase(p);
        
        std::cout << "Phase " << p << " (" << current_phase.to_clock() << "): ";
        
        // Update each octant based on temporal dynamics
        for (uint8_t o = 0; o < 8; o++) {
            oct8 octant(o);
            
            // Simple rule: increment if phase matches octant quadrant
            if (p / 3 == o / 2) {
                system(current_phase, octant)++;
            }
            
            std::cout << static_cast<int>(system(current_phase, octant)) << " ";
        }
        std::cout << std::endl;
    }
}

void example_spatial_pathfinding() {
    std::cout << "\n=== Example: Spatiotemporal Pathfinding ===" << std::endl;
    
    matrix12x8<float> cost_map;
    
    // Initialize costs: distance from origin
    cost_map.apply([](phase12, oct8 o, float& cost) {
        cost = euclidean_distance(oct8(0), o, false);
    });
    
    std::cout << "Cost map (distance from origin):" << std::endl;
    for (uint8_t p = 0; p < 3; p++) {  // Show first 3 phases
        std::cout << "  Phase " << p << ": ";
        for (uint8_t o = 0; o < 8; o++) {
            std::cout << std::fixed << std::setprecision(2) 
                      << cost_map(phase12(p), oct8(o)) << " ";
        }
        std::cout << std::endl;
    }
    
    // Find optimal phase-octant with minimal cost
    float min_cost = cost_map.min();
    std::cout << "Minimum cost: " << min_cost << std::endl;
}

void example_octobrain_pattern() {
    std::cout << "\n=== Example: OctoBrain Pattern Storage ===" << std::endl;
    
    // OctoBrain pattern: 8 cognitive functions × 12 temporal phases
    matrix12x8<oct8> pattern;
    
    // Define "creativity" pattern: high activation in CONCEVOIR (0) and TRANSCENDER (7)
    pattern.apply([](phase12 p, oct8 func, oct8& activation) {
        if (func.value() == 0 || func.value() == 7) {
            activation = oct8(7);  // Maximum activation
        } else {
            activation = oct8(0);  // Minimal activation
        }
    });
    
    std::cout << "Creativity pattern:" << std::endl;
    std::cout << "  CONCEVOIR (0): ";
    for (uint8_t p = 0; p < 12; p++) {
        std::cout << static_cast<int>(pattern(phase12(p), oct8(0)).value()) << " ";
    }
    std::cout << std::endl;
    
    std::cout << "  TRANSCENDER (7): ";
    for (uint8_t p = 0; p < 12; p++) {
        std::cout << static_cast<int>(pattern(phase12(p), oct8(7)).value()) << " ";
    }
    std::cout << std::endl;
    
    // Temporal profile for function 0
    auto temporal = temporal_profile(pattern, oct8(0));
    std::cout << "  Temporal consistency: " 
              << (std::all_of(temporal.begin(), temporal.end(), 
                             [](oct8 o) { return o.value() == 7; }) ? "YES" : "NO")
              << std::endl;
}

void example_factory_functions() {
    std::cout << "\n=== Example: Factory Functions ===" << std::endl;
    
    auto m_zeros = zeros<uint8_t>();
    auto m_ones = ones<uint8_t>();
    auto m_identity = identity<uint8_t>();
    
    std::cout << "Zeros sum: " << m_zeros.sum() << std::endl;
    std::cout << "Ones sum: " << m_ones.sum() << std::endl;
    std::cout << "Identity sum: " << m_identity.sum() << std::endl;
    
    // Temporal rotation
    auto m_rotated = rotate_temporal(m_identity, 3);
    std::cout << "Identity rotated by 3 phases" << std::endl;
    
    // Spatial mirroring
    auto m_mirrored = mirror_spatial(m_ones, 'Z');
    std::cout << "Ones mirrored across Z-axis" << std::endl;
}

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   Language O - Integrated Test Suite" << std::endl;
    std::cout << "   oct8 + phase12 + matrix12x8" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // Run tests
    try {
        // phase12 tests
        test_phase12_basic();
        test_phase12_distances();
        test_phase12_temporal();
        test_phase12_edges();
        
        // matrix12x8 tests
        test_matrix_basic();
        test_matrix_operations();
        test_matrix_queries();
        test_matrix_aggregation();
        
        // Integration test
        test_integration();
        
        std::cout << "\n✅ All tests passed!\n" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed: " << e.what() << std::endl;
        return 1;
    }
    
    // Run examples
    example_temporal_simulation();
    example_spatial_pathfinding();
    example_octobrain_pattern();
    example_factory_functions();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "   All demonstrations complete!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}
