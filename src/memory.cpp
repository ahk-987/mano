#include "../header/mymemory.hpp"

#include <print>
#include <stdexcept>


    memory::~memory()
    {   
        std::println("ram destroyed");
        // i am uncertain if i should make a 
        // provision for saving to file or print 
        // or both on destruction of ram?
    }

    uint16_t& memory::operator[](int index)
    {
        if (index>=0&&index<ram_size)
        {
            return ram[index];
        }
        else{
            std::println("{}",index);
            throw std::out_of_range("Index out of Bounds !!");
        }
    }


