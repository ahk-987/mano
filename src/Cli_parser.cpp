#include "../header/Cli_parser.hpp"
#include <climits>
#include <cstring>
#include <print>

auto parser::error(std::string errorstr)
{
    std::println("Error : {}",errorstr);
}
auto parser::help()
{
    std::println("A simple Mano Simulator\n");
    std::println("-i \t [eg:assemblymano.txt] \t file with mano assembly");
    std::println("-o \t [eg:ramstate.txt] \t file to store ram state");
    std::println("\t-oa \t \t To show all ram (Normally only non zero locations)");
    std::println("-r \t [eg:register.txt] \t file to store register state");
    std::println("-c \t [eg:hexcode.txt] \t file to store hexcode genereated from assembly file");
    // std::println("-e \t [error_store.txt] \t file to store errors? ");
    std::println("--stdio-only \t \t force all IO to be in stdin and stdio only");
    std::println("-h / --help \t \t See this help");
    std::println("Currently IO operation is not supported");
    std::println("Use args as -arg PATH eg : -i PATH/TO/File.txt \n Use .txt as extensions\n");
}
parser::parser(int kwargs, char *args[])
{
    ram=memory();
    cpu=simulator(0,ram);
    file_handler=file_io(cpu,ram);
    bool force_break=false;
    for(int i = 0;i<kwargs;i++)
    {
        if(force_break)
        {
            break;
        }
        if(args[i][0]=='-')
        {
            switch(args[i][1])
            {
                case 'i':
                    if(args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::INPUT_FILE);
                    }
                    i++;
                    break;
                    /* 
                        next argument is taken as input file
                    */
                case 'o':
                 if(args[i+1][0]!='-')
                    {
                        if(args[i][2]=='a')
                        {
                            file_handler.set_files_nm(args[++i], file_io::OUTPUT_FILE_SHOW_ALL);
                        }
                        else{
                            file_handler.set_files_nm(args[++i], file_io::OUTPUT_FILE);
                        }
                    }
                    break;
                    
                case 'r':
                     if(args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::REGISTER_FILE);
                    }
                    break;
                    
                case 'e'://not properly setup yet but future ig idk?
                     if(args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::ERROR_FILE);
                    }
                    break;
                case 'c': 
                    if(args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::HEXCODE_FILE);
                    }
                    break;
                case 'h':
                    help();
                    force_break=true;
                    break;
                case '-':
                    if (std::strcmp(args[i],"--stdio-only"))
                    {
                        input_order=INT_MIN;
                    }
                    else if(std::strcmp(args[i],"--help"))
                    {
                        help();
                        force_break=true;
                    }
                    break;
                default:
                    help();
                    force_break=true;
            }
        }
        }   
}