#pragma once

#include <thread>

namespace mirai::utils
{
    /**
     * \brief A thread class wrapping std::thread which joins automatically
     * when destructed
     * \remarks Alternative for the upcoming C++20 std::jthread. The one in
     * C++20 has more functionalities like cooperative cancelling.
     */
    class Thread final
    {
    private:
        template <typename T>
        static constexpr bool not_thread_v = !std::is_same_v<std::decay_t<T>, Thread>
            && !std::is_same_v<std::decay_t<T>, std::thread>;

        std::thread thread_;

        void force_join() noexcept;
    public:
        /**
         * \brief Construct a empty thread object
         */
        Thread() noexcept = default;

        /**
         * \brief Start a new thread running a callable object with the given arguments
         * \tparam F Type of the callable object
         * \tparam Ts Type of the bound arguments
         * \param f The callable object
         * \param args The bound arguments
         */
        template <typename F, typename... Ts, typename = std::enable_if_t<not_thread_v<F>>>
        explicit Thread(F&& f, Ts&&... args) :thread_(std::forward<F>(f), std::forward<Ts>(args)...) {}

        /**
         * \brief Join and destroy the thread
         */
        ~Thread() noexcept { force_join(); }

        /**
         * \brief A thread cannot be duplicated
         */
        Thread(const Thread&) = delete;

        /**
         * \brief Transfer the ownership from another thread
         */
        Thread(Thread&&) noexcept = default;

        /**
         * \brief A thread cannot be duplicated
         */
        Thread& operator=(const Thread&) = delete;

        /**
         * \brief Transfer the ownership of a thread from another object
         * \param other The other thread
         * \return Reference to this object
         */
        Thread& operator=(Thread&& other) noexcept;
        
        /**
         * \brief Swap this thread with another
         * \param other The other object
         */
        void swap(Thread& other) noexcept { std::swap(thread_, other.thread_); }
        
        /**
         * \brief Swap two threads
         * \param lhs The first object
         * \param rhs The second object
         */
        friend void swap(Thread& lhs, Thread& rhs) noexcept { lhs.swap(rhs); }

        /**
         * \brief Retrieve the native handle of the thread
         * \return The handle
         */
        std::thread::native_handle_type native_handle() { return thread_.native_handle(); }

        /**
         * \brief Retrieve the id of the thread
         * \return The id
         */
        std::thread::id get_id() const noexcept { return thread_.get_id(); }

        /**
         * \brief Query whether the thread is joinable
         * \return The result
         */
        bool joinable() const noexcept { return thread_.joinable(); }

        /**
         * \brief Join the thread
         */
        void join() { thread_.join(); }

        /**
         * \brief Detach the thread
         */
        void detach() { thread_.detach(); }
    };
}
