#include "compile.hpp"

using namespace WY_compiler;

int main()
{
    std::string file_name = "code";
    Compiler::Compile(file_name);
    return 0;
}