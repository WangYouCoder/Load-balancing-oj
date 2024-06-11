#pragma once

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "compile.hpp"
#include "runner.hpp"
namespace WY_compile_run
{
    using namespace WY_util;
    using namespace WY_compile;
    using namespace WY_run;
    class Compile_Run
    {
        static void Start(const std::string &in_json, std::string *out)
        {
            Json::Value ret;
            Json::Reader read;
            read.parse(in_json, ret);

            std::string code = ret["code"].asString();
            std::string input = ret["input"].asString();

            if(code.size() == 0)
            {
                // 差错处理
            }

            int _rlimit_cpu = ret["_rlimit_cpu"].asInt();
            int _rlimit_mem = ret["_rlimit_mem"].asInt();

            std::string file_name = FileUtil::UniFileName();

            FileUtil::WriteFile(PathUtil::Src(file_name), code);

            Compiler::Compile(file_name);

            Runer::Run(file_name, _rlimit_cpu, _rlimit_mem);
        }
    };
}