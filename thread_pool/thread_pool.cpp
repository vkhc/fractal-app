#include "thread_pool.h"


thread_pool& thread_pool::instance()
{
    static thread_pool pool;

    return pool;
}

thread_pool::thread_pool() : m_count(std::thread::hardware_concurrency()), m_threads(m_count), m_queues(m_count), m_index(0)
{

    for (int n = 0; n < m_count; ++n)
    {
        m_threads[n] = std::jthread([this, n]{ run(n); });
    }
}

thread_pool::~thread_pool()
{
    for (auto& q : m_queues)
        q.done();
}

void thread_pool::run(unsigned i)
{
    while (true)
    {
        std::function<void()> f;

        for (size_t n = 0; n < m_count; ++n)
        {
            if (m_queues[(i + n) % m_count].try_pop(f))
                break;
        }

        if (!f && !m_queues[i].pop(f))
            break;

        f();
    }
}