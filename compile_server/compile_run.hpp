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
    class CompileRun
    {
        static void RemoveTempFile(const std::string &file_name)
        {
            if (FileUtil::IsFileExist(PathUtil::Exc(file_name)))
                unlink(PathUtil::Exc(file_name).c_str());
                
            if (FileUtil::IsFileExist(PathUtil::Compile_error(file_name)))
                unlink(PathUtil::Compile_error(file_name).c_str());

            if (FileUtil::IsFileExist(PathUtil::Src(file_name)))
                unlink(PathUtil::Src(file_name).c_str());

            if (FileUtil::IsFileExist(PathUtil::Stderr(file_name)))
                unlink(PathUtil::Stderr(file_name).c_str());

            if (FileUtil::IsFileExist(PathUtil::Stdin(file_name)))
                unlink(PathUtil::Stdin(file_name).c_str());

            if (FileUtil::IsFileExist(PathUtil::Stdout(file_name)))
                unlink(PathUtil::Stdout(file_name).c_str());
        }

        static std::string toString(const Json::Value &val)
        {
            static Json::Value def = []()
            {
                Json::Value def;
                Json::StreamWriterBuilder::setDefaults(&def);
                def["emitUTF8"] = true;
                return def;
            }();

            std::ostringstream stream;
            Json::StreamWriterBuilder stream_builder;
            stream_builder.settings_ = def; // Config emitUTF8
            std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
            writer->write(val, &stream);
            return stream.str();
        }

    public:
        static std::string StaToRea(int code, const std::string &file_name)
        {
            std::string dst;
            switch (code)
            {
            case 0:
                dst = "编译运行成功";
                break;
            case -1:
                dst = "代码为空";
                break;
            case -2:
                dst = "未知错误";
                break;
            case -3:
                // dst = "编译失败";
                FileUtil::ReadFile(PathUtil::Compile_error(file_name), &dst, true);
                break;
            case SIGXCPU:
                dst = "cpu使用超时";
                break;
            case SIGABRT:
                dst = "内存超出范围";
                break;
            default:
                dst = "未知错误, 错误码是" + std::to_string(code);
            }
            return dst;
        }

        static void Start(const std::string &in_json, std::string *out)
        {
            Json::Value ret;
            Json::Reader read;
            read.parse(in_json, ret);

            std::string code = ret["code"].asString();
            std::string input = ret["input"].asString();
            int _rlimit_cpu = ret["cpu_limit"].asInt();
            int _rlimit_mem = ret["mem_limit"].asInt();

            int status_code = 0;
            int result_code = 0;
            std::string file_name;

            Json::Value out_value;

            if (code.size() == 0)
            {
                status_code = -1; // 提交的代码为空
                goto END;
            }

            file_name = FileUtil::UniFileName();

            if (!FileUtil::WriteFile(PathUtil::Src(file_name), code))
            {
                status_code = -2; // 写入文件失败
                goto END;
            }

            if (!Compiler::Compile(file_name))
            {
                status_code = -3; // 编译失败
                goto END;
            }

            result_code = Runer::Run(file_name, _rlimit_cpu, _rlimit_mem);
            if (result_code == 0)
            {
                status_code = 0;
                // 编译运行成功
            }
            else if (result_code < 0)
            {
                // 未知错误
                status_code = result_code;
            }
            else
            {
                // 信号量
                status_code = result_code;
            }
        END:
            out_value["status"] = status_code;
            out_value["reason"] = StaToRea(status_code, file_name);

            if (status_code == 0)
            {
                std::string _stdout;
                FileUtil::ReadFile(PathUtil::Stdout(file_name), &_stdout, true);
                out_value["stdout"] = _stdout;

                std::string _stderr;
                FileUtil::ReadFile(PathUtil::Stderr(file_name), &_stderr, true);
                out_value["stderr"] = _stderr;
            }

            // Json::StyledWriter write;
            // *out = write.write(out_value);

            *out = toString(out_value);

            RemoveTempFile(file_name);
        }
    };
}