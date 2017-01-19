#include "slotfactory.h"

#include <fstream>
#include <iostream>

namespace
{

class FileWriter : public Slot
{
public:
    FileWriter()
        : file_("a.txt")
        , started_(false)
    {
    }

    std::string id() override
    {
        return "FileWriter";
    }

    void setPara(const std::string & key, const std::string & value) override
    {
        if(key == "fileName")
        {
            file_ = value;
        }
    }

    void addSink(Slot *) override
    {

    }

    bool start() override
    {
        if(started_)
            return true;
        if(ofs_.is_open())
        {
            ofs_.close();
        }

        ofs_.open(file_, std::ios_base::binary|std::ios_base::out|std::ios_base::app);

        started_ = true;
        return true;
    }

    void input(const char * data, size_t size) override
    {
        if(started_)
        {
            ofs_.write(data, size);
	    ofs_.flush();
        }
    }

    void stop() override
    {
        if(!started_)
            return;

        if(ofs_.is_open())
        {
            ofs_.close();
        }
        started_ = false;
    }

    ~FileWriter() override
    {
        stop();
    }

private:
    bool started_;

    std::string file_;

    std::ofstream ofs_;
};

}

SlotPtr createSlot()
{
    return std::shared_ptr<FileWriter>(new FileWriter());
}
