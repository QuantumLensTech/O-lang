# O — Universal Multi-State Extension for C++

> **C++ + N-States. Nothing more, nothing less.**

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg" alt="C++17/20"/>
  <img src="https://img.shields.io/badge/license-CC%20BY--NC--SA%204.0-green.svg" alt="License"/>
  <img src="https://img.shields.io/badge/headers-only-orange.svg" alt="Header-only"/>
  <img src="https://img.shields.io/badge/deps-zero-brightgreen.svg" alt="Zero dependencies"/>
</p>

---

## What is O?

**O** adds **one thing** to C++: native multi-state types.

```cpp
// C++ (binary only)
bool flag;           // 2 states

// O (universal)
octo<3> trit;        // 3 states (ternary)
octo<8> octval;      // 8 states (octovalent)
octo<N> custom;      // N states (any N)
```

That's it. No new syntax. No language redesign. No garbage collector.

**Just multi-state types that C++ should have had from the start.**

---

## Why O?

### The Problem

Traditional computing assumes **everything is binary** (0/1). But emerging hardware isn't:

| Hardware | States | C++ Support | O Support |
|----------|--------|-------------|-----------|
| Binary CPUs | 2 | ✅ Native | ✅ Native (N=2) |
| Ternary logic | 3 | ❌ Emulated | ✅ Native (N=3) |
| Quantum (3-qubit) | 8 | ❌ Complex | ✅ Native (N=8) |
| Multi-level memory | 4-16 | ❌ Manual | ✅ Native (N=4,8,16) |

### The Solution

**Stop forcing everything through binary.**

```cpp
#include <o/core.hpp>

octo<8> quantum_state = 5;  // |101⟩ in 3-qubit system
octo<3> ternary_value = 2;  // Ternary logic
octo<4> mlc_cell = 3;       // Multi-level memory
```

---

## Core Philosophy

### Minimalism First

**O does ONE thing well**: multi-state types.

We **deliberately** keep it minimal:
- ✅ Two core headers: `core.hpp` + `logic.hpp`
- ✅ Zero dependencies (C++ standard library only)
- ✅ Header-only (no linking, no compilation)
- ✅ Zero-cost abstractions (constexpr everything)

**Everything else is optional.**

### Relationship to 3ODS

O is **not** the "3ODS language". O is **universal**.

```
O (foundation for ANY multi-state system)
    ↓ used by
3ODS (N=8, octovalent computing)
    ↓ adds
Geometric semantics + Temporal phases
```

**Analogy**: C++ is to Qt what **O is to 3ODS**.

---

## Quick Start

### Installation

```bash
# Clone
git clone https://github.com/YourUsername/O.git
cd O

# That's it. Header-only = no build needed.
```

### Your First Program

```cpp
#include <o/core.hpp>
#include <iostream>

int main() {
    // Create an 8-state value (octovalent)
    o::octo<8> state = 3;
    
    // Operations (modulo N)
    auto result = state + o::octo<8>(2);
    
    std::cout << "3 + 2 (mod 8) = " 
              << (int)result.raw() << "\n";  // 5
    
    return 0;
}
```

### Compile

```bash
g++ -std=c++17 -I include main.cpp -o main
./main
```

**Output**: `3 + 2 (mod 8) = 5`

---

## Core Features (Required)

### 1. Multi-State Types (`o/core.hpp`)

```cpp
#include <o/core.hpp>
using namespace o;

// Basic type
octo<8> value = 5;

// Arrays
octo_array<8, 64> data;

// Matrices
octo_matrix<8, 3, 3> transform;

// Arithmetic (modulo N)
octo<8> a = 7, b = 3;
auto c = a + b;  // 2 (mod 8)

// Conversions
uint8_t binary = to_binary(value);
octo<8> from_bin = from_binary<8>(binary);
```

**Size**: ~600 lines  
**Dependencies**: `<array>`, `<cstdint>`, `<type_traits>`

