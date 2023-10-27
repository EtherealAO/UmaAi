#include "GameDatabase.h"
#include "UmaData.h"
#include "../External/utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
using json = nlohmann::json;
using namespace std;

unordered_map<int, UmaData> GameDatabase::AllUmas;
unordered_map<int, unordered_map<int, string> > GameDatabase::TLGTranslation;

void GameDatabase::loadUmas(const string& pathname)
{
    try
    {
        ifstream ifs(pathname);
        stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        json j = json::parse(ss.str(), nullptr, true, true);
        AllUmas.clear();
        for (auto& it : j.items())
        {
            UmaData uma;
            int id = atoi(it.key().c_str());
            it.value().get_to(uma);
            if (TLGTranslation[4].contains(id % 1000000))
                uma.name = TLGTranslation[4][id % 1000000];
            AllUmas[id] = uma;
           //cout << uma.name << endl;
        }
        cout << "������ " << AllUmas.size() << " ��������������" << endl;        
    }
    catch (exception& e)
    {
        cout << "��ȡ������Ϣ����: " << endl << e.what() << endl;
    }
    catch (...)
    {
        cout << "��ȡ������Ϣ����δ֪����" << endl;
    }
}

void GameDatabase::loadTranslation(const string& dir)
{
    try
    {
        ifstream ifs("db/text_data.json");
        stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        json j = json::parse(ss.str(), nullptr, true, true);
        TLGTranslation.clear();
        for (auto& it : j.items())
        {
            unordered_map<int, string> dict;
            int idx = atoi(it.key().c_str());
            for (auto& it1 : it.value().items())
            {
                int index = atoi(it1.key().c_str());
                string st;
                it1.value().get_to(st);
                dict[index] = UTF8_To_string(st);   // json������utf8������ת��������������
            }
            TLGTranslation[idx] = dict;
        }
        cout << "�����������" << endl;
    }
    catch (exception& e)
    {
        cout << "��ȡ������Ϣ����: " << endl << e.what() << endl;
    }
    catch (...)
    {
        cout << "��ȡ������Ϣ����δ֪����" << endl;
    }
}