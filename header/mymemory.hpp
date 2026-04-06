#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <cstdint>

class memory
{
    public :static constexpr int ram_size = 4096;
    private:
    uint16_t ram[ram_size]{};
   
    
    public:

uint16_t& operator[](int index);


~memory();

};

#endif

