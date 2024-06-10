#include "compile.hpp"
#include "runner.hpp"

using namespace WY_compiler;
using namespace WY_run;
int main()
{
    std::string file_name = "code";
    Compiler::Compile(file_name);
    Runer::Run(file_name);
    return 0;
}