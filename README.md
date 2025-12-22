# O — Universal Multi-State Programming Language

<p align="center">
  <strong>The Foundation for Multi-State Computing</strong>
</p>

<p align="center">
  <a href="#what-is-o">What is O?</a> •
  <a href="#quick-start">Quick Start</a> •
  <a href="#features">Features</a> •
  <a href="#examples">Examples</a> •
  <a href="#roadmap">Roadmap</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg" alt="C++17/20"/>
  <img src="https://img.shields.io/badge/license-CC%20BY--NC--SA%204.0-green.svg" alt="License"/>
  <img src="https://img.shields.io/badge/status-alpha-orange.svg" alt="Status"/>
</p>

---

## What is O?

**O** is a universal multi-state extension of C++ that enables programming for emerging hardware architectures beyond binary computing. It provides:

- **N-state types**: Binary (2), ternary (3), octovalent (8), quantum (superposition), etc.
- **Generic operations**: Arithmetic, logic, control flow that scale across state spaces
- **Zero-cost abstractions**: Compile-time polymorphism, no runtime overhead
- **Hardware agnostic**: Write once, run on binary, ternary, quantum, or neuromorphic hardware

```cpp
#include <o/core.hpp>

// Binary (N=2) - works on all hardware
octo<2> flag = 1;

// Ternary (N=3) - for balanced ternary processors
octo<3> trit = {0, 1, 2};

// Octovalent (N=8) - for 3-qubit quantum systems or 3ODS
octo<8> octval = 5;  // One of 8 discrete states

// Arbitrary N-states
octo<16> hexval;     // 16-state system
octo<256> byte;      // Full byte range
```

### The O Philosophy

**Binary computing is a special case, not the universal solution.**

O recognizes that:
- Quantum systems naturally have 2^n states (n qubits)
- Multi-level memory cells store 3, 4, or more states per cell
- Neuromorphic hardware uses variable-state neurons
- Topological quantum computers operate on non-binary state spaces

**O provides the language infrastructure to program these systems directly**, without forcing everything through a binary abstraction layer.

---

## What is O NOT?

❌ **Not a competitor to C++** — O **extends** C++ with multi-state primitives  
❌ **Not tied to 3ODS** — 3ODS is **one application** of O (using N=8)  
❌ **Not binary-only** — O supports **any N ≥ 2**  
❌ **Not theoretical** — O works on current binary hardware (via emulation)

**Analogy**: 
- **C++** is to binary computing what **O** is to multi-state computing
- **Qt/Boost** is to C++ what **3ODS** is to O (specialized library on top)

---

## Quick Start

### Installation

```bash
git clone https://github.com/your-org/O-Language.git
cd O-Language

# Header-only library, just include it
cp -r include/o /usr/local/include/
```

### Hello Multi-State World

```cpp
#include <o/core.hpp>
#include <iostream>

int main() {
    // Ternary logic (3 states: 0, 1, 2)
    octo<3> trit_a = 1;
    octo<3> trit_b = 2;
    
    // Addition wraps around in 3-state space
    octo<3> result = trit_a + trit_b;  // 1 + 2 = 0 (mod 3)
    
    std::cout << "In ternary: 1 + 2 = " 
              << result.value() << std::endl;
    
    // Octovalent logic (8 states: 0-7)
    octo<8> oct_a = 5;
    octo<8> oct_b = 6;
    
    octo<8> oct_result = oct_a + oct_b;  // 5 + 6 = 3 (mod 8)
    
    std::cout << "In octovalent: 5 + 6 = " 
              << oct_result.value() << std::endl;
    
    return 0;
}
```

**Output**:
```
In ternary: 1 + 2 = 0
In octovalent: 5 + 6 = 3
```

---

## Core Features

### 1. Universal Multi-State Type

