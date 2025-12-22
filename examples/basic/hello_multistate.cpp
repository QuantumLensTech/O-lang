#include <o/core.hpp>
#include <o/operators.hpp>
#include <o/ranges.hpp>
#include <iostream>

using namespace o;

int main() {
    std::cout << "=== O Language - Basic Examples ===" << std::endl << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 1: Binary (N=2)
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "1. Binary (N=2)" << std::endl;
    octo<2> flag = octo<2>(1);
    std::cout << "   flag = " << static_cast<int>(flag.value()) << std::endl;
    std::cout << "   ~flag = " << static_cast<int>((~flag).value()) << std::endl;
    std::cout << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 2: Ternary (N=3)
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "2. Ternary (N=3)" << std::endl;
    octo<3> trit_a = octo<3>(1);
    octo<3> trit_b = octo<3>(2);
    octo<3> trit_sum = trit_a + trit_b;
    
    std::cout << "   " << static_cast<int>(trit_a.value()) 
              << " + " << static_cast<int>(trit_b.value()) 
              << " = " << static_cast<int>(trit_sum.value()) 
              << " (mod 3)" << std::endl;
    std::cout << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 3: Octovalent (N=8)
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "3. Octovalent (N=8)" << std::endl;
    octo<8> oct_a = octo<8>(5);
    octo<8> oct_b = octo<8>(6);
    octo<8> oct_sum = oct_a + oct_b;
    
    std::cout << "   " << static_cast<int>(oct_a.value()) 
              << " + " << static_cast<int>(oct_b.value()) 
              << " = " << static_cast<int>(oct_sum.value()) 
              << " (mod 8)" << std::endl;
    std::cout << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 4: Iteration over all states
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "4. All ternary states:" << std::endl;
    std::cout << "   ";
    for (auto state : octo_range<3>()) {
        std::cout << static_cast<int>(state.value()) << " ";
    }
    std::cout << std::endl << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 5: Logical operations (N=4)
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "5. Logical operations (N=4)" << std::endl;
    octo<4> a = octo<4>(1);
    octo<4> b = octo<4>(3);
    
    std::cout << "   a = " << static_cast<int>(a.value()) << std::endl;
    std::cout << "   b = " << static_cast<int>(b.value()) << std::endl;
    std::cout << "   a & b (min) = " << static_cast<int>((a & b).value()) << std::endl;
    std::cout << "   a | b (max) = " << static_cast<int>((a | b).value()) << std::endl;
    std::cout << "   ~a (complement) = " << static_cast<int>((~a).value()) << std::endl;
    std::cout << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 6: Arrays
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "6. Octovalent array" << std::endl;
    octo_array<8, 5> arr = {0, 2, 4, 6, 7};
    
    std::cout << "   Array: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << static_cast<int>(arr[i].value()) << " ";
    }
    std::cout << std::endl << std::endl;
    
    // ───────────────────────────────────────────────────────────────────────
    // Example 7: Fold (sum of all quaternary states)
    // ───────────────────────────────────────────────────────────────────────
    
    std::cout << "7. Sum of all quaternary states (N=4)" << std::endl;
    int sum = octo_fold<4>(0, [](int acc, auto state) {
        return acc + state.value();
    });
    std::cout << "   Sum: " << sum << std::endl;
    std::cout << "   (0 + 1 + 2 + 3 = 6)" << std::endl;
    std::cout << std::endl;
    
    return 0;
}
