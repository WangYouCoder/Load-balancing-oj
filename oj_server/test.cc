// #include "oj_model.hpp"
// using namespace WY_model;


// int main()
// {
//     Question q;
//     Model m;
//     m.GetOnlyOneQuestion("1", &q);
//     std::cout << q.desc << std::endl;
// }


#include <ctemplate/template.h>
#include <iostream>
#include <string>

int main()
{
    std::string in_html = "./test.html";
    std::string value = "hello word";

    // 形成数据字典
    ctemplate::TemplateDictionary root("test"); // 类比成 unordered_map<> test
    root.SetValue("key", value);                // test.insert({"key", value})

    // 获取被渲染对象
    ctemplate::Template *tpl = ctemplate::Template::GetTemplate(in_html, ctemplate::DO_NOT_STRIP/*保持原貌, 不去除空行*/);

    // 添加数据字典到网页中
    std::string out_html;
    tpl->Expand(&out_html, &root);

    // 完成渲染
    std::cout << out_html << std::endl;
    return 0;
}
