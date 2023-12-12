#include "thread_pool.h"


TPool& TPool::instance()
{
    static TPool pool;

    return pool;
}

TPool::TPool()
{
    unsigned int count = std::thread::hardware_concurrency();
    m_queues = std::vector<task_queue>(count);
    m_threads.reserve(count);
    for (int n = 0; n < count; ++n)
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

        if (!m_queues[i].try_pop(f))
            break;

        f();
    }
}