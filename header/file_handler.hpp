#ifndef FILE_HANDLER
#define FILE_HANDLER

#include "mymemory.hpp"
#include "../src/cpu.cpp"
#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
class file_io
{
    bool stdio_only,all_ram_loc;
    simulator &cpu;
    memory &ram;
    std::stringstream hexcode_stream;//use if stdio only to store hexcode for later processing
    std::string input_file,output_file,register_file,hexcode_file,error_file;
    /* This is extra idk why i made them lol*/
    int generalint;
    
    uint16_t generaluint16;
    char generalchar;
    
    [[maybe_unused]]
    size_t generalulong;
    
    [[maybe_unused]]
    
    std::string generalstr;
    public:
    
    enum filetype {
        INPUT_FILE,
        OUTPUT_FILE,
        OUTPUT_FILE_SHOW_ALL,
        REGISTER_FILE,
        HEXCODE_FILE,
        ERROR_FILE
    };
    const static std::unordered_map<std::string, uint16_t> NON_MRI,MRI ;

    file_io();
    file_io( simulator &cpu,memory &ram);//default case stdio-only case
    auto set_files_nm(std::string filenm,filetype type)->void;//to set file name 
    auto ram_to_file();
    auto registry_to_file();
    auto hexcode_to_file();
    auto load_instruction_to_ram();
    auto input_from_file();
    void run();
    void set_stdio(bool stdio);

};
#endif