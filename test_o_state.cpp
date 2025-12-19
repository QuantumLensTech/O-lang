// Unit Tests for O_state<N>
// Testing: construction, arithmetic, comparison, conversion

#include <o/core.hpp>
#include <gtest/gtest.h>

using namespace o;

// ============================================================================
// CONSTRUCTION AND BASIC PROPERTIES
// ============================================================================

TEST(OState, Construction) {
    O_octal a;
    EXPECT_EQ(a.value(), 0);
    
    O_octal b(5);
    EXPECT_EQ(b.value(), 5);
    
    O_octal c(10);  // 10 % 8 = 2
    EXPECT_EQ(c.value(), 2);
}

TEST(OState, NumStates) {
    EXPECT_EQ(O_octal::num_states, 8);
    EXPECT_EQ(O_duodec::num_states, 12);
    EXPECT_EQ(O_hex::num_states, 16);
}

TEST(OState, MinMax) {
    auto min_o = O_octal::min_value();
    auto max_o = O_octal::max_value();
    
    EXPECT_EQ(min_o.value(), 0);
    EXPECT_EQ(max_o.value(), 7);
}

// ============================================================================
// ARITHMETIC OPERATIONS
// ============================================================================

TEST(OState, Addition) {
    O_octal a(5);
    O_octal b(3);
    O_octal c = a + b;
    
    EXPECT_EQ(c.value(), 0);  // (5+3) % 8 = 0
}

TEST(OState, Subtraction) {
    O_octal a(2);
    O_octal b(5);
    O_octal c = a - b;
    
    EXPECT_EQ(c.value(), 5);  // (2-5+8) % 8 = 5
}

TEST(OState, Multiplication) {
    O_octal a(3);
    O_octal b(5);
    O_octal c = a * b;
    
    EXPECT_EQ(c.value(), 7);  // (3*5) % 8 = 15 % 8 = 7
}

TEST(OState, Division) {
    O_octal a(6);
    O_octal b(2);
    O_octal c = a / b;
    
    EXPECT_EQ(c.value(), 3);  // 6 / 2 = 3
}

TEST(OState, DivisionByZero) {
    O_octal a(5);
    O_octal b(0);
    
    EXPECT_THROW(a / b, std::domain_error);
}

// ============================================================================
// INCREMENT/DECREMENT
// ============================================================================

TEST(OState, Increment) {
    O_octal a(6);
    ++a;
    EXPECT_EQ(a.value(), 7);
    
    ++a;
    EXPECT_EQ(a.value(), 0);  // Wraps around
}

TEST(OState, Decrement) {
    O_octal a(1);
    --a;
    EXPECT_EQ(a.value(), 0);
    
    --a;
    EXPECT_EQ(a.value(), 7);  // Wraps around
}

TEST(OState, PostIncrement) {
    O_octal a(5);
    O_octal b = a++;
    
    EXPECT_EQ(b.value(), 5);  // Old value
    EXPECT_EQ(a.value(), 6);  // New value
}

// ============================================================================
// COMPARISON
// ============================================================================

TEST(OState, Equality) {
    O_octal a(5);
    O_octal b(5);
    O_octal c(3);
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(OState, Ordering) {
    O_octal a(3);
    O_octal b(5);
    
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a >= a);
}

// ============================================================================
// CONVERSION
// ============================================================================

TEST(OState, ExplicitConversion) {
    O_octal a(5);
    uint64_t value = static_cast<uint64_t>(a);
    
    EXPECT_EQ(value, 5);
}

TEST(OState, BoolConversion) {
    O_octal zero(0);
    O_octal nonzero(5);
    
    EXPECT_FALSE(static_cast<bool>(zero));
    EXPECT_TRUE(static_cast<bool>(nonzero));
}

// ============================================================================
// EDGE CASES
// ============================================================================

TEST(OState, LargeValues) {
    O_octal a(1000);
    EXPECT_EQ(a.value(), 1000 % 8);
}

TEST(OState, NegativeWraparound) {
    O_octal a(0);
    O_octal b(1);
    O_octal c = a - b;
    
    EXPECT_EQ(c.value(), 7);  // Wraps to 7
}

// ============================================================================
// DIFFERENT N VALUES
// ============================================================================

TEST(OState, Binary) {
    O_binary a(0);
    O_binary b(1);
    
    EXPECT_EQ((a + b).value(), 1);
    
    O_binary c(1);
    c++;
    EXPECT_EQ(c.value(), 0);  // Wraps
}

TEST(OState, Duodecimal) {
    O_duodec a(10);
    O_duodec b(5);
    
    EXPECT_EQ((a + b).value(), 3);  // (10+5) % 12 = 3
}

TEST(OState, Hexadecimal) {
    O_hex a(15);
    O_hex b(2);
    
    EXPECT_EQ((a + b).value(), 1);  // (15+2) % 16 = 1
}

// ============================================================================
// COMPILE-TIME TESTS (constexpr)
// ============================================================================

static_assert(O_octal(5).value() == 5, "Constexpr construction");
static_assert((O_octal(3) + O_octal(5)).value() == 0, "Constexpr addition");
static_assert(O_octal::num_states == 8, "Constexpr num_states");

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
