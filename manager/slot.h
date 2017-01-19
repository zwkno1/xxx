#pragma once

#include <string>
#include <memory>

class Slot
{
public:
    virtual std::string id() = 0;

    virtual void setPara(const std::string & key, const std::string & value) = 0;

    virtual void addSink(Slot *) = 0;

    virtual bool start() = 0;

    virtual void input(const char * data, size_t size) = 0;

    virtual void stop() = 0;

    virtual ~Slot() {}
};

typedef std::shared_ptr<Slot> SlotPtr;
