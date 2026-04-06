#ifndef CLI_PARSER_MANO
#define CLI_PARSER_MANO

#include "mymemory.hpp"
#include "file_handler.hpp"
#include <string>
class parser
{
    private:
    simulator  cpu;
    memory ram;
    file_io file_handler;
    std::string input_file;
    std::string output_file;
    std::string error_file;
    std::string register_file;
    int input_order;
    int requested_input_cnt;
    auto  static help();
    auto list_commands(int options);
    auto error(std::string);
    public:
    parser(int kwargs,char *args[]);
};

#endif