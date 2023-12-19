#pragma once

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

class task_queue
{
    using task_t = std::function<void()>;
    using lock_t = std::unique_lock<std::mutex>;
public:

    void done();

    bool pop(task_t& task);
    bool try_pop(task_t& task);

    template <typename T>
    void push(T&& task)
    {
        {
            lock_t lock(m_mutex);
            m_queue.emplace_back(std::forward<T>(task));
        }

        m_ready.notify_one();
    }

    template <typename T>
    bool try_push(T&& task)
    {
        {
            lock_t lock(m_mutex, std::try_to_lock);
            if (!lock)
                return false;

            m_queue.emplace_back(std::forward<T>(task));
        }

        m_ready.notify_one();

        return true;
    }

private:
    std::deque<task_t> m_queue;
    std::condition_variable m_ready;
    std::mutex m_mutex;
    
    bool m_done { false };
};