# Contributing to O Language

Thank you for your interest in contributing to O!

## Ways to Contribute

### 1. Bug Reports

- Open an issue on GitHub
- Include minimal reproducing example
- Specify compiler version and OS

### 2. Feature Requests

- Propose new multi-state operations
- Suggest API improvements
- Request backend support (quantum, ternary, etc.)

### 3. Code Contributions

#### Areas needing help:

- **Test coverage**: Unit tests for edge cases
- **Documentation**: Tutorials, examples, API docs
- **Backends**: Quantum simulators, ternary hardware interfaces
- **Performance**: SIMD optimizations for specific N values

#### Pull Request Process:

1. Fork the repository
2. Create feature branch: `git checkout -b feature/my-feature`
3. Write tests for new code
4. Ensure all tests pass
5. Update documentation
6. Submit PR with clear description

### 4. Documentation

- Fix typos or clarify explanations
- Add examples
- Improve tutorials
- Translate docs

## Code Style

- Follow existing style (clang-format)
- Use `constexpr` where possible
- Document all public API functions
- Keep functions small and focused

## Testing

```bash
# Build tests
mkdir build && cd build
cmake .. -DO_BUILD_TESTS=ON
make

# Run tests
ctest --verbose
```

## Questions?

- Email: quantumlens.research@gmail.com
- GitHub Discussions: TBD

## License

All contributions will be licensed under CC BY-NC-SA 4.0.

---

**Thank you for helping make O better!**
