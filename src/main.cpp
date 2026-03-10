#include "RequiredFn.cpp"
#include "../header/mymemory.hpp"
#include <print>
#include<filesystem>
auto main (void)
-> int
{
    memory ram= memory();
    std::println("Start");
    auto temp=std::filesystem::exists("IO/asmano.txt");
    std::println("exists {}",temp?"exists":"not");
    ram.load_instruction("IO/asmano.txt");
 
   ram.ram_to_file("IO/ramstate.txt",false);
    std::println("End");
    return 0;
}