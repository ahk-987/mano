#include "../header/file_handler.hpp"
#include <algorithm>
#include <cctype>
#include <charconv>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <print>
#include <sstream>
#include <string>
#include <system_error>
#include <unordered_map>

const std::unordered_map<std::string, uint16_t> file_io::MRI = {

        {"AND",0x0},
        {"ADD",0x1000},
        {"LDA",0x2000},
        {"STA",0x3000},
        {"BUN",0x4000},
        {"BSA",0x5000},
        {"ISZ",0x6000}
};

const std::unordered_map<std::string, uint16_t> file_io::NON_MRI = {
        
        {"CLA",0x7800},
        {"CLE",0x7400},
        {"CMA",0x7200},
        {"CME",0x7100},
        {"CIR",0x7080},
        {"CIL",0x7040},
        {"INC",0x7020},
        {"SPA",0x7010},
        {"SNA",0x7008},
        {"SZA",0x7004},
        {"SZE",0x7002},
        {"HLT",0x7001},


        {"INP",0xF800},
        {"OUT",0xF400},
        {"SKI",0xF200},
        {"SKO",0xF100},
        {"ION",0xF080},
        {"IOF",0xF040}
    };
file_io::file_io(simulator cpu, memory ram) {
  this->cpu = cpu;
  this->ram = ram;
  input_file = hexcode_file = output_file = register_file =
      ""; // empty intitalise
}

file_io::file_io() {
  /*Idk why there was some wierd issues of sorts so creating empty constructor
   * :(*/
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
  if (stdio_only) {
    output << "\t  RAM  \t " << (all_ram_loc ? "[  all  ]" : "[not all]")
           << std::endl;
  }
  output << "[LOC ]\t[HEX]\t[DECI]\n";
  for (int loc = 0; loc < memory::ram_size; loc++) {
    if (ram[loc]) {
      formattedstr =
          (loc / 10 ? loc / 100 ? loc / 1000 ? "" : "0" : "00" : "000");
      output << "[" << formattedstr << loc << "]\t";
      formattedstr =
          (ram[loc] / 16 ? (ram[loc] / 256 ? ram[loc] / 4096 ? "" : "0" : "00")
                         : "000");
      output << std::hex << formattedstr << ram[loc] << "\t";
      output << std::dec;
      formattedstr =
          (ram[loc] / 10
               ? ram[loc] / 100
                     ? ram[loc] / 1000 ? ram[loc] / 10000 ? "" : "0" : "00"
                     : "000"
               : "0000");
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
  std::istream &input = stdio_only
                            ? static_cast<std::istream &>(file)
                            : static_cast<std::istream &>(hexcode_stream);
  while ((input >> generalstr)) {
    if (generalstr.starts_with('O')) {
      std::istringstream temp(generalstr);
      temp >> generalstr;
      if (generalstr == "ORG" || generalstr == "org") {
        temp >> std::hex >> generaluint16; // set location counter to ORG
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
        std::println("Not Hex Value at Line : {},index :{} ,value found :{}",
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

auto file_io::set_files_nm(std::string filenm, filetype type) -> void {
  switch (type) {
  case REGISTER_FILE:
    register_file = filenm;
    break;
  case INPUT_FILE:
    if (!std::filesystem::exists(filenm)) {
      throw std::string("This File does not Exist : filenm");
    }
    input_file = filenm;
    break;
  case OUTPUT_FILE_SHOW_ALL:
    output_file = filenm;
    all_ram_loc = true;
    break;
  case OUTPUT_FILE:
    output_file = filenm;
    break;
  case HEXCODE_FILE:
    hexcode_file = filenm;
    break;
  case ERROR_FILE:
    error_file = filenm;
    break;
  default:
    std::println("Nani???IS this possible");
  }
}

auto file_io::input_from_file() {
  std::string generalstr_fpass,filenm = input_file;
  std::stringstream stdinput; // store input from cli for future parsing
  std::stringstream hexcode_stream; // stores hexcode_stream in case of stdio_only case
  auto assembly_file_stream = std::ifstream(filenm);
  if (stdio_only) // get user input and save it to a stream for doing parsing
  {
    std::println("Enter Your assembly Code Below : (chatgpt says ctrl+D to "
                 "exit idk bro i fool in this point )");
    while (std::getline(std::cin, generalstr)) {
      stdinput << generalstr << std::endl;
    }
    stdinput.clear();  // to set it clean
    stdinput.seekg(0); // To point again to start for reading purposes
  }
  std::istream &assembly =
      (stdio_only) ? static_cast<std::istream&>(stdinput)
                   : static_cast<std::istream&>(assembly_file_stream);
  

  /*First Pass of assembler*/
    int curr_mem_pointer=0;
    bool end_pass_bool=true;
    std::unordered_map<std::string, int> labels;
    std::stringstream str_line_stream;
    while(std::getline(assembly,generalstr_fpass)&&end_pass_bool)
    {
        std::transform(generalstr_fpass.begin(),generalstr_fpass.end(),generalstr_fpass.begin(),[](unsigned char c ){return std::tolower(c);});
        //This is to not mind any content after comment sign ";" 
        int loc_str_tmp=generalstr_fpass.find(';');
        loc_str_tmp!=std::string::npos?generalstr_fpass.erase(loc_str_tmp):"";//"" for ternary operation
        //Above remove anything after comment

  
        str_line_stream=std::stringstream(generalstr_fpass);

        while(end_pass_bool&&(str_line_stream>>generalstr_fpass))
        {
          if(!(MRI.contains(generalstr_fpass)&&(NON_MRI.contains(generalstr_fpass))))
          {
            if(generalstr_fpass=="ORG")
            {
              size_t pos;
              int templocint;
              str_line_stream>>generalstr_fpass;
              auto [temp_ptr_fpass,temp_ec_fpass]=std::from_chars(generalstr_fpass.data(),generalstr_fpass.data()+generalstr_fpass.size(),templocint,16);
              //Here i am checking if address passed in is in format or like proper 
              if((temp_ec_fpass!=std::errc())&&temp_ptr_fpass-generalstr_fpass.data()!=generalstr_fpass.size()&&(templocint>0xFFFE))
              {
                std::println("Error invalid Address at index : {}",curr_mem_pointer);
              }
              else{
                curr_mem_pointer=templocint;
              }
            }
            else if(generalstr_fpass=="END")
            {
              end_pass_bool=false;
            }
            else {
              bool chk_label_add_loc=false;
              if(generalstr_fpass.ends_with(',')){
                generalstr_fpass.pop_back();
                chk_label_add_loc=true;
              }
              else{
                std::string tempstr;
                auto temp_pos=str_line_stream.tellg();
                str_line_stream>>tempstr;
                if(tempstr==","){
                  chk_label_add_loc=true;
                }
                else{
                  str_line_stream.seekg(temp_pos);
                }
              }
              // if(!labels.contains(generalstr_fpass)){
              //   labels["generalstr_fpass"]=-1;
              // }
              if(chk_label_add_loc)
              {
                labels[generalstr_fpass]=curr_mem_pointer;
              }
              else if(!labels.contains(generalstr_fpass)){
                labels[generalstr_fpass]=(-1);
                //If not yet stored then set to -1 to help flag if never set
              }
            }

          }
        }

       curr_mem_pointer++;//increase LC 
    }
    /*End First Pass */
    

}