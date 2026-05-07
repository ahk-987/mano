#ifndef CLI_PARSER_MANO
#define CLI_PARSER_MANO

#include "mymemory.hpp"
#include "file_handler.hpp"
#include <string>
class parser
{
    private:
    memory ram;
    simulator  cpu;
    file_io file_handler;
    auto  static help();
    auto error(std::string);
    public:
    parser(int kwargs,char *args[]);
};

#endif