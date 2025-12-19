// O Language — Universal Multi-State Extension of C++
// runtime.hpp — Runtime system for concurrent O processes
//
// Manages thread pool and process scheduling

#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

namespace o {

// ============================================================================
// PROCESS HANDLE
// ============================================================================

class O_handle {
    std::future<void> future_;
    
public:
    O_handle() = default;
    explicit O_handle(std::future<void>&& f) : future_(std::move(f)) {}
    
    // Wait for process to complete
    void join() {
        if (future_.valid()) {
            future_.get();
        }
    }
    
    // Check if process has completed
    bool is_ready() const {
        return future_.valid() && 
               future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }
    
    // Check if handle is valid
    bool valid() const {
        return future_.valid();
    }
};

// ============================================================================
// RUNTIME (Thread Pool + Scheduler)
// ============================================================================

class O_runtime {
private:
    // Thread pool
    std::vector<std::thread> workers_;
    
    // Task queue
    std::queue<std::function<void()>> tasks_;
    mutable std::mutex queue_mutex_;
    std::condition_variable cv_;
    
    // State
    std::atomic<bool> stop_;
    std::atomic<size_t> active_tasks_;
    
    // Singleton instance
    static std::unique_ptr<O_runtime> instance_;
    static std::once_flag init_flag_;
    
    // Private constructor
    explicit O_runtime(size_t num_threads = 0) 
        : stop_(false), active_tasks_(0) {
        
        // Default: hardware concurrency
        if (num_threads == 0) {
            num_threads = std::thread::hardware_concurrency();
            if (num_threads == 0) num_threads = 4; // Fallback
        }
        
        // Start worker threads
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_thread(); });
        }
    }
    
    // Worker thread function
    void worker_thread() {
        while (true) {
            std::function<void()> task;
            
            {
                std::unique_lock lock(queue_mutex_);
                cv_.wait(lock, [this] { 
                    return stop_ || !tasks_.empty(); 
                });
                
                if (stop_ && tasks_.empty()) {
                    return;
                }
                
                task = std::move(tasks_.front());
                tasks_.pop();
            }
            
            ++active_tasks_;
            task();
            --active_tasks_;
        }
    }
    
public:
    // Disable copy/move
    O_runtime(const O_runtime&) = delete;
    O_runtime& operator=(const O_runtime&) = delete;
    
    // Destructor
    ~O_runtime() {
        {
            std::lock_guard lock(queue_mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    // Get singleton instance
    static O_runtime& get() {
        std::call_once(init_flag_, []() {
            instance_.reset(new O_runtime());
        });
        return *instance_;
    }
    
    // Spawn a new process
    template<typename Func>
    O_handle spawn(Func&& f) {
        auto task = std::make_shared<std::packaged_task<void()>>(
            std::forward<Func>(f)
        );
        
        O_handle handle(task->get_future());
        
        {
            std::lock_guard lock(queue_mutex_);
            if (stop_) {
                throw std::runtime_error("O_runtime: cannot spawn on stopped runtime");
            }
            tasks_.emplace([task]() { (*task)(); });
        }
        cv_.notify_one();
        
        return handle;
    }
    
    // Wait for all active tasks to complete
    void wait_all() {
        while (active_tasks_ > 0 || !tasks_.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    // Get number of worker threads
    size_t num_workers() const {
        return workers_.size();
    }
    
    // Get number of pending tasks
    size_t num_pending() const {
        std::lock_guard lock(queue_mutex_);
        return tasks_.size();
    }
    
    // Get number of active tasks
    size_t num_active() const {
        return active_tasks_;
    }
};

// Static member initialization
inline std::unique_ptr<O_runtime> O_runtime::instance_ = nullptr;
inline std::once_flag O_runtime::init_flag_;

// ============================================================================
// CONVENIENCE FUNCTIONS
// ============================================================================

// Spawn a process
template<typename Func>
O_handle O_process(Func&& f) {
    return O_runtime::get().spawn(std::forward<Func>(f));
}

// Join all processes
inline void O_join_all() {
    O_runtime::get().wait_all();
}

// Get runtime statistics
struct O_runtime_stats {
    size_t num_workers;
    size_t num_pending;
    size_t num_active;
};

inline O_runtime_stats O_get_stats() {
    auto& rt = O_runtime::get();
    return {
        rt.num_workers(),
        rt.num_pending(),
        rt.num_active()
    };
}

} // namespace o

// End of runtime.hpp