```cpp
template<uint8_t N>
class octo {
    static_assert(N >= 2 && N <= 256);
    
    // States: [0, N-1]
    uint8_t value() const;
    
    // Full operator support
    octo operator+(octo other) const;  // Modular arithmetic
    octo operator-(octo other) const;
    octo operator*(octo other) const;
    octo operator/(octo other) const;
    
    // Comparison
    bool operator==(octo other) const;
    bool operator<(octo other) const;
};
```

### 2. Arrays & Matrices

```cpp
// Array of N-state values
octo_array<3, 10> ternary_array;  // 10 ternary values

// Matrix operations
octo_matrix<8, 4, 4> octovalent_matrix;
```

### 3. Generic Control Flow

```cpp
// Multi-state conditional
o_switch<8>(octval) {
    o_case(0): /* state 0 */; break;
    o_case(1): /* state 1 */; break;
    // ... cases 2-6 ...
    o_case(7): /* state 7 */; break;
}

// Range-based iteration
for (auto state : octo_range<3>()) {
    // Iterates 0, 1, 2
}
```

### 4. Compile-Time Validation

```cpp
// These compile
octo<8> valid = 5;       // ✓ 5 is in [0,7]
octo<3> trit = 2;        // ✓ 2 is in [0,2]

// These don't compile
octo<8> invalid = 9;     // ✗ Compile error: 9 > 7
octo<3> overflow = 5;    // ✗ Compile error: 5 > 2
```

---

## Examples

### Quantum State Simulation (3 qubits = 8 states)

```cpp
#include <o/quantum.hpp>

// 3-qubit quantum register
quantum_register<3> qubits;  // Internally: octo<8>

// Apply Hadamard gate
qubits.hadamard(0);  // Superposition on qubit 0

// Measure
octo<8> result = qubits.measure();  // Collapse to one of 8 basis states
```

### Ternary Logic Processor

```cpp
#include <o/logic.hpp>

// Balanced ternary: {-1, 0, +1} mapped to {0, 1, 2}
octo<3> trit_not(octo<3> x) {
    // NOT: 0→2, 1→1, 2→0
    return octo<3>(2 - x.value());
}

octo<3> trit_and(octo<3> a, octo<3> b) {
    return octo<3>(std::min(a.value(), b.value()));
}
```

### 3ODS Integration (N=8 specialization)

```cpp
#include <o/core.hpp>
#include <3ods/octant.hpp>  // 3ODS library on top of O

// O provides the foundation (octo<8>)
octo<8> state = 5;

// 3ODS adds geometric semantics
Octant octant(state);  // Converts 5 → spatial position (+,-,+)
```

---

## Architecture

### The O Stack

```
┌───────────────────────────────────┐
│  User Code (C++ with O types)    │  <- You write this
├───────────────────────────────────┤
│  O Standard Library               │  <- o/core.hpp, o/operators.hpp
│  (Templates & Algorithms)         │
├───────────────────────────────────┤
│  O Compiler Extensions            │  <- Validation, optimizations
│  (Compile-time checks)            │
├───────────────────────────────────┤
│  C++ Compiler (C++17/20)          │  <- GCC, Clang, MSVC
├───────────────────────────────────┤
│  Backend (Binary/Quantum/Custom)  │  <- Hardware abstraction
└───────────────────────────────────┘
```

### How O Relates to Other Systems

```
O Language (Universal)
    ├── Specialization: 3ODS (N=8, geometric semantics)
    ├── Specialization: Ternary Computing (N=3)
    ├── Specialization: Quantum (N=2^n)
    └── Specialization: Custom (any N)
```

**Key Principle**: O is the **foundation layer**. Systems like 3ODS **build on top** of O, adding domain-specific semantics (geometry, time, ethics) while O provides the multi-state substrate.

---

## Project Structure

