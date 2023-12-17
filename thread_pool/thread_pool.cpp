#include "thread_pool.h"


TPool& TPool::instance()
{
    static TPool pool;

    return pool;
}

TPool::TPool() : m_count(std::thread::hardware_concurrency()), m_index(0)
{
    m_queues = std::vector<task_queue>(m_count);
    m_threads.reserve(m_count);
    for (int n = 0; n < m_count; ++n)
    {
        m_threads.emplace_back([this, n]{ run(n); });
    }
}

TPool::~TPool()
{
    for (auto& q : m_queues)
        q.done();
}

void TPool::run(unsigned i)
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