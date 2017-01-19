#include <iostream>

#include "slotmanager.h"

using namespace std;

int main(int argc, char *argv[])
{
    SlotManager m;
    m.setPara("config", "config.xml");
    m.start();
    std::string cmd;
    while(true)
    {
        std::cout << "input exit to exit:" << std::endl;
        std::cin >> cmd;
        if(cmd == "exit")
            break;
    }
    m.stop();
    return 0;
}