```
O-Language/
├── include/
│   └── o/
│       ├── core.hpp           # Fundamental types (octo<N>)
│       ├── operators.hpp      # Arithmetic, logic operators
│       ├── control.hpp        # Multi-state control flow
│       ├── ranges.hpp         # Iteration utilities
│       ├── quantum.hpp        # Quantum computing helpers
│       └── meta.hpp           # Compile-time utilities
├── docs/
│   ├── SPEC.md               # Formal language specification
│   ├── INTEGRATION.md        # How O relates to 3ODS
│   ├── QUICKSTART.md         # Getting started guide
│   └── REFERENCE.md          # Complete API reference
├── tests/
│   ├── test_core.cpp         # Core type tests
│   ├── test_operators.cpp    # Operator tests
│   └── test_quantum.cpp      # Quantum simulation tests
├── examples/
│   ├── ternary_logic.cpp     # Balanced ternary example
│   ├── quantum_3qubit.cpp    # 3-qubit simulation
│   └── 3ods_integration.cpp  # Using O with 3ODS
└── CMakeLists.txt
```

---

## Language Specification

### Type System

| Type | Description | Valid Range |
|------|-------------|-------------|
| `octo<N>` | N-state scalar | [0, N-1] |
| `octo_array<N,M>` | Array of M N-state values | - |
| `octo_matrix<N,R,C>` | R×C matrix of N-state values | - |

### Operations

| Category | Operations | Notes |
|----------|------------|-------|
| Arithmetic | `+`, `-`, `*`, `/`, `%` | Modular arithmetic (mod N) |
| Logical | `&`, `|`, `^`, `~` | N-valued logic |
| Comparison | `==`, `!=`, `<`, `>`, `<=`, `>=` | Total ordering |
| Assignment | `=`, `+=`, `-=`, ... | Standard semantics |

### Control Flow

```cpp
// Multi-state switch
o_switch<N>(value) { /* ... */ }

// Conditional
if (octo_all_equal(a, b, c)) { /* ... */ }

// Loops
for (auto s : octo_range<N>()) { /* ... */ }
```

---

## Roadmap

### Phase 1: Foundation (Current)
- ✅ Core type system (`octo<N>`)
- ✅ Basic operators
- ✅ Compile-time validation
- ✅ Header-only library

### Phase 2: Expanded Library (Q1 2025)
- [ ] `o/ranges.hpp` — Range utilities
- [ ] `o/quantum.hpp` — Quantum helpers
- [ ] `o/concurrent.hpp` — Multi-state threading
- [ ] Comprehensive test suite

### Phase 3: Compiler Integration (Q2-Q3 2025)
- [ ] Clang plugin for enhanced diagnostics
- [ ] Custom optimizations for multi-state operations
- [ ] LLVM backend integration

### Phase 4: Hardware Backends (Q4 2025+)
- [ ] Ternary processor support
- [ ] Quantum computer interfaces (Qiskit, Cirq)
- [ ] Neuromorphic hardware backends

---

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Areas seeking help:
- Test coverage expansion
- Additional backend implementations
- Documentation improvements
- Example applications

---

## License

**CC BY-NC-SA 4.0** — Creative Commons Attribution-NonCommercial-ShareAlike 4.0

- ✅ Share and adapt for research/education
- ✅ Attribution required
- ❌ No commercial use without permission

---

## Citation

```bibtex
@software{o_language_2025,
  author = {Ané, Jean-Christophe},
  title = {O: Universal Multi-State Programming Language},
  year = {2025},
  url = {https://github.com/your-org/O-Language}
}
```

---

## Contact

**Jean-Christophe Ané** — Creator  
📧 quantumlens.research@gmail.com  
🐙 [@QuantumLensTech](https://github.com/QuantumLensTech)

---

## Related Projects

- **3ODS** — Three-Dimensional Octovalent Duodecavalent System (uses O with N=8)
- **QuantumLENS** — Scientific visualization environment (uses O quantum module)
- **OctoBrain** — Octovalent AI system (uses O for multi-state neural networks)

---

<p align="center">
  <em>"Binary is a special case. O is universal."</em>
</p>

<p align="center">
  ⭐ Star if interested in multi-state computing
</p>

---

**© 2025 Jean-Christophe Ané • CC BY-NC-SA 4.0**
