#ifndef O_QUANTUM_HPP
#define O_QUANTUM_HPP

/**
 * @file quantum.hpp
 * @brief Quantum Computing Support
 * 
 * Quantum operations and state representation for O<N> types.
 * Particularly relevant for N = 2^k (qubit systems).
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <complex>
#include <vector>
#include <cmath>

namespace o {
namespace quantum {

// ═══════════════════════════════════════════════════════════════════════════
// QUANTUM STATE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Quantum state with N basis states
 * 
 * Represents a quantum superposition:
 * |ψ⟩ = α₀|0⟩ + α₁|1⟩ + ... + α_{N-1}|N-1⟩
 */
template<uint8_t N>
class QuantumState {
    std::vector<std::complex<double>> amplitudes_;
    
public:
    QuantumState() : amplitudes_(N, {0.0, 0.0}) {
        amplitudes_[0] = {1.0, 0.0};  // Initialize to |0⟩
    }
    
    /**
     * @brief Create state from classical O<N> value
     */
    explicit QuantumState(O<N> classical) : amplitudes_(N, {0.0, 0.0}) {
        amplitudes_[classical.value()] = {1.0, 0.0};
    }
    
    /**
     * @brief Get amplitude for basis state
     */
    std::complex<double> amplitude(uint8_t state) const {
        return amplitudes_[state];
    }
    
    /**
     * @brief Set amplitude (does not normalize!)
     */
    void set_amplitude(uint8_t state, std::complex<double> amp) {
        amplitudes_[state] = amp;
    }
    
    /**
     * @brief Get probability of measuring state
     */
    double probability(uint8_t state) const {
        auto amp = amplitudes_[state];
        return std::norm(amp);  // |α|²
    }
    
    /**
     * @brief Normalize state
     */
    void normalize() {
        double sum = 0.0;
        for (const auto& amp : amplitudes_) {
            sum += std::norm(amp);
        }
        double norm = std::sqrt(sum);
        for (auto& amp : amplitudes_) {
            amp /= norm;
        }
    }
    
    /**
     * @brief Measure state (collapses to classical O<N>)
     */
    O<N> measure() {
        // Simple measurement simulation (uniform random)
        double r = static_cast<double>(rand()) / RAND_MAX;
        double cumulative = 0.0;
        
        for (uint8_t i = 0; i < N; ++i) {
            cumulative += probability(i);
            if (r <= cumulative) {
                // Collapse to this state
                amplitudes_.assign(N, {0.0, 0.0});
                amplitudes_[i] = {1.0, 0.0};
                return O<N>(i);
            }
        }
        
        return O<N>(N - 1);  // Fallback
    }
    
    /**
     * @brief Check if state is classical (only one basis state)
     */
    bool is_classical() const {
        int count = 0;
        for (const auto& amp : amplitudes_) {
            if (std::norm(amp) > 1e-10) ++count;
        }
        return count == 1;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// QUANTUM GATES
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Hadamard-like gate (creates superposition)
 * 
 * For N=2 (qubit), this is the standard Hadamard gate.
 * For general N, it creates equal superposition.
 */
template<uint8_t N>
QuantumState<N> hadamard(const QuantumState<N>& state) {
    QuantumState<N> result;
    std::complex<double> coeff = {1.0 / std::sqrt(N), 0.0};
    
    for (uint8_t i = 0; i < N; ++i) {
        result.set_amplitude(i, coeff);
    }
    
    return result;
}

/**
 * @brief Phase gate
 * 
 * Applies phase rotation to each basis state.
 */
template<uint8_t N>
QuantumState<N> phase_gate(const QuantumState<N>& state, double phase) {
    QuantumState<N> result = state;
    std::complex<double> phase_factor = std::polar(1.0, phase);
    
    for (uint8_t i = 0; i < N; ++i) {
        result.set_amplitude(i, state.amplitude(i) * phase_factor);
    }
    
    return result;
}

/**
 * @brief Rotation gate (generalized Pauli rotations)
 */
template<uint8_t N>
QuantumState<N> rotation(const QuantumState<N>& state, double angle) {
    QuantumState<N> result;
    
    for (uint8_t i = 0; i < N; ++i) {
        double phase = (2.0 * M_PI * i * angle) / N;
        std::complex<double> rot = std::polar(1.0, phase);
        result.set_amplitude(i, state.amplitude(i) * rot);
    }
    
    return result;
}

/**
 * @brief NOT gate (state inversion)
 * 
 * |i⟩ → |N-1-i⟩
 */
template<uint8_t N>
QuantumState<N> quantum_not(const QuantumState<N>& state) {
    QuantumState<N> result;
    
    for (uint8_t i = 0; i < N; ++i) {
        uint8_t flipped = N - 1 - i;
        result.set_amplitude(flipped, state.amplitude(i));
    }
    
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
// QUANTUM ENTANGLEMENT
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Two-qudit (N-state) entangled state
 * 
 * For N=2, this can represent Bell states.
 */
template<uint8_t N>
class EntangledPair {
    std::vector<std::complex<double>> amplitudes_;  // N² amplitudes
    
public:
    EntangledPair() : amplitudes_(N * N, {0.0, 0.0}) {
        amplitudes_[0] = {1.0, 0.0};  // |00⟩
    }
    
    /**
     * @brief Get amplitude for joint state (i, j)
     */
    std::complex<double> amplitude(uint8_t i, uint8_t j) const {
        return amplitudes_[i * N + j];
    }
    
    /**
     * @brief Set amplitude for joint state
     */
    void set_amplitude(uint8_t i, uint8_t j, std::complex<double> amp) {
        amplitudes_[i * N + j] = amp;
    }
    
    /**
     * @brief Create maximally entangled state (Bell-like)
     */
    static EntangledPair<N> bell_like() {
        EntangledPair<N> state;
        std::complex<double> coeff = {1.0 / std::sqrt(N), 0.0};
        
        // |Φ⁺⟩ = (|00⟩ + |11⟩ + ... + |N-1,N-1⟩) / √N
        for (uint8_t i = 0; i < N; ++i) {
            state.set_amplitude(i, i, coeff);
        }
        
        return state;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// QUANTUM MEASUREMENT
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Measure in computational basis
 */
template<uint8_t N>
O<N> measure_computational(QuantumState<N>& state) {
    return state.measure();
}

/**
 * @brief Measure in Hadamard basis
 */
template<uint8_t N>
O<N> measure_hadamard(QuantumState<N>& state) {
    auto rotated = hadamard(state);
    return rotated.measure();
}

} // namespace quantum
} // namespace o

#endif // O_QUANTUM_HPP
