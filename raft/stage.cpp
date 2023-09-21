#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class Stage {
public:
    Stage() {
        worker_thread_ = std::thread(&Stage::run, this);
    }

    ~Stage() {
        stop_ = true;
        condition_.notify_all();
        worker_thread_.join();
    }

    void enqueueTask(const std::function<void()>& task) {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(task);
        condition_.notify_one();
    }

private:
    void run() {
        while (!stop_) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this] { return !tasks_.empty() || stop_; });
                if (stop_) return;
                task = tasks_.front();
                tasks_.pop();
            }
            task();
        }
    }

    std::thread worker_thread_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

int main() {
    Stage stage;

    return 0;
}