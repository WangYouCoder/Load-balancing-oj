#include "compile_run.hpp"
#include "../comm/httplib.h"

using namespace WY_compile_run;
using namespace httplib;

void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 0)
    {
        Usage(argv[0]);
        return -1;
    }

    Server svr;

    svr.Post("/compile_run", [](const Request &req, Response &resp)
             {
                 std::string in_json = req.body;
                 std::string out_json;
                 if (!in_json.empty())
                 {
                     CompileRun::Start(in_json, &out_json);
                     resp.set_content(out_json, "application/json;charset=utf-8");
                 }
             });

    svr.listen("0.0.0.0", 8080);

    // std::string in_json;

    // Json::Value in_value;

    // in_value["code"] = R"(#include <iostream>
    //                         int main()
    //                         {
    //                             std::cout << "hello" << std::endl;
    //                             while(1);
    //                             return 0;
    //                         })";
    // in_value["input"] = "";
    // in_value["cpu_limit"] = 1;
    // in_value["mem_limit"] = 10240 * 5;

    // Json::StyledWriter writer;
    // in_json = writer.write(in_value);

    // std::string out;
    // CompileRun::Start(in_json, &out);
    // std::cout << out << std::endl;
    return 0;
}