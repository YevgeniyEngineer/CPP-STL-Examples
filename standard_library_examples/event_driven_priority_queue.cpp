#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>

class EventQueue
{
  public:
    void signalEvent()
    {
        event_count_.fetch_add(1, std::memory_order_release);
        cond_var_.notify_one();
    }

    void waitForEvent()
    {
        while (event_count_.load(std::memory_order_acquire) == 0)
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_var_.wait(lock, [this] { return event_count_.load(std::memory_order_acquire) > 0; });
        }
        event_count_.fetch_sub(1, std::memory_order_release);
    }

  private:
    std::atomic<int> event_count_{0};
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

struct TimestampedData
{
    int data;
    std::chrono::system_clock::time_point timestamp;

    bool operator<(const TimestampedData &other) const
    {
        return timestamp < other.timestamp;
    }
};

class TimestampedPriorityQueue
{
  public:
    void push(const TimestampedData &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
    }

    TimestampedData pop()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        TimestampedData value = queue_.top();
        queue_.pop();
        return value;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

  private:
    std::priority_queue<TimestampedData> queue_;
    std::mutex mutex_;
};

void producer(EventQueue &event_queue, TimestampedPriorityQueue &data_queue, int producer_id, int num_events)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < num_events; ++i)
    {
        int data = dis(gen);
        auto timestamp = std::chrono::system_clock::now();

        TimestampedData item{data, timestamp};
        std::cout << "Producer " << producer_id << ": Pushing data " << data << std::endl;
        data_queue.push(item);
        event_queue.signalEvent();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(EventQueue &event_queue, TimestampedPriorityQueue &data_queue, int num_total_events)
{
    for (int i = 0; i < num_total_events; ++i)
    {
        event_queue.waitForEvent();
        TimestampedData item = data_queue.pop();
        std::cout << "Consumer: Processing data " << item.data << std::endl;
    }
}

int main()
{
    EventQueue event_queue;
    TimestampedPriorityQueue data_queue;

    int num_producers = 3;
    int num_events_per_producer = 5;

    std::vector<std::thread> producer_threads;
    for (int i = 0; i < num_producers; ++i)
    {
        producer_threads.emplace_back(producer, std::ref(event_queue), std::ref(data_queue), i + 1,
                                      num_events_per_producer);
    }

    std::thread consumer_thread(consumer, std::ref(event_queue), std::ref(data_queue),
                                num_producers * num_events_per_producer);

    for (auto &t : producer_threads)
    {
        t.join();
    }
    consumer_thread.join();

    return 0;
}