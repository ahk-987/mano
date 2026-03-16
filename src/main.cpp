#include "cpu.cpp"
#include "../header/mymemory.hpp"
#include <unistd.h>
auto main (int kwargs,char *args[])
-> int
{
   // parser;
    memory ram= memory();
    ram.load_instruction("mano/IO/asmano1.txt");

    simulator manocpu=simulator(0,ram);
    manocpu.run();
    manocpu.registry_to_file("mano/IO/registers.txt");
    ram.ram_to_file("IO/ramstate.txt",false);
    return 0;
}