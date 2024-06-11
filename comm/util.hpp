#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

namespace WY_util
{
    class PathUtil
    {
        const std::string temp_path = "./tmp/";

    public:
        // code(文件名) ---> ./tmp/code.cpp  ./tmp/code.exe  ./tmp/code.stderr
        static std::string AddSuffix(const std::string &file_name, const std::string &suffix)
        {
            // std::string path_name = temp_path;
            std::string path_name = "./tmp/";
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        // 编译
        static std::string Src(const std::string &file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }

        static std::string Exc(const std::string &file_name)
        {
            return AddSuffix(file_name, ".exe");
        }

        static std::string Compile_error(const std::string &file_name)
        {
            return AddSuffix(file_name, ".compile_error");
        }

        static std::string Stdin(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }

        static std::string Stdout(const std::string &file_name)
        {
            return AddSuffix(file_name, ".Stdout");
        }

        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
    };

    class FileUtil
    {
    public:
        static bool IsFileExist(const std::string &path_name)
        {
            // stat: 获得指定目录文件的属性，如果获取成功返回0，失败返回1
            struct stat st;
            if(stat(path_name.c_str(), &st) == 0)
            {
                return true;
            }
            return false;
        }

        static std::string UniFileName()
        {
            return "";
        }

        static bool WriteFile(const std::string &file_name, const std::string code)
        {

        }

        static bool ReadFile(const std::string &file_name, std::string *out, bool keep = false)
        {
            
        }
    };

    class TimeUtil
    {
    public:
        static std::string GetTimeStamp()
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return std::to_string(tv.tv_sec);
        }
    };
}