
#include <string>

#include "conf_get.h"

using namespace std;

//消除头部空格
string head_dis(string &str_Str)
{
        for(int i = 0; i < str_Str.length();)
        {
                if(str_Str.at(0) == ' ')
                        str_Str = str_Str.substr(1, str_Str.length() - 1);
                else
                        break;
        }
        return str_Str;
}

//消除尾部空格
string tail_dis(string &str_Str)
{
        for(int i = str_Str.length() - 1; i > -1; i--)
        {
                if(str_Str.at(i) == ' ')
                        str_Str = str_Str.substr(0, i );
                else
                        break;
        }
        return str_Str;
}

//分别消除头部尾部空格
void re_blank(config &oriCon)
{
        if(oriCon.con_Name.length() > 0)
        {
                oriCon.con_Name = head_dis(oriCon.con_Name);
                oriCon.con_Name = tail_dis(oriCon.con_Name);
        }
        if(oriCon.con_Value.length() > 0)
        {
                oriCon.con_Value = head_dis(oriCon.con_Value);
                oriCon.con_Value = tail_dis(oriCon.con_Value);
        }
}

//从=处分割字符串
config &str_segment(config &oriCon)
{
        int pos = oriCon.con_All.find('=');
        oriCon.con_Name = oriCon.con_All.substr(0, pos);
        oriCon.con_Value = oriCon.con_All.substr(pos +1, oriCon.con_All.length()-(pos + 1));
        re_blank(oriCon);
        return oriCon;
}

