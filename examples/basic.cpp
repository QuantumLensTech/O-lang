// Basic Example — Getting Started with O Language
//
// Demonstrates:
// - Multi-state types
// - Arithmetic operations
// - Logic operations
// - Channels and concurrency

#include <o/core.hpp>
#include <iostream>

using namespace o;

int main() {
    std::cout << "=== O Language — Basic Example ===\n\n";
    
    // ========================================================================
    // 1. MULTI-STATE TYPES
    // ========================================================================
    
    std::cout << "1. Multi-State Types\n";
    std::cout << "--------------------\n";
    
    // Octal (8 states: 0-7)
    O_octal a(5);
    O_octal b(3);
    
    std::cout << "a = " << a.value() << " (octal)\n";
    std::cout << "b = " << b.value() << " (octal)\n\n";
    
    // ========================================================================
    // 2. MODULAR ARITHMETIC
    // ========================================================================
    
    std::cout << "2. Automatic Modular Arithmetic\n";
    std::cout << "--------------------------------\n";
    
    O_octal c = a + b;
    std::cout << a.value() << " + " << b.value() << " = " 
              << c.value() << " (mod 8)\n";
    
    O_octal d = a * b;
    std::cout << a.value() << " × " << b.value() << " = " 
              << d.value() << " (mod 8)\n";
    
    // Increment (cyclic)
    O_octal e(7);
    std::cout << "e = " << e.value() << "\n";
    e++;
    std::cout << "e++ = " << e.value() << " (wraps around)\n\n";
    
    // ========================================================================
    // 3. MULTI-STATE LOGIC
    // ========================================================================
    
    std::cout << "3. Multi-State Logic (Łukasiewicz)\n";
    std::cout << "-----------------------------------\n";
    
    using Logic = Lukasiewicz<8>;
    
    O_octal x(7), y(2);
    
    auto z_and = Logic::AND(x, y);
    auto z_or = Logic::OR(x, y);
    auto z_not = Logic::NOT(x);
    
    std::cout << "AND(" << x.value() << ", " << y.value() << ") = " 
              << z_and.value() << " (min)\n";
    std::cout << "OR(" << x.value() << ", " << y.value() << ") = " 
              << z_or.value() << " (max)\n";
    std::cout << "NOT(" << x.value() << ") = " 
              << z_not.value() << "\n\n";
    
    // ========================================================================
    // 4. CHANNELS (Concurrency)
    // ========================================================================
    
    std::cout << "4. Channels and Concurrency\n";
    std::cout << "----------------------------\n";
    
    O_channel<8> channel;
    
    // Producer process
    auto producer = O_process([&channel]() {
        for (int i = 0; i < 5; ++i) {
            O_octal value(i);
            channel << value;
            std::cout << "Sent: " << value.value() << "\n";
        }
        channel.close();
    });
    
    // Consumer process
    auto consumer = O_process([&channel]() {
        while (!channel.is_closed() || !channel.empty()) {
            try {
                O_octal value;
                channel >> value;
                std::cout << "Received: " << value.value() << "\n";
            } catch (const std::runtime_error&) {
                break;  // Channel closed
            }
        }
    });
    
    // Wait for completion
    producer.join();
    consumer.join();
    
    std::cout << "\n";
    
    // ========================================================================
    // 5. FUNCTIONAL COMPOSITION
    // ========================================================================
    
    std::cout << "5. Functional Composition\n";
    std::cout << "-------------------------\n";
    
    auto double_it = [](O_octal v) { return v * O_octal(2); };
    auto add_one = [](O_octal v) { return v + O_octal(1); };
    
    auto pipeline = O_compose(double_it, add_one);
    
    O_octal input(3);
    O_octal result = pipeline(input);
    
    std::cout << "Pipeline: double → add_one\n";
    std::cout << "Input: " << input.value() << "\n";
    std::cout << "Output: " << result.value() << " ((3×2)+1 mod 8 = 7)\n\n";
    
    // ========================================================================
    // 6. OTHER N-STATE TYPES
    // ========================================================================
    
    std::cout << "6. Other N-State Types\n";
    std::cout << "----------------------\n";
    
    O_binary binary(1);        // 2 states (0-1)
    O_ternary ternary(2);      // 3 states (0-2)
    O_duodec duodec(11);       // 12 states (0-11)
    O_hex hex(15);             // 16 states (0-15)
    
    std::cout << "Binary: " << binary.value() << " / 1\n";
    std::cout << "Ternary: " << ternary.value() << " / 2\n";
    std::cout << "Duodecimal: " << duodec.value() << " / 11\n";
    std::cout << "Hexadecimal: " << hex.value() << " / 15\n\n";
    
    // ========================================================================
    // DONE
    // ========================================================================
    
    std::cout << "=== Example Complete ===\n";
    
    return 0;
}
