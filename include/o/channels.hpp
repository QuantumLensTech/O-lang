#ifndef O_CHANNELS_HPP
#define O_CHANNELS_HPP

/**
 * @file channels.hpp
 * @brief Hardware Channel Abstractions
 * 
 * Defines hardware-agnostic channels for different computing backends:
 * - Binary (OctoBIN)
 * - Quantum (OctoQUANT)
 * - Topological (OctoTOPO)
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

#include "core.hpp"
#include <string>
#include <memory>

namespace o {

// ═══════════════════════════════════════════════════════════════════════════
// CHANNEL INTERFACE
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Abstract channel interface
 * 
 * All hardware backends must implement this interface.
 */
template<uint8_t N>
class Channel {
public:
    virtual ~Channel() = default;
    
    /**
     * @brief Execute operation on channel
     */
    virtual O<N> execute(const std::string& operation, O<N> input) = 0;
    
    /**
     * @brief Execute operation on array
     */
    virtual void execute_array(const std::string& operation, 
                               O<N>* data, 
                               size_t size) = 0;
    
    /**
     * @brief Get channel name
     */
    virtual std::string name() const = 0;
    
    /**
     * @brief Check if channel is available
     */
    virtual bool is_available() const = 0;
    
    /**
     * @brief Initialize channel
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief Shutdown channel
     */
    virtual void shutdown() = 0;
};

// ═══════════════════════════════════════════════════════════════════════════
// BINARY CHANNEL (OctoBIN)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Binary CPU emulation channel
 * 
 * Emulates N-state operations on traditional binary hardware.
 */
template<uint8_t N>
class BinaryChannel : public Channel<N> {
public:
    O<N> execute(const std::string& operation, O<N> input) override {
        // Emulate operation using binary CPU
        if (operation == "NOT") return ~input;
        if (operation == "INC") return ++input;
        if (operation == "DEC") return --input;
        return input;
    }
    
    void execute_array(const std::string& operation, O<N>* data, size_t size) override {
        for (size_t i = 0; i < size; ++i) {
            data[i] = execute(operation, data[i]);
        }
    }
    
    std::string name() const override { return "OctoBIN"; }
    bool is_available() const override { return true; }  // Always available
    bool initialize() override { return true; }
    void shutdown() override {}
};

// ═══════════════════════════════════════════════════════════════════════════
// QUANTUM CHANNEL (OctoQUANT)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Quantum processor channel
 * 
 * Maps N-state operations to quantum circuits.
 * Requires N = 2^k (power of 2) for qubit encoding.
 */
template<uint8_t N>
class QuantumChannel : public Channel<N> {
    static_assert((N & (N - 1)) == 0, "N must be power of 2 for quantum channel");
    
    bool initialized_ = false;
    
public:
    O<N> execute(const std::string& operation, O<N> input) override {
        // TODO: Map to quantum gates
        return input;
    }
    
    void execute_array(const std::string& operation, O<N>* data, size_t size) override {
        // TODO: Quantum circuit compilation
    }
    
    std::string name() const override { return "OctoQUANT"; }
    bool is_available() const override { return initialized_; }
    
    bool initialize() override {
        // TODO: Connect to quantum hardware
        initialized_ = false;  // Not yet implemented
        return initialized_;
    }
    
    void shutdown() override {
        initialized_ = false;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// TOPOLOGICAL CHANNEL (OctoTOPO)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Topological quantum processor channel
 * 
 * Native octovalent execution on topological hardware.
 * Ideal for N=8 (3-qubit topological systems).
 */
template<uint8_t N>
class TopologicalChannel : public Channel<N> {
    bool initialized_ = false;
    
public:
    O<N> execute(const std::string& operation, O<N> input) override {
        // TODO: Map to topological braids
        return input;
    }
    
    void execute_array(const std::string& operation, O<N>* data, size_t size) override {
        // TODO: Braid sequence compilation
    }
    
    std::string name() const override { return "OctoTOPO"; }
    bool is_available() const override { return initialized_; }
    
    bool initialize() override {
        // TODO: Connect to topological hardware
        initialized_ = false;  // Not yet implemented
        return initialized_;
    }
    
    void shutdown() override {
        initialized_ = false;
    }
};

// ═══════════════════════════════════════════════════════════════════════════
// CHANNEL MANAGER
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Manages available channels and selects optimal one
 */
template<uint8_t N>
class ChannelManager {
    std::unique_ptr<Channel<N>> current_channel_;
    
public:
    ChannelManager() {
        // Try channels in order of preference
        auto topo = std::make_unique<TopologicalChannel<N>>();
        if (topo->initialize()) {
            current_channel_ = std::move(topo);
            return;
        }
        
        auto quant = std::make_unique<QuantumChannel<N>>();
        if (quant->initialize()) {
            current_channel_ = std::move(quant);
            return;
        }
        
        // Fallback to binary emulation
        current_channel_ = std::make_unique<BinaryChannel<N>>();
        current_channel_->initialize();
    }
    
    Channel<N>* get() { return current_channel_.get(); }
    
    std::string active_channel() const {
        return current_channel_ ? current_channel_->name() : "None";
    }
};

} // namespace o

#endif // O_CHANNELS_HPP
