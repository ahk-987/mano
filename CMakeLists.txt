cmake_minimum_required(VERSION 4.2.1 FATAL_ERROR )
project(mano
    VERSION 0.0.1
    LANGUAGES CXX ASM
    DESCRIPTION "mano"
)
add_executable(mano.out)

target_sources(mano.out PRIVATE
    src/memory.cpp
    src/cpu.cpp
    src/main.cpp
    src/Cli_parser.cpp
)
target_compile_features ( mano.out PRIVATE
    cxx_std_26
)

target_compile_options ( mano.out PRIVATE
    $<$<CXX_COMPILER_ID:Clang>: 
    # this now only works for the clang compiler
        
        #-Werror
        #-Weverything
        -Wno-c++98-compat # disable the warning for C++98 compatibility
        -Wno-c++98-compat-pedantic # disable the warning for c++98 violations
    >
    $<$<CXX_COMPILER_ID:GCC,GNU>: # this now only works for the GCC compiler
        -Werror
        -Wall
        -Wpedantic
    >
)
