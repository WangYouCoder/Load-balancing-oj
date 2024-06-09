#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../comm/util.hpp"
#include "../comm/log.hpp"

namespace WY_compiler
{
    using namespace WY_util;
    using namespace WY_log;
    class Compiler
    {
    public:
        Compiler()
        {
        }

        static bool Compile(const std::string &file_name)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                LOG(Error) << "创建子进程失败\n";
                return false;
            }
            else if (pid == 0)
            {
                int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if(_stderr < 0)
                {
                    LOG(Warnning) << "没有成功形成stderr文件\n";
                    exit(1);
                }
                dup2(_stderr, 2);

                execlp("g++", "g++", "-o", PathUtil::Exc(file_name).c_str(), PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr);
                LOG(Error) << "execlp程序替换失败,可能是参数错误\n";
                exit(2);
            }
            else
            {
                waitpid(pid, nullptr, 0);
                if(FileUtil::IsFileExist(PathUtil::Exc(file_name)))
                {
                    LOG(Info) << PathUtil::Src(file_name).c_str() << " 编译成功\n";
                    return true;
                }
            }
            LOG(Debug) << PathUtil::Src(file_name).c_str() << "\n";
            LOG(Error) << " 编译失败，没有形成可执行程序\n";
            return false;
        }
        ~Compiler()
        {
        }
    };
}
