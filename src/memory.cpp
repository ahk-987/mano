#include "../header/mymemory.hpp"
#include <format>
#include <stdexcept>

uint16_t& memory::operator[](int index)
    {
        if (index>=0&&index<ram_size)
        {
            return ram[index];
        }
        else{
            throw std::out_of_range(std::format("Index out of Bounds :{} !!",index));
        }
    }


