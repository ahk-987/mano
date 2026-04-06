#include "../header/file_handler.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <print>
#include <sstream>
#include <string>
#include <system_error>

file_io::file_io(simulator cpu, memory ram) {
    this->cpu = cpu;
    this->ram = ram;
    input_file=hexcode_file=output_file=register_file="";//empty intitalise
}

file_io::file_io()
{
    /*Idk why there was some wierd issues of sorts so creating empty constructor :(*/
}

auto file_io::registry_to_file() {
  auto file = std::ofstream(register_file);
  auto &regfile = stdio_only ? file : std::cout;
  regfile << std::hex << "IR  : " << cpu.IR << std::endl;
  regfile << "AC  : " << cpu.AC << std::endl;
  regfile << "AR  : " << cpu.AR << std::endl;
  regfile << "PC  : " << cpu.PC << std::endl;
  regfile << "E   : " << cpu.E << std::endl;
  regfile << "HLT : " << cpu.hlt << std::endl;
  if (!stdio_only && file.is_open()) {
    file.close();
  }
}
auto file_io::ram_to_file() {
  std::string formattedstr;
  std::string loc_ram_state;
  std::ofstream file(output_file);
  auto &output = stdio_only ? file : std::cout;
  if(stdio_only){
    output<<"\t  RAM  \t "<<(all_ram_loc?"[  all  ]":"[not all]")<<std::endl;
  }
  output << "[LOC ]\t[HEX]\t[DECI]\n";
  for (int loc = 0; loc < memory::ram_size; loc++) {
    if (ram[loc]) {
      formattedstr =
          (loc / 10 ? loc / 100 ? loc / 1000 ? "" : "0" : "00" : "000");
      output << "[" << formattedstr << loc << "]\t";
      formattedstr =
          (ram[loc] / 16 ? (ram[loc] / 256 ? ram[loc] / 4096 ? "" : "0" : "00") : "000");
      output << std::hex << formattedstr << ram[loc] << "\t";
      output << std::dec;
      formattedstr =
          (ram[loc] / 10
               ? ram[loc] / 100
                     ? ram[loc] / 1000 ? ram[loc] / 10000 ? "" : "0" : "00": "000": "0000");
      output << formattedstr << ram[loc] << "\n";
    } else if (all_ram_loc) {
      output << "[" << loc << "]" << "0000";
    }
  }
  (!stdio_only && file.is_open()) ? file.close() : void();
  // close file if file open and not stdio mode
}

auto file_io::load_instruction_to_ram() {
  uint16_t curr_mem_pointer = 0;
  auto file = std::fstream(hexcode_file);
  std::istream &input = stdio_only ? static_cast<std::istream&>(file) : static_cast<std::istream&>(hexcode_stream);
    while ((input>>generalstr)) {
        if (generalstr.starts_with('O')) {
          std::istringstream temp(generalstr);
          temp >> generalstr;
          if (generalstr == "ORG" || generalstr == "org") {
            temp >> std::hex >> generaluint16;//set location counter to ORG
            temp >> std::dec;
          }
          curr_mem_pointer =
              generaluint16 >= memory::ram_size - 1
                  ? throw std::out_of_range("Origin can not be at last index!!")
                  : generaluint16;
          continue;
        }
        try {
          generaluint16 =
              static_cast<uint16_t>(std::stoi(generalstr, &generalulong, 16));
          ram[curr_mem_pointer++] = generaluint16;
          if (generalulong != 4) {
            std::println(
                "Not Hex Value at Line : {},index :{} ,value found :{}",
                curr_mem_pointer, generalulong, generalstr);
          }
        } catch (std::exception e) {
          file.close();
          std::cout << "Error while parsing text !!" << std::endl
                    << e.what() << std::endl;
          break;
    }
  }
}
 
auto file_io::set_files_nm(std::string filenm,filetype type)->void
{
    switch (type) {
        case REGISTER_FILE:
        register_file=filenm;
        break;
        case INPUT_FILE:
            if(!std::filesystem::exists(filenm))
            {
                throw std::string("This File does not Exist : filenm");
            }
            input_file=filenm;
            break;
        case OUTPUT_FILE_SHOW_ALL:
            output_file=filenm;
            all_ram_loc=true;
            break;
        case OUTPUT_FILE:
            output_file=filenm;
            break;
        case HEXCODE_FILE:
            hexcode_file=filenm;
            break;
        case ERROR_FILE:
            error_file=filenm;
            break;
        default:
            std::println("Nani???IS this possible");
        }
        
}

auto file_io::input_from_file()
{
    std::string filenm=input_file;
    if(stdio_only)
    {
        std::println("Enter Your assembly Code Below : (chatgpt says ctrl+D to exit idk bro i fool in this point )");
        std::stringstream stdinput;
        while(std::getline(std::cin,generalstr))
        {
            stdinput<<generalstr<<std::endl;
        }
    }
}