### 2. Multi-Valued Logic (`o/logic.hpp`)

```cpp
#include <o/logic.hpp>
using namespace o;

octo<8> a = 3, b = 5;

// Łukasiewicz logic (fuzzy)
auto and_result = lukasiewicz_and(a, b);
auto or_result = lukasiewicz_or(a, b);
auto not_result = lukasiewicz_not(a);

// Product logic
auto prod = product_and(a, b);

// Gödel logic
auto godel = godel_and(a, b);
```

**Size**: ~500 lines  
**Dependencies**: `<algorithm>`, `<cmath>`

---

## Optional Extensions

**These are bonus features. Use them if you want. Ignore them if you don't.**

### 1. Advanced Features (`o/advanced.hpp`)

Inspired by D language, but fully optional:

```cpp
#include <o/advanced.hpp>

// Compile-time execution
constexpr octo<8> result = compute_at_compile_time(octo<8>(3));

// Mixins (code reuse)
class MyType : public Serializable<MyType> { };

// Contracts (preconditions/postconditions)
void set(octo<8> v) requires (v.raw() < 8);

// UFCS (uniform function call syntax)
auto y = x.rotate_left(2);  // or rotate_left(x, 2)

// Ranges (functional pipelines)
auto result = data 
    | filter([](auto x) { return x.raw() > 3; })
    | transform([](auto x) { return x * octo<8>(2); });
```

**Size**: ~500 lines  
**Status**: Complete, optional

### 2. Concurrency (`o/channels.hpp`)

CSP-style channels (Go-inspired):

```cpp
#include <o/channels.hpp>

Channel<octo<8>> ch(10);  // Buffered channel

// Producer
ch.send(octo<8>(5));

// Consumer
auto value = ch.receive();
```

**Status**: Planned (Q1 2025)

### 3. Functional Programming (`o/functional.hpp`)

Higher-order functions:

```cpp
#include <o/functional.hpp>

auto data = octo_array<8, 100>();

// Map, filter, reduce
auto result = map(data, [](auto x) { return x * octo<8>(2); });
auto filtered = filter(result, [](auto x) { return x.raw() > 3; });
auto sum = reduce(filtered, octo<8>(0), std::plus<>{});
```

**Status**: Planned (Q1 2025)

### 4. Quantum Computing (`o/quantum.hpp`)

Natural mapping: K qubits → 2^K states

```cpp
#include <o/quantum.hpp>

// 3 qubits = 8 states
Qubit<3> qstate;  // Uses octo<8> internally

// |101⟩ state
qstate.set(5);

// Quantum gates
qstate.hadamard(0);
qstate.cnot(0, 1);
```

**Status**: Planned (Q1 2025)

---

## Design Principles

### 1. Minimalism

**Core = 2 headers.** Everything else is optional.

We resist feature creep. O does **one thing**: multi-state types.

### 2. Zero-Cost Abstractions

```cpp
// This compiles to the same machine code as raw uint8_t
octo<8> x = 3;
octo<8> y = x + octo<8>(2);
```

**No runtime overhead.** All type safety is compile-time.

### 3. Compatibility First

**100% valid C++ is 100% valid O.**

O is a pure extension. No breaking changes. No new compiler.

### 4. Header-Only

No linking. No compilation. Just:

```cpp
#include <o/core.hpp>
```

### 5. Universal, Not Specific

O works for **any** N-state system:
- Binary (N=2)
- Ternary (N=3)
- Octovalent (N=8) — used by 3ODS
- Hexadecimal (N=16)
- Custom (any N from 2 to 256)

**3ODS is one user of O, not the only user.**

---

## Use Cases

### 1. Quantum Computing

```cpp
// 3 qubits = 8 basis states
octo<8> qubit_state = 5;  // |101⟩

// Natural representation
// No complex Hilbert space abstractions
```

**Real hardware**: IBM/Google superconducting qubits (Josephson junctions) with 3-qubit registers naturally map to `octo<8>`. See [examples/quantum/](examples/quantum/) for detailed hardware examples.

