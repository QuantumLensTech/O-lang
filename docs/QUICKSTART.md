# O Language — Quick Start Guide

Get started with multi-state programming in 5 minutes.

---

## Installation

### Header-Only (Recommended)

```bash
# Clone repository
git clone https://github.com/your-org/O-Language.git
cd O-Language

# Copy headers to your include path
sudo cp -r include/o /usr/local/include/

# Or just add to your project
cp -r include/o your_project/include/
```

### CMake Integration

```cmake
# In your CMakeLists.txt
find_package(O REQUIRED)
target_link_libraries(your_target PRIVATE O::o)
```

---

## Hello World

Create `hello.cpp`:

```cpp
#include <o/core.hpp>
#include <iostream>

int main() {
    // Octovalent (8-state) value
    o::octo<8> state = o::octo<8>(5);
    
    std::cout << "State: " << state.value() << std::endl;
    
    return 0;
}
```

Compile:

```bash
g++ -std=c++17 -I include hello.cpp -o hello
./hello
# Output: State: 5
```

---

## Basic Concepts

### 1. Multi-State Types

```cpp
#include <o/core.hpp>

o::octo<2> binary;     // 2 states: {0, 1}
o::octo<3> ternary;    // 3 states: {0, 1, 2}
o::octo<8> octovalent; // 8 states: {0-7}
o::octo<16> hex;       // 16 states: {0-15}
```

### 2. Modular Arithmetic

```cpp
o::octo<8> a = o::octo<8>(5);
o::octo<8> b = o::octo<8>(6);
o::octo<8> sum = a + b;  // 5 + 6 = 11 mod 8 = 3
```

### 3. Logical Operations

```cpp
o::octo<4> x = o::octo<4>(1);
o::octo<4> y = o::octo<4>(3);

auto and_result = x & y;  // min(1, 3) = 1
auto or_result = x | y;   // max(1, 3) = 3
auto not_result = ~x;     // (4-1) - 1 = 2
```

### 4. Iteration

```cpp
#include <o/ranges.hpp>

for (auto state : o::octo_range<4>()) {
    // Iterates: 0, 1, 2, 3
    std::cout << state.value() << " ";
}
```

### 5. Arrays

```cpp
o::octo_array<8, 5> arr = {0, 2, 4, 6, 7};

for (size_t i = 0; i < arr.size(); ++i) {
    std::cout << arr[i].value() << " ";
}
```

---

## Complete Example

```cpp
#include <o/core.hpp>
#include <o/operators.hpp>
#include <o/ranges.hpp>
#include <iostream>

int main() {
    // Create ternary values
    o::octo<3> a = o::octo<3>(1);
    o::octo<3> b = o::octo<3>(2);
    
    // Arithmetic
    std::cout << "Sum: " << (a + b).value() << std::endl;  // 0 (mod 3)
    
    // Logical
    std::cout << "AND: " << (a & b).value() << std::endl;  // 1 (min)
    std::cout << "OR: " << (a | b).value() << std::endl;   // 2 (max)
    
    // Iteration
    std::cout << "All ternary states: ";
    for (auto s : o::octo_range<3>()) {
        std::cout << s.value() << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

**Output**:
```
Sum: 0
AND: 1
OR: 2
All ternary states: 0 1 2
```

---

## Next Steps

1. **Read the spec**: `docs/SPEC.md` for formal definition
2. **Explore examples**: `examples/` directory
3. **Learn integration**: `docs/INTEGRATION.md` for using with 3ODS
4. **Check API reference**: `docs/REFERENCE.md` (coming soon)

---

## Common Patterns

### Pattern 1: State Machine

```cpp
o::octo<4> current_state = o::octo<4>(0);

while (true) {
    switch (current_state.value()) {
        case 0: /* IDLE */; current_state = o::octo<4>(1); break;
        case 1: /* RUNNING */; current_state = o::octo<4>(2); break;
        case 2: /* STOPPING */; current_state = o::octo<4>(3); break;
        case 3: /* STOPPED */; current_state = o::octo<4>(0); break;
    }
}
```

### Pattern 2: Lookup Table

```cpp
std::array<int, 8> lut = {10, 20, 30, 40, 50, 60, 70, 80};

o::octo<8> index = o::octo<8>(5);
int result = lut[index.value()];  // 60
```

### Pattern 3: Fold/Reduce

```cpp
#include <o/ranges.hpp>

int sum = o::octo_fold<8>(0, [](int acc, auto state) {
    return acc + state.value();
});
// sum = 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28
```

---

## Troubleshooting

### Problem: "undefined reference to octo<N>"

**Solution**: O is header-only. Make sure you're including the headers:
```cpp
#include <o/core.hpp>
```

### Problem: "N must be in [2, 256]"

**Solution**: Check your N value:
```cpp
o::octo<1> invalid;   // ✗ N=1 not allowed
o::octo<300> invalid; // ✗ N=300 too large
o::octo<8> valid;     // ✓ N=8 is valid
```

### Problem: Modulo behavior unexpected

**Solution**: Remember all operations are modular:
```cpp
o::octo<3> x = o::octo<3>(2);
o::octo<3> y = x + x;  // 2 + 2 = 4 mod 3 = 1 (not 4!)
```

---

## Help & Support

- **Documentation**: Full docs in `docs/` directory
- **Examples**: Working examples in `examples/` directory
- **Issues**: Report bugs on GitHub
- **Email**: quantumlens.research@gmail.com

---

**You're ready to start multi-state programming with O!**
