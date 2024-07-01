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

    svr.Post(R"(/judge(\d+))",[&cont](const Request &req, Response &resp){
        std::string number = req.matches[1];
        std::string in_json = req.body;
        std::string out_json;
        cont.Judge(number, in_json, &out_json);
        resp.set_content(out_json, "application/json;charset=utf-8");
    });
    svr.set_base_dir("./wwwroot");

    svr.listen("0.0.0.0", 8080);
    return 0;
}