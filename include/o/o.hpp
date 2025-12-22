#ifndef O_HPP
#define O_HPP

/**
 * @file o.hpp
 * @brief O Language - Complete Header
 * 
 * Main entry point for the O universal multi-state programming language.
 * Includes all core modules.
 * 
 * Usage:
 * ```cpp
 * #include <o/o.hpp>
 * using namespace o;
 * 
 * O<8> state = 5;  // Octovalent value
 * ```
 * 
 * @author Jean-Christophe Ané
 * @version 1.0
 * @date December 2025
 */

// Core types and operations
#include "core.hpp"
#include "logic.hpp"
#include "operators.hpp"

// Extended functionality
#include "ranges.hpp"
#include "functional.hpp"

// Hardware abstraction
#include "channels.hpp"

// Quantum computing support
#include "quantum.hpp"

/**
 * @namespace o
 * @brief O Language namespace
 * 
 * Contains all O language types, functions, and utilities for
 * universal multi-state computing.
 */

#endif // O_HPP
