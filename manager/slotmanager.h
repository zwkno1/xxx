#pragma once

#include <map>
#include <list>

#include <boost/dll.hpp>

#include "slot.h"

typedef SlotPtr (SlotFactoryFunc)();

typedef boost::dll::detail::import_type<SlotFactoryFunc>::type SlotFactoryType;

struct SlotFactoryData
{
    SlotFactoryType factory;
    SlotPtr slot;
};

class SlotManager : public Slot
{
public:
    SlotManager();

    ~SlotManager();

    std::string id() override;

    void setPara(const std::string & key, const std::string & value) override;

    void addSink(Slot *) override;

    bool start() override;

    void input(const char * data, size_t size) override;

    void stop() override;

private:

    void loadSlots();

    bool loadConfig();

    std::string id_;

    std::string slotPath_;

    std::string configFileName_;
    
    std::map<std::string, SlotFactoryData> slots_;

    bool stopped_;

};
