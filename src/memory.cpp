#include "../header/mymemory.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <exception>
#include <iostream>
#include <print>
#include <sstream>
#include <stdexcept>
#include<fstream>
#include <string>


    auto memory::ram_used()->int
    {
        return generalint;
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
                        std::println("idx:{}",generalulong);
                        if(generalulong!=4)
                        {
                            std::println("Not Hex Value at Line : {},index :{} ,value found :{}",curr_mem_pointer,generalulong,generalstr);
                        }
                    }
                    catch(std::exception e)
                    {
                       std::cout<< "Error while parsing text !!"<<std::endl<<e.what()<<std::endl;
                    }
                }
            }
            if(!input.eof()){
                std::cout<< "Error ig :( file not end proper"<<std::endl;
            }
        }
        else{
            std::cout << "Error FILE =>"<< filenm<<" could not be open :("<<std::endl;
        }
        generalint=curr_mem_pointer;
        std::println("in load {}",generalint);
    }


