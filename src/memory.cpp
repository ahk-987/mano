#include "../header/mymemory.hpp"
#include "RequiredFn.cpp"
#include <cstdio>
#include <exception>
#include <filesystem>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <string>


    auto memory::ram_to_file(std::string filenm,bool all=false)->void
    {
        std::string formattedstr;
        std::string loc_ram_state;
        std::ofstream output;
        try{
            output.open(filenm);
            if(output.is_open()){
                output<<"[LOC ]\t[HEX]\t[DECI]\n";
                for(int loc=0;loc<ram_size;loc++){
                    if(ram[loc]){
                        formattedstr=(loc/10?loc/100?loc/1000?"":"0":"00":"000");
                        output<<"["<<formattedstr<< loc<<"]\t";
                        formattedstr=(ram[loc]/16?(ram[loc]/256?ram[loc]/4096?"":"0":"00"):"000");
                        output<<std::hex<<formattedstr<< ram[loc] << "\t";
                        output<<std::dec;
                        formattedstr=(ram[loc]/10?ram[loc]/100?ram[loc]/1000?ram[loc]/10000?"":"0":"00":"000":"0000");
                        output<<formattedstr<<ram[loc]<<"\n";
                    }
                    else if(all)
                    {
                        output<<"["<<loc<<"]"<<"0000";
                    }
                }
                output.close();
            }
            else {
                std::println("Error File could not be opened :(");
            }
        }
        catch(std::filesystem::filesystem_error e)
        {
            std::println("{}gadbad",e.what());
        }
    }

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
    void memory::load_instruction(std::string filenm)
    {
        
        uint16_t curr_mem_pointer=0;
        auto input = std::ifstream() ;
        input.open(filenm);
        if(input.is_open()&&input.peek()!=EOF)
        {
            while (std::getline(input,generalstr)) {
                if(generalstr.at(0)!=';')
                {
                    if(generalstr.starts_with('O'))
                    {
                        std::istringstream temp(generalstr);
                        temp>>generalstr;
                        if(generalstr=="ORG" || generalstr =="org")
                        {
                            temp>>generaluint16;
                        }
                        curr_mem_pointer=generaluint16>=ram_size-1?throw std::out_of_range("Origin can not be at last index!!"):generaluint16;
                        continue;
                    }
                    try{
                        generaluint16=static_cast<uint16_t>(std::stoi(generalstr,&generalulong,16));
                        ram[curr_mem_pointer++]=generaluint16;
                        //std::println("idx:{}",generalulong);
                        if(generalulong!=4)
                        {
                            std::println("Not Hex Value at Line : {},index :{} ,value found :{}",curr_mem_pointer,generalulong,generalstr);
                        }
                    }
                    catch(std::exception e)
                    {
                        input.close();
                       std::cout<< "Error while parsing text !!"<<std::endl<<e.what()<<std::endl;
                       break;
                    }
                }
            }
            if(!input.eof()){
                std::cout<< "Error ig :( file not end proper"<<std::endl;
            }
        input.close();
        }
        else{
            std::cout << "Error FILE =>"<< filenm<<" could not be open :("<<std::endl;
        }
        generalint=curr_mem_pointer;
       // std::println("in load {}",generalint);
    }
    auto memory::printRam(){
    for(int i=0;i<ram_used();i++)
    {
        std::println("{} : {} ",i,ram[i]); 

    }
}

