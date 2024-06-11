#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
namespace WY_run
{
    using namespace WY_util;
    using namespace WY_log;
    class Runer
    {
        static void Sourcelimit(int _rlimit_cpu, int _rlimit_mem)
        {
            struct rlimit rlimit_cpu;
            rlimit_cpu.rlim_max = RLIM_INFINITY;
            rlimit_cpu.rlim_cur = _rlimit_cpu;
            setrlimit(RLIMIT_CPU, &rlimit_cpu);

            struct rlimit rlimit_mem;
            rlimit_mem.rlim_max = RLIM_INFINITY;
            rlimit_mem.rlim_cur = _rlimit_mem * 1024; // 转化为kb
            setrlimit(RLIMIT_AS, &rlimit_mem);

        }
    public:
        static int Run(const std::string &file_name, int rlimit_cpu/*程序最大执行时间*/, int rlimit_mem /*KB*/)
        {
            int statue = 0;

            int _stdin = open(PathUtil::Stdin(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
            int _stdout = open(PathUtil::Stdout(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
            int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
            if((_stdin < 0) | (_stdout < 0) | (_stderr < 0))
            {
                LOG(Error) << "运行时打开文件失败\n";
                return -1;
            }

            pid_t pid = fork();
            if (pid < 0)
            {
                close(_stdin);
                close(_stdout);
                close(_stderr);
                LOG(Error) << "运行时创建子进程失败\n";
                return -1;
            }
            else if (pid == 0)
            {
                dup2(_stdin, 0);
                dup2(_stdout, 1);
                dup2(_stderr, 2);

                Runer::Sourcelimit(rlimit_cpu, rlimit_mem);
                execl(PathUtil::Exc(file_name).c_str(), PathUtil::Exc(file_name).c_str(), nullptr);
                exit(1);
            }
            else
            {
                close(_stdin);
                close(_stdout);
                close(_stderr);
                // statue & 0x7f
                waitpid(pid, &statue, 0);
                LOG(Info) << "程序执行的退出信息: " << (statue & 0x7f) << "\n";
                return statue & 0x7f;
            }
        }
    };
}