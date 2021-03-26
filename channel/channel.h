#ifndef CHANNEL_H
#define CHANNEL_H
#include <condition_variable>
#include <queue>
template<typename T>
class Channel {
public:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cv;

    void blockAndPut(const T& thing) {
        {
            std::lock_guard<std::mutex> g(&mutex);
            queue.push(thing);
        }
        cv.notify_one();
    }
    T blockAndTake() {
        std::unique_lock<std::mutex> lck(mutex);
        cv.wait(lck, [this]{ return !queue.empty(); });
        return queue.front();
    }
};


#endif // CHANNEL_H
