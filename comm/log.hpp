#pragma once

#include <iostream>
#include <string>
#include "util.hpp"
namespace WY_log
{
    using namespace WY_util;
    enum
    {
        Info,
        Debug,
        Warnning,
        Error,
        Fatal
    };

    inline std::ostream& Log(const std::string &level, const std::string &file_name, int line)
    {
        // 添加日志等级
        std::string message = "[";
        message += level;
        message += "]";

        // 添加文件名
        message += "[";
        message += file_name;
        message += "]";

        // 添加报错行
        message += "[";
        message += std::to_string(line);
        message += "]";

        // 添加时间戳
        message += "[";
        message += TimeUtil::GetTimeStamp();
        message += "]";

        std::cout << message;  

        return std::cout;
    }

    #define LOG(level) Log(#level, __FILE__, __LINE__) 

}