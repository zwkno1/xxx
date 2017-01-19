#include "slotfactory.h"

#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace
{

class Tick : public Slot
{
public:
    Tick()
        : interval_(1)
        , stopped_(true)
    {
    }

    std::string id() override
    {
        return "Tick";
    }

    void setPara(const std::string & key, const std::string & value) override
    {
        if(key == "interval")
        {
            size_t sec = 0;
            sec = std::stoi(value);
            if(sec != 0)
                interval_ = sec;
        }
    }

    void addSink(Slot * s) override
    {
        slots_.push_back(s);
    }

    bool start() override
    {
        if(!stopped_)
            return true;
        thread_ = std::thread([this]()
        {
            std::unique_lock<std::mutex> lock(mutex_);
            while (!stopped_)
            {
                cond_.wait_for(lock, std::chrono::seconds(interval_));
                for(auto s : slots_)
                    s->input("tick...\n", 8);
            }
        });
        stopped_ = false;
        return true;
    }

    void input(const char * data, size_t size) override
    {
    }

    void stop() override
    {
        if(stopped_)
            return;

        std::unique_lock<std::mutex> lock(mutex_);
        stopped_ = true;
        cond_.notify_one();
        lock.unlock();

        thread_.join();
    }

    ~Tick() override
    {
        stop();
    }

private:
    std::atomic<bool> stopped_;

    size_t interval_;

    std::list<Slot *> slots_;

    std::thread thread_;

    std::mutex mutex_;

    std::condition_variable cond_;
};

}

SlotPtr createSlot()
{
    return std::shared_ptr<Tick>(new Tick());
}
