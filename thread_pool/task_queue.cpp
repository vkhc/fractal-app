#include "task_queue.h"

void task_queue::done()
{
    {
        lock_t lock(m_mutex);
        m_done = true;
    }

    m_ready.notify_all();
}

bool task_queue::pop(task_t& task)
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

bool task_queue::try_pop(task_t& task)
{
    lock_t lock(m_mutex, std::try_to_lock);
    if (!lock || m_queue.empty())
        return false;

    task = std::move(m_queue.front());
    m_queue.pop_front();

    return true;
}