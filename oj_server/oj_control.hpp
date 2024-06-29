#pragma once
#include <iostream>
#include <string>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
namespace WY_control
{
    using namespace WY_model;
    using namespace WY_view;
    class Control
    {
    private:
        Model _model;
        View _view;
    public:
        bool AllQuestion(std::string *html)
        {
            bool ret = true;
            std::vector<Question> v;
            if(_model.GetAllQuestion(&v))
            {
                _view.AllExpandHtml(v, html);
            }
            else
            {
                *html = "获取题目列表失败";
                ret = false;
            }
            return ret;
        }

        bool OneQuestgion(const std::string &number, std::string *html)
        {
            Question q;
            bool ret = true;
            if(_model.GetOnlyOneQuestion(number, &q))
            {
                _view.OneExpandHtml(q, html);
            }
            else
            {
                *html = "题号: " + number + "不存在";
                ret = false;
            }
            return ret;
        }
    };
};