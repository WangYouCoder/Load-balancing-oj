#include "compile_run.hpp"
using namespace WY_compile_run;

int main()
{
    std::string in_json;

    Json::Value in_value;

    in_value["code"] = R"(#include <iostream>
                            int main()
                            {
                                std::cout << "hello" << std::endl; 
                                while(1);
                                return 0;
                            })";
    in_value["input"] = "";
    in_value["cpu_limit"] = 1;
    in_value["mem_limit"] = 10240 * 5;

    Json::StyledWriter writer;
    in_json = writer.write(in_value);

    std::string out;
    CompileRun::Start(in_json, &out);
    std::cout << out << std::endl;
    return 0;
}