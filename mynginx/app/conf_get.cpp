#include "conf_get.h"

extern config &str_segment(config &oriCon);

conf_get *conf_get::m_Instance = NULL;
vector<string>con_N_str = {"all", "log", "socket"};

conf_get::~conf_get()
{ 
       /* for(vector<config>::iterator iter = con_List.begin(); iter != con_List.end(); )
        {
                con_List.erase(iter);
        }*/
}

conf_get::conf_get()
{ 
       con_group_t temp;
       for(int i = 0; i < con_N_str.size(); ++i)
       {
               temp.group_N = con_N_str.at(i);
               temp.status = false;
               con_group.push_back(temp);
       }
}

//检测到目标配置组
void conf_get::group_load(const string &group)
{
        vector<con_group_t>::iterator it = con_group.begin();
        while(it != con_group.end())
        {
                if(group == it->group_N)
                {
                        it->status = true;
                        break;
                }
                ++it;
        }
}

//配置文件检测
void conf_get::detec_load()
{
        vector<con_group_t>::iterator it = con_group.begin();
        while(it != con_group.end())
        {
                if(it->status == false)
                {
                        cout << "未检测有关" + it->group_N + "的配置文件" << endl;
                }
                ++it;
        }
}

bool conf_get::Load(string con_FName)
{
        ifstream fp(con_FName);
        config *con = new config;
        string str, str2, group;
        string str1(500, ' ');
        if(fp)
        {
                //cout << "加载配置文件：" + con_FName << endl;
                while(getline(fp, str))
                {
                        if(str.length() > 0)
                        {
                                str2 = str1.substr(0, str.length());
                                if(str.at(0) == '[' && str.at(str.length() - 1) == ']')
                                {
                                        group = str.substr(1,str.length() - 2);
                                        group_load(group);
                                }
                                else if(str.at(0) == ' '  && str2 == str)
                                {
                                         continue;                  
                                }
                                else if(str.at(0) != '\n' && str.at(0) != '#' )
                                {
                                        con->group = group;
                                        con->con_All = str;
                                        str_segment(*con); 
                                        con_List.push_back(*con);
                                }
                        }
                }
                detec_load();
        }
        else
        {
                cout << "配置文件读取失败，请检查配置文件是否正确 ..." << endl;
                exit(EXIT_FAILURE);
        }
        delete con;
        fp.close();
        return true;
}