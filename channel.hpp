// O Language — Universal Multi-State Extension of C++
// channel.hpp — Concurrency channels for O_state types
//
// Implements CSP-style channels for communication between
// concurrent processes operating on multi-state data

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <chrono>

namespace o {

// ============================================================================
// O_CHANNEL<N> — Thread-safe channel for O_state<N>
// ============================================================================

template<size_t N>
class O_channel {
public:
    using State = O_state<N>;
    using size_type = size_t;
    
private:
    std::queue<State> buffer_;
    mutable std::mutex mutex_;
    std::condition_variable cv_not_empty_;
    std::condition_variable cv_not_full_;
    size_type capacity_;
    bool closed_;
    
public:
    // ========================================================================
    // CONSTRUCTORS
    // ========================================================================
    
    // Unbuffered channel (capacity = 0)
    O_channel() : capacity_(0), closed_(false) {}
    
    // Buffered channel with fixed capacity
    explicit O_channel(size_type capacity) 
        : capacity_(capacity), closed_(false) {}
    
    // Disable copy
    O_channel(const O_channel&) = delete;
    O_channel& operator=(const O_channel&) = delete;
    
    // Enable move
    O_channel(O_channel&& other) noexcept {
        std::lock_guard lock(other.mutex_);
        buffer_ = std::move(other.buffer_);
        capacity_ = other.capacity_;
        closed_ = other.closed_;
    }
    
    // ========================================================================
    // BLOCKING OPERATIONS
    // ========================================================================
    
    // Send value (blocks if channel full)
    void send(State value) {
        std::unique_lock lock(mutex_);
        
        if (closed_) {
            throw std::runtime_error("O_channel: send on closed channel");
        }
        
        // Wait if buffered channel is full
        if (capacity_ > 0) {
            cv_not_full_.wait(lock, [this] { 
                return buffer_.size() < capacity_ || closed_; 
            });
            
            if (closed_) {
                throw std::runtime_error("O_channel: channel closed while waiting");
            }
        }
        
        buffer_.push(value);
        cv_not_empty_.notify_one();
    }
    
    // Receive value (blocks if channel empty)
    State receive() {
        std::unique_lock lock(mutex_);
        
        cv_not_empty_.wait(lock, [this] { 
            return !buffer_.empty() || closed_; 
        });
        
        if (buffer_.empty() && closed_) {
            throw std::runtime_error("O_channel: receive on closed and empty channel");
        }
        
        State value = buffer_.front();
        buffer_.pop();
        
        if (capacity_ > 0) {
            cv_not_full_.notify_one();
        }
        
        return value;
    }
    
    // ========================================================================
    // NON-BLOCKING OPERATIONS
    // ========================================================================
    
    // Try to send (returns false if would block)
    bool try_send(State value) {
        std::unique_lock lock(mutex_);
        
        if (closed_) {
            return false;
        }
        
        if (capacity_ > 0 && buffer_.size() >= capacity_) {
            return false;
        }
        
        buffer_.push(value);
        cv_not_empty_.notify_one();
        return true;
    }
    
    // Try to receive (returns empty optional if would block)
    std::optional<State> try_receive() {
        std::unique_lock lock(mutex_);
        
        if (buffer_.empty()) {
            return std::nullopt;
        }
        
        State value = buffer_.front();
        buffer_.pop();
        
        if (capacity_ > 0) {
            cv_not_full_.notify_one();
        }
        
        return value;
    }
    
    // ========================================================================
    // TIMED OPERATIONS
    // ========================================================================
    
    // Send with timeout
    template<typename Rep, typename Period>
    bool send_for(State value, 
                  const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock lock(mutex_);
        
        if (closed_) {
            return false;
        }
        
        if (capacity_ > 0) {
            bool success = cv_not_full_.wait_for(lock, timeout, [this] {
                return buffer_.size() < capacity_ || closed_;
            });
            
            if (!success || closed_) {
                return false;
            }
        }
        
        buffer_.push(value);
        cv_not_empty_.notify_one();
        return true;
    }
    
    // Receive with timeout
    template<typename Rep, typename Period>
    std::optional<State> receive_for(
        const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock lock(mutex_);
        
        bool success = cv_not_empty_.wait_for(lock, timeout, [this] {
            return !buffer_.empty() || closed_;
        });
        
        if (!success || (buffer_.empty() && closed_)) {
            return std::nullopt;
        }
        
        State value = buffer_.front();
        buffer_.pop();
        
        if (capacity_ > 0) {
            cv_not_full_.notify_one();
        }
        
        return value;
    }
    
    // ========================================================================
    // CHANNEL MANAGEMENT
    // ========================================================================
    
    // Close channel (no more sends allowed)
    void close() {
        std::lock_guard lock(mutex_);
        closed_ = true;
        cv_not_empty_.notify_all();
        cv_not_full_.notify_all();
    }
    
    // Check if channel is closed
    bool is_closed() const {
        std::lock_guard lock(mutex_);
        return closed_;
    }
    
    // Get current buffer size
    size_type size() const {
        std::lock_guard lock(mutex_);
        return buffer_.size();
    }
    
    // Check if channel is empty
    bool empty() const {
        std::lock_guard lock(mutex_);
        return buffer_.empty();
    }
    
    // Get capacity
    size_type capacity() const {
        return capacity_;
    }
    
    // ========================================================================
    // OPERATOR OVERLOADS (for natural syntax)
    // ========================================================================
    
    // ch << value  (send)
    O_channel& operator<<(State value) {
        send(value);
        return *this;
    }
    
    // ch >> variable  (receive)
    O_channel& operator>>(State& out) {
        out = receive();
        return *this;
    }
};

// ============================================================================
// SELECT HELPERS (for multi-channel operations)
// ============================================================================

// Select result
template<size_t N>
struct SelectResult {
    int channel_index;  // Which channel was ready (-1 if timeout)
    std::optional<O_state<N>> value;  // Received value (if any)
    bool is_send;  // true if send operation, false if receive
};

// Select on multiple channels (simplified version)
// Real implementation would use more sophisticated mechanism
template<size_t N, typename... Channels>
SelectResult<N> select_receive(Channels&... channels) {
    // Try all channels in round-robin
    std::array<O_channel<N>*, sizeof...(Channels)> ptrs = {&channels...};
    
    for (size_t i = 0; i < ptrs.size(); ++i) {
        auto value = ptrs[i]->try_receive();
        if (value.has_value()) {
            return {static_cast<int>(i), value, false};
        }
    }
    
    // If none ready, return timeout
    return {-1, std::nullopt, false};
}

} // namespace o

// ============================================================================
// MACROS FOR SELECT SYNTAX
// ============================================================================

// O_select { case ch1 >> x: ...; case ch2 >> y: ...; }
#define O_select \
    { \
        bool __o_select_matched = false; \
        (void)__o_select_matched;

#define O_case_recv(ch, var) \
        if (!__o_select_matched) { \
            auto __o_opt = (ch).try_receive(); \
            if (__o_opt.has_value()) { \
                (var) = __o_opt.value(); \
                __o_select_matched = true;

#define O_case_send(ch, val) \
        if (!__o_select_matched) { \
            if ((ch).try_send(val)) { \
                __o_select_matched = true;

#define O_end_case \
            } \
        }

#define O_end_select \
    }

// End of channel.hpp
