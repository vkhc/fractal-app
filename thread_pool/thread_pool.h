#pragma once

#include "task_queue.h"

#include <vector>
#include <thread>
#include <functional>
#include <future>

class thread_pool
{
public:
    static thread_pool& instance();
    thread_pool(const thread_pool&) = delete;
    thread_pool(thread_pool&&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool& operator=(thread_pool&&) = delete;

    template<typename Task, typename... Args, typename R = std::invoke_result_t<Task, Args...>>
    std::future<R> async(Task task, Args... args)
    {
        auto promise = std::make_shared<std::promise<R>>();
        async_([promise, task, ...args = std::forward<Args>(args)]()
        {
            if constexpr (std::is_void_v<R>)
            {
                task(args...);
                // TODO: exception handling
                promise->set_value();
            }
            else
            {
                auto result = task(args...);

                promise->set_value(std::move(result));
            }
        });

        return promise->get_future();
    }

private:
    thread_pool();
    ~thread_pool();

    void run(unsigned i);

    template<typename F>
    void async_(F&& f)
    {
        auto i = m_index++;

        for (size_t n = 0; n < m_count; ++n)
        {
            if (m_queues[(i + n) % m_count].try_push(std::forward<F>(f)))
                return;
        }

        m_queues[i % m_count].push(std::forward<F>(f));
    }

private:
    size_t m_count;
    
    std::vector<std::jthread> m_threads;
    std::vector<task_queue> m_queues;

    std::atomic<unsigned> m_index;
};