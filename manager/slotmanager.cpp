#include "slotmanager.h"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>

#include "entry.h"

SlotManager::SlotManager()
    : stopped_(true)
    , slotPath_("./slots")
{
}

SlotManager::~SlotManager()
{

}

std::string SlotManager::id()
{
    return id_;
}

void SlotManager::setPara(const std::string & key, const std::string & value)
{
    if(key == "config")
    {
        configFileName_ = value;
    }

    if(key == "id")
    {
        id_ = value;
    }
}

void SlotManager::addSink(Slot * slot)
{
}

bool SlotManager::start()
{
    loadConfig();
    for(auto & f : slots_)
    {
        f.second.slot->start();
    }
    stopped_ = false;
}

void SlotManager::input(const char * data, size_t size)
{
}

void SlotManager::stop()
{
    stopped_ = true;
    for(auto & f : slots_)
    {
        f.second.slot->stop();
    }
}

void SlotManager::loadSlots()
{

}

bool SlotManager::loadConfig()
{
    EntryList el;
    
    try
    {
        boost::property_tree::ptree tree;
        boost::property_tree::read_xml(configFileName_, tree);

        for(boost::property_tree::ptree::value_type & v : tree.get_child("root"))
        {
            if(v.first != "slot")
                continue;
            Entry e;
            e.name = v.second.get<std::string>("name");

            for(boost::property_tree::ptree::value_type & v2 : v.second.get_child("sink"))
            {
                if(v2.first != "name")
                    continue;
                e.sink.push_back(v2.second.get_value<std::string>());
            }

            for(boost::property_tree::ptree::value_type & v2 : v.second.get_child("para"))
            {
                if(v2.first != "pair")
                    continue;
                std::string key = v2.second.get<std::string>("key");
                std::string value = v2.second.get<std::string>("value");
                e.para.push_back(std::make_pair(std::move(key), std::move(value)));
            }

            el.push_back(std::move(e));
        }
    }
    catch(...)
    {
        return false;
    }

    for(auto e : el)
    {
           boost::filesystem::path libPath("slots");
           libPath /= e.name;

           auto factory = boost::dll::import<SlotFactoryFunc>(
                       libPath,
                       "createSlot",
                       boost::dll::load_mode::append_decorations
                       );

           auto slot = factory();
           for(auto & p : e.para)
	   {
		   std::cout << slot->id() << "->setPara(" << p.first << ", " << p.second << ")" << std::endl;
               slot->setPara(p.first, p.second);
	   }

           slots_.insert(std::pair<std::string, SlotFactoryData>(
                                 e.name, {factory, slot}));
    }

    for(auto & e : el)
    {
        auto iter = slots_.find(e.name);
        if(iter == slots_.end())
        {
            std::cout << e.name << " not loaded" << std::endl;
            return false;
        }
        for(auto & str : e.sink)
        {
            auto iter2 = slots_.find(str);
            if(iter == slots_.end())
            {
                std::cout << str << " not loaded" << std::endl;
                return false;
            }
            iter->second.slot->addSink(iter2->second.slot.get());
        }
    }

    return true;
}
