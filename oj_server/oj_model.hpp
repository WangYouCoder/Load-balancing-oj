#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <cassert>
#include "../comm/util.hpp"
#include "../comm/log.hpp"
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
        int cpu_limit;
        int mem_limit;
        std::string desc;
        std::string header;
        std::string tail;
    };

    const std::string question_path = "./question/";
    class Model
    {
    private:
        std::unordered_map<std::string, Question> questions;

    public:
        Model()
        {
            assert(LoadQuestionList(question_path));
        }

        bool LoadQuestionList(const std::string &question_list)
        {
            std::string path = question_list + "question.list";
            std::ifstream in(path);
            if (!in.is_open())
            {
                LOG(Fatal) << " 加载题库失败,请检查题库文件是否存在\n";
                return false;
            }

            std::string line;
            while (getline(in, line))
            {
                std::vector<std::string> v;
                SplitUtil::Split(line, " ", &v);
                if (v.size() != 5)
                {
                    LOG(Warnning) << " 加载部分题目失败, 请检查题目题目格式\n";
                    continue;
                }

                Question q;
                q.number = v[0];
                q.title = v[1];
                q.star = v[2];
                q.cpu_limit = atoi(v[3].c_str());
                q.mem_limit = atoi(v[4].c_str());

                std::string path = question_path + v[0] + "/";
                FileUtil::ReadFile(path + "header.hpp", &q.header, true);
                FileUtil::ReadFile(path + "tail.hpp", &q.tail, true);
                FileUtil::ReadFile(path + "question.desc", &q.desc, true);

                questions.insert({q.number, q});
            }
            
            LOG(Info) << " 加载题库成功...\n";
            in.close();
            return true;
        }

        bool GetAllQuestion(std::vector<Question> *out)
        {
            if (questions.size() == 0)
            {
                LOG(Error) << " 获取所有题目失败\n";
                return false;
            }
            for (const auto &q : questions)
            {
                out->push_back(q.second);
            }
            return true;
        }

        bool GetOnlyOneQuestion(const std::string &numble, Question *q)
        {
            auto it = questions.find(numble);
            if (it == questions.end())
            {
                LOG(Error) << " 获取单个题目失败\n";
                return false;
            }
            (*q) = it->second;
            return true;
        }
        ~Model()
        {
        }
    };
};