### 2. Ternary Logic

```cpp
// True/False/Unknown logic
octo<3> value = 2;  // Unknown

if (value == octo<3>(0)) {
    // False
} else if (value == octo<3>(1)) {
    // True
} else {
    // Unknown (2)
}
```

### 3. Multi-Level Memory

```cpp
// 4-level cell (2 bits per cell)
octo<4> mlc_value = 3;  // 11 in binary

// Direct manipulation without bit masking
```

### 4. 3ODS (Octovalent Computing)

```cpp
// 8 spatial octants
octo<8> octant = 5;  // Octant (+, -, +)

// 3ODS adds geometric meaning on top of O
Vector3 position = octant_to_position(octant);
```

---

## Project Structure

```
O/
├── README.md              ← You are here
├── LICENSE.md             ← CC BY-NC-SA 4.0
│
├── include/o/
│   ├── core.hpp           ← REQUIRED: Multi-state types
│   ├── logic.hpp          ← REQUIRED: Multi-valued logic
│   │
│   └── ext/               ← OPTIONAL: Extensions
│       ├── advanced.hpp   ← D-inspired features
│       ├── channels.hpp   ← CSP concurrency (planned)
│       ├── functional.hpp ← FP primitives (planned)
│       └── quantum.hpp    ← Quantum types (planned)
│
├── examples/
│   ├── basic.cpp          ← Minimal example
│   ├── universal.cpp      ← All N values
│   └── 3ods.cpp           ← How 3ODS uses O
│
├── tests/
│   ├── test_core.cpp      ← 43 tests (core.hpp)
│   ├── test_logic.cpp     ← 37 tests (logic.hpp)
│   └── run_tests.sh       ← Automated testing
│
└── CMakeLists.txt         ← Build system (optional)
```

**Core**: 2 files (~1,100 lines)  
**Extensions**: 4+ files (optional)  
**Tests**: 80+ tests (100% passing)

---

## Comparison with Alternatives

### vs. Raw C++

| Feature | C++ | O |
|---------|-----|---|
| Binary types | ✅ | ✅ |
| Ternary types | ❌ Manual | ✅ Native |
| Octovalent types | ❌ Emulated | ✅ Native |
| Multi-valued logic | ❌ Custom | ✅ Built-in |

### vs. D Language

| Feature | D | O |
|---------|---|---|
| New language | Yes | No (C++ extension) |
| Breaking changes | Many | Zero |
| Adoption barrier | High | Low |
| Multi-state types | No | Yes |
| Ecosystem | Small | C++ ecosystem |

**O learns from D's mistakes**: stay minimal, stay compatible.

### vs. Rust

| Feature | Rust | O |
|---------|------|---|
| Memory safety | ✅ | ❌ (uses C++) |
| Multi-state types | ❌ | ✅ |
| Learning curve | Steep | Shallow (just C++) |

**Different goals**: Rust = safety, O = multi-states.

---

## Roadmap

### Phase 1 (Q4 2024) — ✅ COMPLETE

- [x] Core types (`octo<N>`)
- [x] Multi-valued logic
- [x] Advanced features (optional)
- [x] 80+ unit tests
- [x] Documentation
- [x] Examples

### Phase 2 (Q1 2025) — In Progress

- [ ] `channels.hpp` — CSP concurrency
- [ ] `functional.hpp` — Higher-order functions
- [ ] `quantum.hpp` — Quantum computing types
- [ ] CI/CD (GitHub Actions)
- [ ] Benchmarks

### Phase 3 (Q2-Q3 2025) — O+ Prototyping

- [ ] O+ language specification
- [ ] O+ → O compiler (transpiler)
- [ ] Native octovalent syntax
- [ ] Runtime/VM

### Phase 4 (2030-2035) — O++ Native

- [ ] Hardware backends (topological, quantum)
- [ ] LLVM integration
- [ ] Native execution (no emulation)

---

## Testing

