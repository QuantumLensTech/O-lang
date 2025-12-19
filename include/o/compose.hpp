// O Language — Universal Multi-State Extension of C++
// compose.hpp — Functional composition utilities
//
// Provides tools for composing O_state transformations

#pragma once

#include <tuple>
#include <utility>
#include <type_traits>

namespace o {

// ============================================================================
// PIPELINE (Function Composition)
// ============================================================================

template<typename... Funcs>
class O_pipeline {
    std::tuple<Funcs...> funcs_;
    
public:
    explicit O_pipeline(Funcs... fs) : funcs_(std::move(fs)...) {}
    
    // Apply pipeline to input
    template<typename Input>
    auto operator()(Input&& input) const {
        return apply_impl(std::forward<Input>(input), 
                         std::index_sequence_for<Funcs...>{});
    }
    
private:
    template<typename Input, size_t... Is>
    auto apply_impl(Input&& input, std::index_sequence<Is...>) const {
        return apply_funcs(std::forward<Input>(input), 
                          std::get<Is>(funcs_)...);
    }
    
    // Base case: single function
    template<typename Input, typename Func>
    static auto apply_funcs(Input&& input, Func&& f) {
        return f(std::forward<Input>(input));
    }
    
    // Recursive case: multiple functions
    template<typename Input, typename Func, typename... Rest>
    static auto apply_funcs(Input&& input, Func&& f, Rest&&... rest) {
        return apply_funcs(f(std::forward<Input>(input)), 
                          std::forward<Rest>(rest)...);
    }
};

// Compose functions into pipeline
template<typename... Funcs>
O_pipeline<Funcs...> O_compose(Funcs&&... funcs) {
    return O_pipeline<Funcs...>(std::forward<Funcs>(funcs)...);
}

// ============================================================================
// MAP (Apply function to collection)
// ============================================================================

template<typename Func, typename Container>
auto O_map(Func&& f, const Container& container) {
    using Result = decltype(f(*container.begin()));
    std::vector<Result> results;
    results.reserve(container.size());
    
    for (const auto& item : container) {
        results.push_back(f(item));
    }
    
    return results;
}

// ============================================================================
// FILTER (Select elements matching predicate)
// ============================================================================

template<typename Pred, typename Container>
Container O_filter(Pred&& pred, const Container& container) {
    Container results;
    
    for (const auto& item : container) {
        if (pred(item)) {
            results.push_back(item);
        }
    }
    
    return results;
}

// ============================================================================
// REDUCE (Fold over collection)
// ============================================================================

template<typename Func, typename Container, typename Init>
auto O_reduce(Func&& f, const Container& container, Init init) {
    auto accumulator = init;
    
    for (const auto& item : container) {
        accumulator = f(accumulator, item);
    }
    
    return accumulator;
}

// ============================================================================
// PARTIAL APPLICATION
// ============================================================================

template<typename Func, typename... Args>
class O_partial {
    Func func_;
    std::tuple<Args...> bound_args_;
    
public:
    O_partial(Func f, Args... args) 
        : func_(std::move(f)), bound_args_(std::move(args)...) {}
    
    template<typename... ExtraArgs>
    auto operator()(ExtraArgs&&... extra_args) const {
        return apply_impl(std::index_sequence_for<Args...>{}, 
                         std::forward<ExtraArgs>(extra_args)...);
    }
    
private:
    template<size_t... Is, typename... ExtraArgs>
    auto apply_impl(std::index_sequence<Is...>, 
                   ExtraArgs&&... extra_args) const {
        return func_(std::get<Is>(bound_args_)..., 
                    std::forward<ExtraArgs>(extra_args)...);
    }
};

template<typename Func, typename... Args>
O_partial<Func, Args...> O_bind(Func&& f, Args&&... args) {
    return O_partial<Func, Args...>(std::forward<Func>(f), 
                                    std::forward<Args>(args)...);
}

// ============================================================================
// CURRY (Transform multi-arg function to chained single-arg)
// ============================================================================

template<typename Func>
class O_curry {
    Func func_;
    
public:
    explicit O_curry(Func f) : func_(std::move(f)) {}
    
    template<typename Arg>
    auto operator()(Arg&& arg) const {
        return O_bind(func_, std::forward<Arg>(arg));
    }
};

template<typename Func>
O_curry<Func> O_make_curry(Func&& f) {
    return O_curry<Func>(std::forward<Func>(f));
}

// ============================================================================
// MEMOIZATION (Cache function results)
// ============================================================================

template<typename Func>
class O_memoized {
    Func func_;
    mutable std::unordered_map<typename Func::argument_type, 
                               typename Func::result_type> cache_;
    mutable std::mutex mutex_;
    
public:
    explicit O_memoized(Func f) : func_(std::move(f)) {}
    
    template<typename Arg>
    auto operator()(Arg&& arg) const {
        std::lock_guard lock(mutex_);
        
        auto it = cache_.find(arg);
        if (it != cache_.end()) {
            return it->second;
        }
        
        auto result = func_(std::forward<Arg>(arg));
        cache_[arg] = result;
        return result;
    }
    
    // Clear cache
    void clear() const {
        std::lock_guard lock(mutex_);
        cache_.clear();
    }
    
    // Get cache statistics
    size_t cache_size() const {
        std::lock_guard lock(mutex_);
        return cache_.size();
    }
};

template<typename Func>
O_memoized<Func> O_memoize(Func&& f) {
    return O_memoized<Func>(std::forward<Func>(f));
}

// ============================================================================
// ZIP (Combine two sequences)
// ============================================================================

template<typename Container1, typename Container2>
auto O_zip(const Container1& c1, const Container2& c2) {
    using T1 = typename Container1::value_type;
    using T2 = typename Container2::value_type;
    
    std::vector<std::pair<T1, T2>> results;
    
    auto it1 = c1.begin();
    auto it2 = c2.begin();
    
    while (it1 != c1.end() && it2 != c2.end()) {
        results.emplace_back(*it1, *it2);
        ++it1;
        ++it2;
    }
    
    return results;
}

// ============================================================================
// FLATTEN (Reduce nested structure)
// ============================================================================

template<typename Container>
auto O_flatten(const Container& container) {
    using Inner = typename Container::value_type;
    using Element = typename Inner::value_type;
    
    std::vector<Element> results;
    
    for (const auto& inner : container) {
        for (const auto& elem : inner) {
            results.push_back(elem);
        }
    }
    
    return results;
}

} // namespace o

// End of compose.hpp
