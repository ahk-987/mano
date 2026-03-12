#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <cstdint>
#include <cstddef>
#include <string>

class memory
{
    private:
    static constexpr int ram_size = 4096;
    uint16_t ram[ram_size]{};
    int generalint;
    
    uint16_t generaluint16;
    char generalchar;
    
    [[maybe_unused]]
    size_t generalulong;
    
    [[maybe_unused]]
    
    std::string generalstr;
    
    
    public:
    bool failed_load_file=true;
    auto printRam();
auto ram_to_file(std::string filenm,bool all)-> void;

auto ram_used()->int;

uint16_t& operator[](int index);


void load_instruction(std::string filenm);
~memory();

};

#endif

