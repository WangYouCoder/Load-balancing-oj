#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <atomic>
#include <fstream>
#include <boost/algorithm/string.hpp>
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
            return AddSuffix(file_name, ".stdout");
        }

        static std::string Stderr(const std::string &file_name)
        {
            return AddSuffix(file_name, ".stderr");
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

        static std::string GetTimeMs()
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return std::to_string(tv.tv_usec * 1000 + tv.tv_sec / 1000);
        }
    };

    class FileUtil
    {
    public:
        static bool IsFileExist(const std::string &path_name)
        {
            // stat: 获得指定目录文件的属性，如果获取成功返回0，失败返回1
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0)
            {
                return true;
            }
            return false;
        }

        static std::string UniFileName()
        {
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtil::GetTimeMs();
            std::string file_name = ms + "_" + std::to_string(id);

            return file_name;
        }

        static bool WriteFile(const std::string &file_name, const std::string &code)
        {
            std::ofstream out(file_name);
            if (!out.is_open())
            {
                return false;
            }

            out.write(code.c_str(), code.size());
            out.close();
            return true;
        }

        static bool ReadFile(const std::string &file_name, std::string *out, bool keep = false)
        {
            (*out).clear();

            std::ifstream in(file_name);
            if (!in.is_open())
            {
                return false;
            }

            std::string line;
            while (getline(in, line))
            {
                (*out) += line;
                (*out) += (keep ? "\n" : "");
            }

            in.close();
            return true;
        }
    };

    class SplitUtil
    {
    public:
        static bool Split(std::string str, std::string seq, std::vector<std::string> *out)
        {
            boost::split((*out), str, boost::is_any_of(seq), boost::algorithm::token_compress_on);
            return true;
        }
    };
}