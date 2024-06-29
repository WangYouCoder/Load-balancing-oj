#include <iostream>
#include "../comm/httplib.h"
#include "oj_control.hpp"
using namespace httplib;
using namespace WY_control;
int main()
{
    Server svr;
    Control cont;
    svr.Get("/all_questions",[&cont](const Request &req, Response &resp){
        std::string html;
        cont.AllQuestion(&html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    svr.Get(R"(/question/(\d+))",[&cont](const Request &req, Response &resp){
        std::string number = req.matches[1];
        std::string html;
        cont.OneQuestgion(number, &html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    svr.Get(R"(/judge(\d+))",[](const Request &req, Response &resp){
        std::string numble = req.matches[1];
        resp.set_content("这是指定一道题的判题" + numble, "text/plai;charset=utf-8");
    });
    svr.set_base_dir("./wwwroot");

    svr.listen("0.0.0.0", 8080);
    return 0;
}