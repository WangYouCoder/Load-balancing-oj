#include <iostream>
#include "../comm/httplib.h"

using namespace httplib;

int main()
{
    Server svr;

    svr.Get("/all_question",[](const Request &req, Response &resp){
        resp.set_content("这是所有的题目列表", "text/plai;charset=utf-8");
    });

    svr.Get(R"(/question/(\d+))",[](const Request &req, Response &resp){
        std::string numble = req.matches[1];
        resp.set_content("这是指定的一道题" + numble, "text/plai;charset=utf-8");
    });

    svr.Get(R"(/judge(\d+))",[](const Request &req, Response &resp){
        std::string numble = req.matches[1];
        resp.set_content("这是指定一道题的判题" + numble, "text/plai;charset=utf-8");
    });
    svr.set_base_dir("./wwwroot");

    svr.listen("0.0.0.0", 8080);
    return 0;
}