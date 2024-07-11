#pragma once

#include <iostream>
#include <string>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "oj_model.hpp"
// #include "oj_model2.hpp"
#include <ctemplate/template.h>
namespace WY_view
{
    using namespace WY_model;
    const std::string template_path = "./template_html/";

    class View
    {
    public:
        View() {}
        ~View() {}
        void AllExpandHtml(const std::vector<Question> &questions, std::string *html)
        {
            // 1. 形成路径
            std::string src_html = template_path + "all_questions.html";
            // 2. 形成数据字典
            ctemplate::TemplateDictionary root("all_question");
            for(const auto& q : questions)
            {
                ctemplate::TemplateDictionary *sub = root.AddSectionDictionary("question_list");
                sub->SetValue("number", q.number);
                sub->SetValue("title", q.title);
                sub->SetValue("star", q.star);
            }
            // 3. 获取被渲染的html
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP/*保持原貌, 不去除空行*/);

            // 4. 开始渲染
            tpl->Expand(html, &root);
        }

        void OneExpandHtml(const Question &q, std::string *html)
        {
            std::string src_html = template_path + "one_question.html";
            ctemplate::TemplateDictionary root("one_question");
            root.SetValue("number", q.number);
            root.SetValue("title", q.title);
            root.SetValue("star", q.star);
            root.SetValue("desc", q.desc);
            root.SetValue("pre_code", q.header);
            ctemplate::Template *tpl = ctemplate::Template::GetTemplate(src_html, ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &root);
        }
    };
};