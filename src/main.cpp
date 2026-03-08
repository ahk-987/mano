#include "../header/mymemory.hpp"
#include <print>
#include<filesystem>
auto printmem(memory ram){
    std::println("TRied printing!!");
    for(int i=0;i<ram.ram_used();i++)
    {
        std::println("{} : {} ",i,ram[i]); 

    }
}
auto main (void)
-> int
{
    memory ram= memory();
    std::println("Start");
    auto temp=std::filesystem::exists("IO/asmano.txt");
    std::println("exists {}",temp?"exists":"not");
    ram.load_instruction("IO/asmano.txt");
    printmem(ram);
    std::println("End");
    return 0;
}