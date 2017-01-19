#pragma once

#include <string>
#include <list>

struct Entry
{
    std::string name;
    std::list<std::pair<std::string, std::string> > para;

    std::list<std::string> sink;
};

typedef std::list<Entry> EntryList;

