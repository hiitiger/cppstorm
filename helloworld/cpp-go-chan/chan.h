#pragma once
#include <shared_mutex>

template<class T>
class ChanQueue
{

    std::mutex mutex_;
    std::deque<T> queue_;
    std::atomic<std::size_t> queueSize_ = 0;

    const std::size_t maxQueueSize_ = 1;

    std::shared_mutex readyReadMutex_;
    std::shared_mutex readyWriteMutex_;

    concurrency_::task_completion_event_once<void> readyRead_;
    concurrency_::task_completion_event_once<void> readyWrite_;


    std::mutex readMutex_;
    std::mutex writeMutex_;


public:
    ChanQueue()
    {
        _notifyWrite();
    }

    ChanQueue(std::size_t sz)
        : maxQueueSize_(sz)
    {
        _notifyWrite();
    }

    concurrency::task<T> read()
    {
        using namespace std::chrono_literals;
        do {
            while (queueSize_ > 0) {
                if (readMutex_.try_lock()) {
                    mutex_.lock();
                    if (queueSize_ > 0)
                    {
                        T d = _deque();
                        mutex_.unlock();

                        readMutex_.unlock();

                        _notifyWrite();

                        __return d;
                    }

                    mutex_.unlock();
                    readMutex_.unlock();
                }
            }

            readyReadMutex_.lock_shared();
            __await concurrency_::asyncWait(readyRead_);
            readyReadMutex_.unlock_shared();
        } while (true);
    }

    concurrency::task<void> write(const T& d)
    {
        using namespace std::chrono_literals;
        do {
            while (queueSize_ < maxQueueSize_) {

                if (writeMutex_.try_lock()) {
                    mutex_.lock();
                    if (queueSize_ < maxQueueSize_) {

                        _enque(d);
                        mutex_.unlock();

                        writeMutex_.unlock();

                        _notifyRead();

                        __return;
                    }

                    mutex_.unlock();

                    writeMutex_.unlock();
                }
            }

            readyWriteMutex_.lock_shared();
            __await concurrency_::asyncWait(readyWrite_);
            readyWriteMutex_.unlock_shared();
        } while (true);
    }

private:

    void _enque(const T& d)
    {
        queue_.push_back(d);
        queueSize_ = queue_.size();
    }

    T _deque()
    {
        T d = queue_.front();
        queue_.pop_front();
        queueSize_ = queue_.size();
        return d;
    }

    void _notifyRead()
    {
        {
            readyReadMutex_.lock_shared();
            readMutex_.lock();
            readyRead_.set();
            readMutex_.unlock();
            readyReadMutex_.unlock_shared();
        }
    }

    void _notifyWrite()
    {
        {
            readyWriteMutex_.lock_shared();
            writeMutex_.lock();
            readyWrite_.set();
            writeMutex_.unlock();
            readyWriteMutex_.unlock_shared();
        }
    }
};
