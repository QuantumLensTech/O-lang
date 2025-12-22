# O — Universal Multi-State Programming Language

**O is a C++ extension for N-state computing.**

## What is O?

O extends C++ with support for arbitrary N-state types, enabling programming for:
- **Binary systems** (N=2) — Traditional computing
- **Ternary systems** (N=3) — Balanced ternary processors
- **Octovalent systems** (N=8) — 3ODS, 3-qubit quantum
- **Arbitrary N** — Future multi-state hardware

```cpp
#include <o/o.hpp>

O<2> bit = 1;      // Binary
O<3> trit = 2;     // Ternary
O<8> oct = 5;      // Octovalent (3ODS)
O<16> hex = 15;    // Hexadecimal
```

## Quick Start

```cpp
#include <o/o.hpp>
using namespace o;

int main() {
    // Create octovalent value
    O<8> state = 3;
    
    // Arithmetic (modulo 8)
    state = state + O<8>(2);  // state = 5
    state++;                  // state = 6
    
    // Logic operations (min/max)
    O<8> a = 2, b = 5;
    O<8> result = a & b;      // min(2, 5) = 2
    result = a | b;           // max(2, 5) = 5
    result = ~a;              // 8-1-2 = 5
    
    // Ranges
    for (auto s : O_range<8>()) {
        // Iterate through all 8 states
    }
    
    return 0;
}
```

## Features

### ✅ Core Types
- **O<N>** — Universal multi-state type
- **O_array<N, Size>** — Arrays of N-state values
- Type-safe, constexpr-compatible

### ✅ Logic Systems
- **Łukasiewicz logic** (min/max) — Default
- **Product logic** — Multiplicative operations
- **Gödel logic** — Binary-like negation
- Custom truth tables

### ✅ Functional Programming
- **map**, **filter**, **reduce**
- **compose**, **curry**
- **Maybe** monad

### ✅ Hardware Channels
- **OctoBIN** — Binary CPU emulation (always available)
- **OctoQUANT** — Quantum processors (N = 2^k)
- **OctoTOPO** — Topological quantum (native N=8)

### ✅ Quantum Support
- **QuantumState<N>** — Superposition states
- **Quantum gates** — Hadamard, phase, rotation
- **Entanglement** — Multi-qudit states

## File Structure

```
include/o/
├── o.hpp               # Main header (include everything)
├── core.hpp            # O<N> type definition
├── logic.hpp           # Logical operations
├── operators.hpp       # Arithmetic operations
├── ranges.hpp          # Iterators and ranges
├── functional.hpp      # Functional programming
├── channels.hpp        # Hardware abstraction
└── quantum.hpp         # Quantum computing support
```

## Examples

### Binary (N=2)
```cpp
O<2> flag = 0;
flag = ~flag;  // Toggle: 0 → 1
```

### Ternary (N=3)
```cpp
O<3> trit = 0;  // {0, 1, 2}
trit++;         // 0 → 1
trit++;         // 1 → 2
trit++;         // 2 → 0 (cyclic)
```

### Octovalent (N=8) — 3ODS
```cpp
O<8> octant = 3;
O<8> next = octant + O<8>(1);  // 3 + 1 = 4

// Iterate all octants
for (auto o : O_range<8>()) {
    // Process octant o
}
```

### Quantum Computing
```cpp
using namespace o::quantum;

QuantumState<2> qubit(O<2>(0));  // |0⟩
qubit = hadamard(qubit);         // (|0⟩ + |1⟩)/√2
O<2> result = qubit.measure();   // Collapse to 0 or 1
```

## Building

```bash
# Header-only library, just include:
g++ -std=c++17 -I include your_code.cpp -o your_program

# Or with CMake:
cmake -B build -DCMAKE_CXX_STANDARD=17
cmake --build build
```

## Philosophy

**O is minimal.** Like C, it provides:
- A fundamental type: `O<N>`
- Essential operations: arithmetic, logic, ranges
- Extensions are optional

**O is universal.** It works for:
- N=2 (binary) — Compatible with existing hardware
- N=3, 4, 8, 16 — Multi-state processors
- N=2^k — Quantum systems (qubits, qutrits, etc.)

**O is explicit.** No hidden costs:
- Zero-cost abstractions
- Compile-time evaluation (constexpr)
- No runtime overhead vs. hand-coded logic

## Relationship with 3ODS

**O is independent of 3ODS.**

- **O** = Universal language for N-state computing
- **3ODS** = Specific operating system architecture using O with N=8

3ODS is **one application** of O, not the only one. O can be used for:
- Binary systems (N=2)
- Ternary processors (N=3)
- Quantum computing (N=2^k)
- Any N-state hardware

## License

CC BY-NC-SA 4.0

## Author

Jean-Christophe Ané  
December 2025

---

**O: Where binary is just N=2.**
