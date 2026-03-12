#ifndef CLI_PARSER_MANO
#define CLI_PARSER_MANO

#include "mymemory.hpp"
#include <exception>
#include <string>
class parser
{
    private:
    std::string input_file;
    std::string output_file;
    std::string error_file;
    std::string register_file;
    auto help();
    auto chk_file_exists(std::string filenm)
    ->bool;
    auto list_commands(int options);
    auto error();
    public:
    parser(int kwargs,char *args[]);
    auto run(bool debug = false);    
};

#endif