#ifndef CHANNEL_H
#define CHANNEL_H
#include <mutex>
#include <condition_variable>
//#include <queue>
#include <sys/syscall.h>
//#include <signal.h>
#include <QDebug>
#include <unistd.h>
inline thread_local char thread_name[30];

template<typename T>
class Channel {
//    using T;
public:
    T thing;
    bool full{false};
    std::condition_variable cv;
    std::mutex mutex;
//    QString waitingName;
    pid_t waitingTid{-1};
//    ~Channel() {

//    }
    void blockAndPut(const T& thing_) {
        {
            pthread_t cur = pthread_self();
            pthread_getname_np(cur,
                                   thread_name, 30);
            qDebug() << "obtaining" << gettid() << thread_name << Q_FUNC_INFO;
            std::lock_guard<std::mutex> g(mutex);
            thing = thing_;
            full = true;
            qDebug() << "releasing" << gettid() << thread_name << Q_FUNC_INFO;

        }
        cv.notify_one();
    }
    T take() {

        pthread_t cur = pthread_self();
        pthread_getname_np(cur,
                               thread_name, 30);
        qDebug() << "obtaining" << gettid() << thread_name << Q_FUNC_INFO;
        std::lock_guard<std::mutex> lck(mutex);
//        cv.wait(lck, [this]{ return full; });
        full = false;
        qDebug() << "releasing" << gettid() << thread_name << Q_FUNC_INFO;
        return thing;
    }
    bool noBlockAndCheck() {
        if(mutex.try_lock()) {
            bool res = full;
            mutex.unlock();
            return res;
        }
        return false;
    }
//    void wake() {
//        raise(SIGALRM);
//    }
    void wait() {
        waitingTid = gettid();
//        char name[40];
//        waitingName = pthread_getname_np(pthread_self(), name, 40);
//        pause();
        qDebug() << "obtaining first" << gettid() << thread_name << Q_FUNC_INFO;

        std::unique_lock<std::mutex> lck(mutex);
        if(full) {
            waitingTid = -1;
            return;
        }
        qDebug() << "waiting on cv" << gettid() << thread_name << Q_FUNC_INFO;
        cv.wait(lck, [this]{ return full; });
        qDebug() << "after waiting on cv" << gettid() << thread_name << Q_FUNC_INFO;

        waitingTid = -1;
        qDebug() << "releasing" << gettid() << thread_name << Q_FUNC_INFO;
        return;
    }
    static inline pid_t gettid() {
        return syscall(SYS_gettid);
    }

};


#endif // CHANNEL_H
