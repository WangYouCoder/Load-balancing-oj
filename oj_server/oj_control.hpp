#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include <jsoncpp/json/json.h>
#include <cassert>
#include <mutex>
#include <fstream>
namespace WY_control
{
    class Machine
    {
    public:
        std::string ip;
        std::uint16_t port;
        int load;
        std::mutex *mtx;
    public:
        Machine() : ip(""), port(0), load(0), mtx(nullptr)
        {}
        ~Machine()
        {}
    public:
        void IncLoad()
        {
            if(mtx) mtx->lock();
            load++;
            if(mtx) mtx->unlock();
        }

        void DecLoad()
        {
            if(mtx) mtx->lock();
            load--;
            if(mtx) mtx->unlock();
        }

        int Load()
        {
            if(mtx) mtx->lock();
             int l = load;
            if(mtx) mtx->unlock();
            return l;
        }

        void ResetLoad()
        {
            if(mtx) mtx->lock();
            load = 0;
            if(mtx) mtx->unlock();
        }
    };

    using namespace WY_util;
    const std::string service_machine = "./conf/service_machine.conf";
    class LoadBlance
    {
    private:
        std::vector<Machine> machines;
        std::vector<int> online;
        std::vector<int> offline;
        std::mutex mtx;
    public:
        LoadBlance()
        {
            assert(LoadConf(service_machine));
            LOG(Info) << " 打开" << service_machine << " 成功\n";

        }
        ~LoadBlance(){}

        bool LoadConf(const std::string &machine_list)
        {
            std::ifstream in(machine_list);
            if(!in.is_open())
            {
                LOG(Fatal) << " 打开" << machine_list << " 失败\n";
                return false;
            }

            std::string line;
            while(getline(in, line))
            {
                std::vector<std::string> v;
                SplitUtil::Split(line, ":", &v);
                if(v.size() != 2)
                {
                    LOG(Fatal) << " 切分" << line << " 失败\n";
                    continue;
                }
                Machine m;
                m.ip = v[0];
                m.port = std::stoi(v[1].c_str());
                m.load = 0;
                m.mtx = new std::mutex();

                online.push_back(machines.size());
                machines.push_back(m);
            }

            in.close();
            return true;
        }

        bool SmartChoice(int *id, Machine **m)
        {
            mtx.lock();
            int online_num = online.size();
            if(online_num == 0)
            {
                mtx.unlock();
                LOG(Fatal) << " 所有的后端主机都已离线, error error error ! ! !\n";
                return false;
            }

            *id = online[0];
            *m = &machines[online[0]];
            int min_load = machines[online[0]].load;
            for(int i = 0; i < online.size(); i++)
            {
                if(min_load > machines[online[i]].load)
                {
                    min_load = machines[online[i]].load;
                    *id = online[i];
                    *m = &machines[online[i]];
                }
            }
            mtx.unlock();
            return true;
        }

        void OfflineMachine(int which)
        {
            mtx.lock();
            for(auto iter = online.begin(); iter != online.end(); iter++)
            {
                if(*iter == which)
                {
                    machines[which].ResetLoad();
                    online.erase(iter);
                    offline.push_back(which);
                    break;
                }
            }

            mtx.unlock();
        }

        void OnlineMachine()
        {
            mtx.lock();
            online.insert(online.end(), offline.begin(), offline.end());
            offline.erase(offline.begin(), offline.end());
            mtx.unlock();

            LOG(Info) << "所有的主机已经上线了\n";
        }

        void ShowMachines()
        {
            mtx.lock();
            std::cout << "当前在线的主机列表: ";
            for(auto &id : online)
            {
                std::cout << id << " ";
            }

            std::cout << "当前离线的主机列表: ";
            for(auto &id : offline)
            {
                std::cout << id << " ";
            }

            mtx.unlock();
        }
    };

    using namespace WY_model;
    using namespace WY_view;
    class Control
    {
    private:
        Model _model;
        View _view;
        LoadBlance _load_blance;
    public:
        void RecoveryMachine()
        {
            _load_blance.OnlineMachine();
        }

        bool AllQuestion(std::string *html)
        {
            bool ret = true;
            std::vector<Question> v;
            if(_model.GetAllQuestion(&v))
            {
                sort(v.begin(), v.end(), [](const Question &q1, const Question &q2){
                    return atoi(q1.number.c_str()) < atoi(q2.number.c_str());
                });
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

        void Judge(const std::string &number, const std::string &in_json, std::string *out_json)
        {
            // 1. 获取题目信息
            Question q;
            _model.GetOnlyOneQuestion(number, &q);
            // 2. 将从客户端得到的数据进行反序列化转为json串
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json, in_value);
            std::string code = in_value["code"].asString();
            // 3. 将从客户端得到的代码与测试用例进行组合
            Json::Value compile_value;
            compile_value["input"] = in_value["input"].asString();
            compile_value["code"] = code + q.tail;
            compile_value["cpu_limit"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;
            // 4. 进行序列化，转为字符串
            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_value);
            // 5. 挑选服务器
            while(true)
            {
                int id = 0;
                Machine *m = nullptr;
                if(!_load_blance.SmartChoice(&id, &m))
                {
                    break;
                }
                
                // 6. 发起http请求
                httplib::Client cli(m->ip, m->port);
                m->IncLoad();

                LOG(Info) << " 选择主机成功， 主机id: " << id <<"  主机ip: "<< m->ip <<  "  主机port: " << m->port << "  当前主机的负载是: " << m->Load() <<  "\n";

                if(auto res = cli.Post("/compile_run", compile_string, "application/json;charset=utf-8"))
                {
                    if(res->status == 200)
                    {
                        *out_json = res->body;
                        m->DecLoad();
                        LOG(Info) << "编译运行成功\n";
                        break;
                    } 
                }
                else
                {
                    LOG(Error) << " 请求资源失败, 主机id: " << id <<" 主机ip: "<< m->ip <<  " 主机port: " << m->port << "当前主机可能已经离线" << "\n";
                    _load_blance.OfflineMachine(id);
                    _load_blance.ShowMachines();
                }
                m->DecLoad();
            }
        }
    };
};