// O Language — Universal Multi-State Extension of C++
// logic.hpp — Multi-state logic operators
//
// Implements various logic systems for N-state types:
// - Łukasiewicz logic (Min/Max)
// - Product logic
// - Gödel logic
// - Custom logic tables

#pragma once

#include <algorithm>
#include <array>
#include <cmath>

namespace o {

// ============================================================================
// ŁUKASIEWICZ LOGIC (Min/Max)
// ============================================================================

template<size_t N>
struct Lukasiewicz {
    using State = O_state<N>;
    
    static constexpr State AND(State a, State b) noexcept {
        return State(std::min(a.value(), b.value()));
    }
    
    static constexpr State OR(State a, State b) noexcept {
        return State(std::max(a.value(), b.value()));
    }
    
    static constexpr State NOT(State a) noexcept {
        return State(N - 1 - a.value());
    }
    
    static constexpr State IMPLIES(State a, State b) noexcept {
        auto val = std::min(N - 1, N - 1 - a.value() + b.value());
        return State(val);
    }
    
    static constexpr State XOR(State a, State b) noexcept {
        // |a - b|
        auto diff = (a.value() > b.value()) ? 
                    (a.value() - b.value()) : 
                    (b.value() - a.value());
        return State(diff);
    }
    
    static constexpr State NAND(State a, State b) noexcept {
        return NOT(AND(a, b));
    }
    
    static constexpr State NOR(State a, State b) noexcept {
        return NOT(OR(a, b));
    }
    
    static constexpr State XNOR(State a, State b) noexcept {
        return NOT(XOR(a, b));
    }
};

// ============================================================================
// PRODUCT LOGIC (Multiplication/Average)
// ============================================================================

template<size_t N>
struct Product {
    using State = O_state<N>;
    
    static constexpr State AND(State a, State b) noexcept {
        return State((a.value() * b.value()) / (N - 1));
    }
    
    static constexpr State OR(State a, State b) noexcept {
        auto product = (a.value() * b.value()) / (N - 1);
        return State(a.value() + b.value() - product);
    }
    
    static constexpr State NOT(State a) noexcept {
        return State(N - 1 - a.value());
    }
};

// ============================================================================
// GÖDEL LOGIC (Truncated difference)
// ============================================================================

template<size_t N>
struct Godel {
    using State = O_state<N>;
    
    static constexpr State AND(State a, State b) noexcept {
        return State(std::min(a.value(), b.value()));
    }
    
    static constexpr State OR(State a, State b) noexcept {
        return State(std::max(a.value(), b.value()));
    }
    
    static constexpr State NOT(State a) noexcept {
        return State((a.value() == 0) ? (N - 1) : 0);
    }
    
    static constexpr State IMPLIES(State a, State b) noexcept {
        return State((a.value() <= b.value()) ? (N - 1) : b.value());
    }
};

// ============================================================================
// BOOLEAN LOGIC (for O_binary compatibility)
// ============================================================================

template<>
struct Lukasiewicz<2> {
    using State = O_binary;
    
    static constexpr State AND(State a, State b) noexcept {
        return State(a.value() & b.value());
    }
    
    static constexpr State OR(State a, State b) noexcept {
        return State(a.value() | b.value());
    }
    
    static constexpr State NOT(State a) noexcept {
        return State(!a.value());
    }
    
    static constexpr State XOR(State a, State b) noexcept {
        return State(a.value() ^ b.value());
    }
    
    static constexpr State NAND(State a, State b) noexcept {
        return State(!(a.value() & b.value()));
    }
    
    static constexpr State NOR(State a, State b) noexcept {
        return State(!(a.value() | b.value()));
    }
    
    static constexpr State XNOR(State a, State b) noexcept {
        return State(!(a.value() ^ b.value()));
    }
    
    static constexpr State IMPLIES(State a, State b) noexcept {
        return State(!a.value() | b.value());
    }
};

// ============================================================================
// CUSTOM LOGIC TABLE
// ============================================================================

template<size_t N, size_t TableSize = N * N>
struct CustomLogic {
    using State = O_state<N>;
    
    std::array<uint64_t, TableSize> and_table;
    std::array<uint64_t, TableSize> or_table;
    std::array<uint64_t, N> not_table;
    
    constexpr CustomLogic() : and_table{}, or_table{}, not_table{} {}
    
    State AND(State a, State b) const {
        return State(and_table[a.value() * N + b.value()]);
    }
    
    State OR(State a, State b) const {
        return State(or_table[a.value() * N + b.value()]);
    }
    
    State NOT(State a) const {
        return State(not_table[a.value()]);
    }
    
    // Builder pattern for initialization
    constexpr CustomLogic& set_and(uint64_t a, uint64_t b, uint64_t result) {
        and_table[a * N + b] = result % N;
        return *this;
    }
    
    constexpr CustomLogic& set_or(uint64_t a, uint64_t b, uint64_t result) {
        or_table[a * N + b] = result % N;
        return *this;
    }
    
    constexpr CustomLogic& set_not(uint64_t a, uint64_t result) {
        not_table[a] = result % N;
        return *this;
    }
};

// ============================================================================
// LOGIC ADAPTERS (for operator overloading)
// ============================================================================

template<size_t N, typename Logic = Lukasiewicz<N>>
class O_logic_adapter {
public:
    using State = O_state<N>;
    
    static State land(State a, State b) { return Logic::AND(a, b); }
    static State lor(State a, State b) { return Logic::OR(a, b); }
    static State lnot(State a) { return Logic::NOT(a); }
};

// ============================================================================
// MACRO FOR DEFINING CUSTOM LOGIC
// ============================================================================

#define O_DEF_LOGIC(Name, N) \
    struct Name { \
        using State = o::O_state<N>; \
        static constexpr State AND(State a, State b) noexcept; \
        static constexpr State OR(State a, State b) noexcept; \
        static constexpr State NOT(State a) noexcept; \
    }

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Apply logic to arrays
template<size_t N, typename Logic, size_t Size>
constexpr std::array<O_state<N>, Size> 
array_and(const std::array<O_state<N>, Size>& a,
          const std::array<O_state<N>, Size>& b) noexcept {
    std::array<O_state<N>, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = Logic::AND(a[i], b[i]);
    }
    return result;
}

template<size_t N, typename Logic, size_t Size>
constexpr std::array<O_state<N>, Size> 
array_or(const std::array<O_state<N>, Size>& a,
         const std::array<O_state<N>, Size>& b) noexcept {
    std::array<O_state<N>, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = Logic::OR(a[i], b[i]);
    }
    return result;
}

template<size_t N, typename Logic, size_t Size>
constexpr std::array<O_state<N>, Size> 
array_not(const std::array<O_state<N>, Size>& a) noexcept {
    std::array<O_state<N>, Size> result;
    for (size_t i = 0; i < Size; ++i) {
        result[i] = Logic::NOT(a[i]);
    }
    return result;
}

} // namespace o

// End of logic.hpp
