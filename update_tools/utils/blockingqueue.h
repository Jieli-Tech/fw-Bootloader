#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>

template<typename T>
class BlockingQueue
{
private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::deque<T> m_queue;

public:
    void clear()
    {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        m_queue.clear();
    }

    void put(T const &value)
    {
        {
            std::unique_lock<std::mutex> lock(this->m_mutex);
            m_queue.push_front(value);
        }
        this->m_cond.notify_one();
    }

    T take()
    {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        this->m_cond.wait(lock, [=] { return !this->m_queue.empty(); });
        T rc(std::move(this->m_queue.back()));
        this->m_queue.pop_back();
        return rc;
    }

    bool poll(T &out, int timeout)
    {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        if (this->m_cond.wait_for(lock, std::chrono::milliseconds(timeout),
                                  [=] { return !this->m_queue.empty(); })) {
            out = std::move(this->m_queue.back());
            this->m_queue.pop_back();
            return true;
        }
        return false;
    }
};

#endif // BLOCKINGQUEUE_H
