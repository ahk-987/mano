#include "../header/Cli_parser.hpp"
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
parser::parser(int kwargs, char *args[]):ram(),cpu(0,ram),file_handler(cpu,ram)
{
    bool improper_args=false;
    bool force_break=false;
    if(kwargs<2) return;
    for(int i = 1;i<kwargs;i++)
    {
        std::println("IN loop {}",args[i]);
        if(force_break)
        {
            break;
        }
        if(args[i][0]=='-')
        {
            switch(args[i][1])
            {
                case 'i':
                    if(i+1<kwargs&&args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::INPUT_FILE);
                    }
                    else{
                        improper_args=true;
                        std::println("Missing Argument for input file");
                    }
                    break;
                    /* 
                        next argument is taken as input file
                    */
                case 'o':
                    if(i+1<kwargs&&args[i+1][0]!='-')
                    {
                        if(strlen(args[i])>2 && args[i][2]=='a')
                        {
                            file_handler.set_files_nm(args[++i], file_io::OUTPUT_FILE_SHOW_ALL);
                        }
                        else{
                            file_handler.set_files_nm(args[++i], file_io::OUTPUT_FILE);
                        }
                    }
                    else{
                        improper_args=true;
                        std::println("Missing Argument for output file");
                    }
                    break;
                    
                case 'r':
                    if(i+1<kwargs&&args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::REGISTER_FILE);
                    }
                    else{
                        improper_args=true;
                        std::println("Missing Argument for registry file");
                    }
                    break;
                    
                case 'e'://not properly setup yet but future ig idk?
                    if(i+1<kwargs&&args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::ERROR_FILE);
                    }
                    else{
                        improper_args=true;
                        std::println("Missing Argument for error file");
                    }
                    break;
                case 'c': 
                    if(i+1<kwargs&&args[i+1][0]!='-')
                    {
                        file_handler.set_files_nm(args[++i], file_io::HEXCODE_FILE);
                    }
                    else{
                        improper_args=true;
                        std::println("Missing Argument for Hexcode file");
                    }
                    break;
                case 'h':
                    help();
                    force_break=true;
                    break;
                case '-':
                    if (std::strcmp(args[i],"--stdio-only")==0)
                    {
                        file_handler.set_stdio(true);
                    }
                    else if(std::strcmp(args[i],"--help")==0)
                    {
                        help();
                        force_break=true;
                    }
                    else{
                        std::println("Improper Argument: {}",args[i]);
                        help();
                        force_break=true;
                    }
                    break;
                default:
                    help();
                    force_break=true;
            }
        }
        else{
            help();
            force_break=true;
        }
    }   
    if(improper_args)
    {
        std::println("Errors in arguments cant execute");
    }
    else if (!force_break) {    
        file_handler.run();
    }
}