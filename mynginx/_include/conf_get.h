#ifndef _CONF_GET_H_
#define _CONF_GET_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

typedef struct 
{
        string group;
        string con_All;
        string con_Name;
        string con_Value;
}config;

typedef struct {
        string group_N;
        bool status;
}con_group_t;

class conf_get
{
        public:
        class my_relese
        {
                public:
                ~my_relese()
                {
                        delete conf_get::m_Instance;
                        conf_get::m_Instance = nullptr;
                }
        };

        public:
        vector<con_group_t> con_group;
        vector<config> con_List;
        void group_load(const string &);
        bool Load(string con_FName);
        void detec_load();

        public:
        ~conf_get();
        static conf_get * getInt ()
        {
                if(!m_Instance)
                {
                        if(!m_Instance)
                        {
                                m_Instance = new conf_get();
                                static my_relese r;
                        }
                }
                return m_Instance;
        }

        private:
        static conf_get * m_Instance;
        conf_get();
};


#endif