#pragma once

#include <vector>
#include <thread>
#include <iostream>

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

#include <future>


class task_queue
{
    using lock_t = std::unique_lock<std::mutex>;
public:
    using task_t = std::function<void()>;
    void done()
    {
        {
            lock_t lock(m_mutex);
            m_done = true;
        }

        m_ready.notify_all();
    }
    bool try_pop(std::function<void()>& task)
    {
        lock_t lock(m_mutex);
        while (m_queue.empty() && !m_done)
            m_ready.wait(lock);

        if (m_queue.empty())
            return false;

        task = std::move(m_queue.front());
        m_queue.pop_front();

        return true;
    }

    template<typename F>
    void push(F&& task)
    {
        {
            lock_t lock(m_mutex);
            m_queue.emplace_back(std::forward<F>(task));
        }

        m_ready.notify_one();
    }


private:
    std::deque<std::function<void()>> m_queue;
    std::condition_variable m_ready;
    std::mutex m_mutex;
    bool m_done = false;
};

class TPool
{
public:
    static TPool& instance();

    ~TPool();

    template<typename F>
    void async_(F&& f)
    {
        auto i = m_index++;
        m_queues[i % m_threads.size()].push(std::forward<F>(f));
    }

    template<typename Task, typename... Args, typename R = std::invoke_result_t<Task, Args...>>
    std::future<R> async(Task task, Args... args)
    {
        auto promise = std::make_shared<std::promise<R>>();
        async_([promise, task, ...args = std::forward<Args>(args)]()
        {
            if constexpr (std::is_void_v<R>)
            {
                task(args...);

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
    TPool();

    void run(unsigned i);

private:
    std::vector<std::jthread> m_threads;
    std::vector<task_queue> m_queues;
    std::atomic<unsigned> m_index = 0;
};