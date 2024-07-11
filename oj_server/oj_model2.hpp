#pragma once

//  MYSQL 版本
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cassert>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
#include <mysql/mysql.h>
using namespace WY_util;
using namespace WY_log;
namespace WY_model
{
    class Question
    {
    public:
        std::string number;
        std::string title;
        std::string star;
        std::string desc;
        std::string header;
        std::string tail;
        int cpu_limit;
        int mem_limit;
    };

    const std::string oj_questions = "oj_questions";
    const std::string host = "127.0.0.1";
    const std::string user = "oj_client";
    const std::string passwd = "li030413";
    const std::string db = "oj";
    const int port = 3306;
    class Model
    {

    public:
        Model()
        {
        }

        bool QueryMySql(const std::string &sql, std::vector<Question> *out)
        {
            // 创建句柄
            MYSQL *my = mysql_init(nullptr);

            // 连接数据库
            if(mysql_real_connect(my, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0) == nullptr)
            {
                LOG(Fatal) << " 数据库连接失败！" << "\n";
            }
            LOG(Info) << " 数据库连接成功！" << "\n";

            // 如果乱码，需要考虑连接的编码是否一致
            mysql_set_character_set(my, "utf8");

            // 执行数据库语句
            if(mysql_query(my, sql.c_str()) != 0)
            {
                LOG(Warnning) << sql << " execute error!" << "\n";
            }

            // 提取结果
            MYSQL_RES * res = mysql_store_result(my);

            // 分析结果
            int rows = mysql_num_rows(res);    // 获得行数量
            int fields = mysql_num_fields(res);// 获得列数量

            // std::cout << "rows: " << rows << " fields: " << fields << std::endl;

            for(int i = 0; i < rows; i++)
            {
                MYSQL_ROW/*char ** */ row = mysql_fetch_row(res);
                Question q;
                // -- CREATE TABLE IF NOT EXISTS `oj_questions`(
                // -- 	`number` int PRIMARY KEY AUTO_INCREMENT COMMENT '题目的ID',
                // -- 	`title` VARCHAR(64) NOT NULL COMMENT '题目的标题',
                // -- 	`star` VARCHAR(8) NOT NULL COMMENT '题目的难度',
                // -- 	`desc` TEXT NOT NULL COMMENT '题目描述',
                // -- 	`header` TEXT NOT NULL COMMENT '题目头部，给用户看的代码',
                // -- 	`tail` TEXT NOT NULL COMMENT '题目尾部，包含我们的测试用例',
                // -- 	`cpu_limit` int DEFAULT 1 COMMENT '题目的时间限制',
                // -- 	`mem_limit` int DEFAULT 500000 COMMENT '题目的空间限制'
                // -- )ENGINE=INNODB DEFAULT CHARSET=utf8;
                q.number = row[0];
                q.title = row[1];
                q.star = row[2];
                q.desc = row[3];
                q.header = row[4];
                q.tail = row[5];
                q.cpu_limit = std::stoi(row[6]);
                q.mem_limit  = std::stoi(row[7]);
                std::cout << q.cpu_limit << " " << q.mem_limit << std::endl;

                out->push_back(q);
            }

            // 释放结果空间
            mysql_free_result(res);
            // 关闭mysql连接
            mysql_close(my);
            return true;
        }

        bool GetAllQuestion(std::vector<Question> *out)
        {
            std::string sql = "select * from ";
            sql += oj_questions;
            return QueryMySql(sql, out);
        }

        bool GetOnlyOneQuestion(const std::string &number, Question *q)
        {
            bool res = false;
            std::string sql = "select * from ";
            sql += oj_questions;
            sql += " where number=";
            sql += number;
            std::vector<Question> result;
            if(QueryMySql(sql, &result))
            {
                if(result.size() == 1)
                {
                    *q = result[0];
                    res = true;
                }
            }
            return res;
        }
        ~Model()
        {
        }
    };
};