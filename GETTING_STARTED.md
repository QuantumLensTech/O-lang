# Getting Started with O

**Quick start guide for the O Language**

---

## Installation

### Prerequisites

- **C++17 compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.14+** (for building examples and tests)

### Option 1: Header-Only (Recommended)

1. Clone the repository:
```bash
git clone https://github.com/OLanguage/O.git
cd O
```

2. Include in your project:
```cpp
#include "O/include/o/core.hpp"
```

3. Compile:
```bash
g++ -std=c++17 -I/path/to/O/include your_code.cpp -o program
```

### Option 2: CMake Integration

Add to your `CMakeLists.txt`:

```cmake
include(FetchContent)
FetchContent_Declare(
    OLanguage
    GIT_REPOSITORY https://github.com/OLanguage/O.git
    GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(OLanguage)

target_link_libraries(your_target PRIVATE o)
```

---

## Basic Concepts

### 1. Multi-State Types

Instead of binary (0/1), O provides N-state types:

```cpp
#include <o/core.hpp>
using namespace o;

// 8-state octal type (values 0-7)
O_octal value(5);

// Other common types
O_binary b(1);      // 2 states (0-1)
O_ternary t(2);     // 3 states (0-2)
O_duodec d(11);     // 12 states (0-11)
O_hex h(15);        // 16 states (0-15)
```

### 2. Modular Arithmetic (Automatic)

All arithmetic is modular by default:

```cpp
O_octal a(5);
O_octal b(4);
O_octal c = a + b;  // = 1 (because (5+4) % 8 = 1)

a++;  // = 6
a++;  // = 7
a++;  // = 0 (wraps around)
```

### 3. Multi-State Logic

Replace boolean AND/OR/NOT with multi-valued logic:

```cpp
using Logic = Lukasiewicz<8>;  // Min/Max logic

O_octal x(7), y(3);

auto z1 = Logic::AND(x, y);  // min(7, 3) = 3
auto z2 = Logic::OR(x, y);   // max(7, 3) = 7
auto z3 = Logic::NOT(x);     // 7 - 7 = 0
```

### 4. Concurrency with Channels

CSP-style channels for thread-safe communication:

```cpp
O_channel<8> ch;

// Producer
O_process([&]() {
    ch << O_octal(5);
    ch.close();
});

// Consumer
O_process([&]() {
    O_octal value;
    ch >> value;
    std::cout << value.value() << "\n";
});

O_join_all();  // Wait for completion
```

### 5. Functional Composition

```cpp
auto double_it = [](O_octal x) { return x * O_octal(2); };
auto add_one = [](O_octal x) { return x + O_octal(1); };

auto pipeline = O_compose(double_it, add_one);

O_octal result = pipeline(O_octal(3));  // (3*2)+1 = 7
```

---

## Complete Example

```cpp
#include <o/core.hpp>
#include <iostream>

using namespace o;

int main() {
    // Create 8-state values
    O_octal a(5);
    O_octal b(3);
    
    // Automatic modular arithmetic
    O_octal sum = a + b;  // = 0 (mod 8)
    
    std::cout << "Sum: " << sum.value() << "\n";
    
    // Multi-state logic
    using Logic = Lukasiewicz<8>;
    auto logical_and = Logic::AND(a, b);
    
    std::cout << "AND: " << logical_and.value() << "\n";
    
    // Concurrency
    O_channel<8> ch;
    
    O_process([&]() {
        ch << O_octal(7);
    });
    
    O_process([&]() {
        O_octal received;
        ch >> received;
        std::cout << "Received: " << received.value() << "\n";
    });
    
    O_join_all();
    
    return 0;
}
```

Compile and run:
```bash
g++ -std=c++17 -I/path/to/O/include example.cpp -o example
./example
```

---

## Building Examples and Tests

```bash
mkdir build && cd build
cmake ..
make

# Run examples
./example_basic

# Run tests
ctest
```

---

## IDE Integration

### Visual Studio Code

Add to `.vscode/c_cpp_properties.json`:

```json
{
    "configurations": [{
        "name": "Linux",
        "includePath": [
            "${workspaceFolder}/**",
            "/path/to/O/include"
        ],
        "cppStandard": "c++17"
    }]
}
```

### CLion

Right-click on `O/include` → Mark Directory as → Library Files

---

## Next Steps

- Read [Language Guide](LANGUAGE_GUIDE.md) for in-depth concepts
- Check [API Reference](API_REFERENCE.md) for complete API
- Explore [examples/](../examples/) directory
- Join [GitHub Discussions](https://github.com/OLanguage/O/discussions)

---

## Common Questions

### Q: Do I need to modify my compiler?
**A:** No! O is a header-only library that works with standard C++17/20 compilers.

### Q: What's the performance overhead?
**A:** Zero. O types compile to standard `uint8_t`/`uint64_t` with modulo operations.

### Q: Can I mix O types with standard C++?
**A:** Yes! O types are fully compatible with STL containers and algorithms.

### Q: Is O production-ready?
**A:** The core types are stable. Channels and runtime are in beta. See [STATUS.md](STATUS.md).

---

**Need help?** Open an issue on [GitHub](https://github.com/OLanguage/O/issues)

**© 2025 Jean-Christophe Ané — CC BY-NC-SA 4.0**