```bash
cd tests
./run_tests.sh
```

**Results** (expected):
```
=== O Language Test Suite ===

[1/2] Building tests...
✓ Build successful

[2/2] Running tests...
[==========] Running 80 tests from 2 test suites.
[  PASSED  ] 80 tests. (27 ms total)

✓ All tests passed!
```

---

## Contributing

### How to Contribute

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing`
3. Commit changes: `git commit -m 'feat: Add amazing feature'`
4. Push: `git push origin feature/amazing`
5. Submit Pull Request

### Guidelines

**For Core** (`core.hpp`, `logic.hpp`):
- Extreme scrutiny
- Must be absolutely minimal
- Must have zero dependencies
- Must have 100% test coverage

**For Extensions** (`ext/*.hpp`):
- More flexible
- Can have dependencies (documented)
- Should have >80% test coverage

### Standards

- C++17 minimum
- Header-only
- Constexpr everywhere possible
- Google Test for unit tests
- Doxygen-style documentation

---

## FAQ

### Is O a new programming language?

**No.** O is a C++ extension. Pure C++ code works unchanged.

### Do I need a special compiler?

**No.** Any C++17 compiler works (GCC, Clang, MSVC).

### What's the runtime overhead?

**Zero.** All type safety is compile-time. The generated machine code is identical to hand-written binary code.

### Why not just use templates?

**You could**, but O provides:
- Standard types (`octo<N>`)
- Standard logic operations
- Conventions and best practices
- Tested, documented, ready to use

### Is this related to 3ODS?

**O is universal.** 3ODS is **one** project that uses O (with N=8). But O works for any N.

Think: C++ vs Qt. O vs 3ODS.

### Can I use O for non-quantum systems?

**Absolutely!** O works for:
- Ternary logic
- Multi-level memory
- Neuromorphic computing
- Any discrete multi-state system

### What about Rust?

**Different goals.**
- Rust = memory safety
- O = multi-state types

You could theoretically port O to Rust, but C++ is fine.

### Why not wait for native hardware?

**Because software leads hardware.**

If nobody programs multi-state systems, nobody will build multi-state hardware.

O lets us explore the algorithms **now**, so when hardware arrives (2030+), we're ready.

---

## License

**CC BY-NC-SA 4.0** — Creative Commons Attribution-NonCommercial-ShareAlike

**You can:**
- ✅ Use for research, education, open-source
- ✅ Modify and redistribute (with attribution)
- ✅ Build on top of O

**You cannot:**
- ❌ Use commercially without permission

**Attribution**: Jean-Christophe Ané, December 2025

---

## Philosophy

### On Minimalism

> "Perfection is achieved, not when there is nothing more to add, but when there is nothing left to take away." — Antoine de Saint-Exupéry

O embraces this. We could add hundreds of features. **We don't.**

Core = 2 headers. That's it.

### On Universality

O is **not** "the 3ODS language" or "the quantum language."

O is a **foundation** for **any** multi-state computing.

3ODS, quantum, ternary, neuromorphic — all welcome.

### On Compatibility

We refuse to break C++.

O is a **pure extension**. 100% backward compatible.

Your C++ code works in O. Always.

### On the Future

Binary computing dominated 1950-2025.

Multi-state computing will dominate 2030-2100.

**O is the bridge.**

---

## Contact

**Author**: Jean-Christophe Ané  
**Project**: O — Universal Multi-State Extension for C++  
**License**: CC BY-NC-SA 4.0  
**Repository**: [GitHub URL]  
**Documentation**: [Docs URL]

---

## Acknowledgments

- **C++ Standard Committee** — For the language
- **D Language** — For inspiration (CTFE, mixins)
- **3ODS Project** — First major user of O
- **Quantum Computing Community** — For motivation

---

**Version**: 1.0  
**Date**: December 21, 2025  
**Status**: Production Ready

---

> **"C++ + N-States. Nothing more, nothing less."**

**🌌 The future is multi-state 💎**
