#include "RequiredFn.cpp"
#include "cpu.cpp"
#include "../header/mymemory.hpp"
#include<filesystem>
#include <iostream>
#include <unistd.h>
auto main (void)
-> int
{
    memory ram= memory();
    //std::println("Start");
    auto temp=std::filesystem::exists("IO/asmano1.txt");
    std::cout<<"Exists?"<<(temp?"yes":"no")<<std::endl;
    ram.load_instruction("IO/asmano1.txt");

    simulator manocpu=simulator(0,ram);
    sleep(1);
    manocpu.run();

    auto temp2=std::filesystem::exists("IO/ramstate.txt");
    std::cout<<"Exists?"<<(temp2?"yes":"no")<<std::endl;
    ram.ram_to_file("IO/ramstate.txt",false);
    //std::println("End");
    return 0;
}