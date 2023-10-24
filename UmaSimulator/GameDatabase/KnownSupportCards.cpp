#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "GameDatabase.h"
#include "../Game/Game.h"

using json = nlohmann::json;
using namespace std;

unordered_map<int, SupportCard> GameDatabase::AllCards;
unordered_map<int, SupportCard> GameDatabase::DBCards;

void GameDatabase::loadCards(const string& dir)
{
    try
    {
        for (auto entry : filesystem::directory_iterator(dir + "/"))
        {
            //cout << entry.path() << endl;
            if (entry.path().extension() == ".json")
            {
                try
                {
                    ifstream ifs(entry.path());
                    stringstream ss;
                    ss << ifs.rdbuf();
                    ifs.close();
                    json j = json::parse(ss.str(), nullptr, true, true);

                    SupportCard jdata[5];

                    for (int x = 0; x < 5; ++x) {
                        jdata[x].load_from_json(j, x);
                    }

                    cout << "����֧Ԯ�� #" << jdata[4].cardName << " --- " << jdata[4].cardID << endl;
                    if (GameDatabase::AllCards.count(jdata[4].cardID) > 0)
                        cout << "�����ظ�֧Ԯ�� #" << jdata[4].cardName << " --- " << jdata[4].cardID << endl;
                    else {
                        for (int x = 0; x < 5; ++x) 
                            GameDatabase::AllCards[jdata[x].cardID] = jdata[x];
                    }
                        
                }
                catch (exception& e)
                {
                    cout << "֧Ԯ����ϢJSON����: " << entry.path() << endl << e.what() << endl;
                }
            }
        }
        cout << "������ " << GameDatabase::AllCards.size() << " ��֧Ԯ������" << endl;
    }
    catch (exception& e)
    {
        cout << "��ȡ֧Ԯ����Ϣ����: " << endl << e.what() << endl;
    }
    catch (...)
    {
        cout << "��ȡ֧Ԯ����Ϣ����δ֪����" << endl;
    }
}

void GameDatabase::loadDBCards(const string& pathname)
{
    try
    {
        ifstream ifs(pathname);
        stringstream ss;
        ss << ifs.rdbuf();
        ifs.close();
        json j = json::parse(ss.str(), nullptr, true, true);

        SupportCard jdata;
        for (auto & it : j.items()) 
        {
            for (int x = 0; x < 5; ++x) {
                SupportCard jdata;
                jdata.load_from_json(it.value(),x);
                if (GameDatabase::AllCards.count(jdata.cardID) > 0) continue;
                jdata.isDBCard = true;

                GameDatabase::AllCards[jdata.cardID] = jdata;
            }
        }
        cout << "������ " << GameDatabase::AllCards.size() << " ֧Ԯ��Ԫ����" << endl;
    }
    catch (exception& e)
    {
        cout << "��ȡ֧Ԯ����Ϣ����: " << endl << e.what() << endl;
    }
    catch (...)
    {
        cout << "��ȡ֧Ԯ����Ϣ����δ֪����" << endl;
    }
}

CardTrainingEffect SupportCard::getCardEffect(const Game& game, int atTrain, int jiBan, int effectFactor) const
{
    CardTrainingEffect effect(this);
    bool isShining = true;//�Ƿ�����
    if (cardType < 5)//���������ǿ�
    {
        if (jiBan < 80)isShining = false;
        if (cardType != atTrain)isShining = false;
    }
    else if (cardType == 5)//����
    {
    //    if (!game.venusCardIsQingRe)
            isShining = false;
    }
    else if (cardType == 6)//����
      isShining = false;
    else std::cout << "δ֪��";

    //if (game.venusIsWisdomActive && game.venusAvailableWisdom == 3)//��Ů��
    //    isShining = true;

    if (!isShining)
    {
        effect.youQing = 0;
    }
    if (!isShining || atTrain != 4)
        effect.vitalBonus = 0;

    if (isDBCard)
    {
        // �µĹ��д������
        // ���ڲ���ά��isFixed״̬��ÿ�ζ�Ҫ���¼���
        double rate;
        int count, totalJiBan;
        double expectedVital;
        bool apply = false; // Ĭ�ϲ�ִ������applyUniqueEffect
        auto args = uniqueEffectParam;
        unordered_map<int, int> effectValues;
        switch (uniqueEffectType)
        {
            case 101:   // �>=args[1]ʱ��Ч
                if (jiBan >= args[1])
                    apply = true;
                break;
            case 102:   // �������+���ó�ѵ��
                if (jiBan >= args[1] && cardType != atTrain)
                    apply = true;
                break;
            case 103:   // DD�����濨�飬����̶���Ч
                apply = true;
                break;
            case 104:   // ����·��û�з�˿�����ûغ�������
                effectValues[(int)UniqueEffectType::TrainingEffectUp] = clamp((double)game.turn*2*args[2] / TOTAL_TURN, 0.0, (double)args[2]);
                apply = true;
                break;
            case 105:   // ���ݱ��֧Ԯ�����͵ĳ�ʼ���Լӳ�(��Ӧ����+10������/�Ŷӿ�ȫ����+2), �ݲ�����
                break;
            case 106:   // �����ǣ���Ҫ�õ�effectFactor
                effectValues[(int)UniqueEffectType::SpecialTagEffectUp] = min(args[1], effectFactor) * args[3];
                apply = true;
            break;
            case 107:   // ���񣬵�
                // ��Ҫ����ѵ�������������ʱ��-20����
                expectedVital = game.vital - 20;
                rate = clamp(expectedVital / game.maxVital, 0.3, 1.0);
                // (0.3, 1) --> (1, 0)
                effectValues[(int)UniqueEffectType::SpecialTagEffectUp] = args[5] + args[2] * (1 - rate) / 0.7;
                apply = true;
                break;
            case 108:   // ����
                effectValues[(int)UniqueEffectType::TrainingEffectUp] = 5 + 3 * clamp((game.maxVital - 100) / 4, 0, 5);
                apply = true;
                break;
            case 109:   // �ر�����Ҫ�������
                totalJiBan = 0;
                for (int i = 0; i < 6; ++i)
                    totalJiBan += game.persons[i].friendship;
                rate = totalJiBan / 600;
                effectValues[(int)UniqueEffectType::TrainingEffectUp] = rate * 20;
                apply = true;
                break;
            case 110:   // ����ӥ����Ҫ����ͬʱѵ���Ŀ�����
                /*
                count = 0;
                for (int i = 0; i < 6; ++i)
                    if (game.cardDistribution[i, atTrain])
                        ++count;
                effect["TrainingEffectUp"] = args[2] * Math.Min(5, count);
                apply = true;*/
                break;
            case 111:   // ˮ˾������Ҫ��ǰѵ���ȼ�
                /*
                effect["TrainingEffectUp"] = args[2] * Math.Min(5, game.trainLevelCount[atTrain] / 12 + 1);
                apply = true;
                */
                break;
            case 112:   // ����ɽ
                break;
            case 113:   // B95���齴
                // todo: ��Ҫ�ж��Ƿ�Ϊ��Ȧѵ��
                apply = true; break;
            case 114:   // ������, ��ʱ��ѵ��ǰ������
                rate = clamp((double)game.vital / 100, 0.0, 1.0);    // >100Ҳ����ѵ��
                effectValues[(int)UniqueEffectType::TrainingEffectUp] = 5 + 15 * rate;
                apply = true;
                break;
            case 115:   // ��ñ����ʱ����
                break;
            case 116:   // �߷壬�����ţ��ȣ�ͳһ����ʼ1�㣬��װ������ڶ�������ѧ������
                count = (game.turn >= 30 ? args[4] :
                    (game.turn >= 24 ? (args[4] + 1) / 2 : 1));
                effectValues[args[2]] = args[3] * count;
                apply = true;
                break;
            case 117:   // ����ͣ���Ҫѵ���ȼ�
                /*
                count = 0;
                for (int i = 0; i < 5; ++i)
                    count += Math.Min(5, game.trainLevelCount[i] / 12 + 1);
                rate = Math.Clamp((double)count / 25, 0, 1);
                effect["TrainingEffectUp"] = rate * args[3];
                apply = true;*/
                break;
            case 118:   // ����
                break;
            default:   // type == 0
                if (uniqueEffectType != 0) {
                    cout << "δ֪���� #" << uniqueEffectType << endl;
                }
                apply = true;
                break;
        }   // switch

        if (apply)
        {
            // uniqueEffectValues�Ĺ̶����ֵ����effectValues�ﶯ̬��ӵ����ֵ��Ҫ��Ч
            for (auto it : uniqueEffectValues)
                effect.applyUniqueEffectLine(it.first, it.second);
            for (auto it : effectValues)
                effect.applyUniqueEffectLine(it.first, it.second);
        }
    }
    else
    {
        // �ϰ汾���д���
        int cardSpecialEffectId = cardID / 10;

        //�������Ǹ��ֹ���
        //1.����
        if (cardSpecialEffectId == 30137)
        {
            if (jiBan < 100)
            {
                if (isShining)
                    effect.youQing = 20;
                effect.ganJing = 0;
                effect.bonus[5] = 0;
            }
        }
        //2.�߷�
        //Ϊ�˼򻯣���Ϊ��ʼѵ���ӳ���4%����һ��������20%��Ҳ���ǵ�n���غ�4+n*(2/3)%
        else if (cardSpecialEffectId == 30134)
        {
            if (game.turn < 24)
                effect.xunLian = 4 + 0.6666667 * game.turn;
        }
        //3.����
        else if (cardSpecialEffectId == 30010)
        {
            //ɶ��û��
        }
        //4.��������
        else if (cardSpecialEffectId == 30019)
        {
            //ɶ��û��
        }
        //5.������
        else if (cardSpecialEffectId == 30011)
        {
            //ɶ��û��
        }
        //6.ˮ˾��
        else if (cardSpecialEffectId == 30107)
        {

        int traininglevel = game.getTrainingLevel(atTrain);
        effect.xunLian = 5 + traininglevel * 5;
        if (effect.xunLian > 25)effect.xunLian = 25;
    }
    //7.����˹
    else if (cardSpecialEffectId == 30130)
    {
        if (jiBan < 80)
        {
            effect.bonus[2] = 0;
        }
    }
    //8.���ʵ�
    else if (cardSpecialEffectId == 30037)
    {
        if (jiBan < 80)
        {
            effect.bonus[0] = 0;
        }
    }
    //9.������
    else if (cardSpecialEffectId == 30027)
    {
        //ɶ��û��
    }
    //10.�ٱ�Ѩ
    else if (cardSpecialEffectId == 30147)
    {
        if (jiBan < 100)
        {
            effect.bonus[0] = 0;
        }
    }
    //11.�ͺ���
    else if (cardSpecialEffectId == 30016)
    {
        //ɶ��û��
    }
    //12.�Ǻø��
    else if (cardSpecialEffectId == 30152)
    {
        if (jiBan < 80)
        {
            effect.bonus[0] = 0;
        }
    }
    //13.���ƽ��
    else if (cardSpecialEffectId == 30153)
    {
        if (jiBan < 100)
        {
            effect.bonus[3] = 0;
        }
    }
    //14.�ǲ���
    else if (cardSpecialEffectId == 30141)
    {
        //ɶ��û��
    }
    //15.�͵Ҷ�˹
    else if (cardSpecialEffectId == 30099)
    {
        int totalJiBan = 0;
        for (int i = 0; i < game.normalCardCount; i++)
            totalJiBan += game.persons[i].friendship;
        if(game.larc_zuoyueType!=0)
          totalJiBan += game.persons[17].friendship;
        effect.xunLian = totalJiBan / 30;
    }
    //����
    else if (cardSpecialEffectId == 30101) {
        if (jiBan < 100)
        {
          if (effect.youQing > 0)
            effect.youQing = 20;
        }
    }
    //22���͹��
    else if (cardSpecialEffectId == 30142)
    {
      if (game.turn < 24)
        effect.bonus[1] = 1;
      else if (game.turn < 48)
        effect.bonus[1] = 2;
      else
        effect.bonus[1] = 3;
    }
    //23������
    else if (cardSpecialEffectId == 30123)
    {
      int traininglevel = game.getTrainingLevel(atTrain);
      effect.xunLian = 5 + traininglevel * 5;
      if (effect.xunLian > 25)effect.xunLian = 25;
    }
    //24������
    else if (cardSpecialEffectId == 30151)
    {
        if (jiBan < 100)
        {
            effect.xunLian = 0;
        }
    }
    //25����ǡ
    else if (cardSpecialEffectId == 30138)
    {
      if (jiBan < 100)
      {
        effect.bonus[2] = 0;
      }
    }
    //28������
    else if (cardSpecialEffectId == 30112)
    {
      //�Ժ�����취
    }
    //29������
    else if (cardSpecialEffectId == 30083)
    {
      if (jiBan < 80 || atTrain == 3)
        effect.xunLian = 0;
    }
    //30������
    else if (cardSpecialEffectId == 30094)
    {
      if (effect.youQing > 0)
      {
        float extraBonus = 5 + (100 - game.vital) / 7.0;
        if (extraBonus > 15)extraBonus = 15;
        if (extraBonus < 5)extraBonus = 5; // std::cout << effect.youQing << " ";

                effect.youQing = 120 * (1 + 0.01 * extraBonus) - 100;

            }

        }
        //Ҳ��
        else if (cardSpecialEffectId == 30126)
        {
            if (jiBan < 80)
            {

                effect.bonus[5] = 0;
            }
        }
        //����
        else if (cardSpecialEffectId == 30127)
        {
            if (isShining)
            {
                effect.ganJing = 60;
            }
        }
        //����
        else if (cardSpecialEffectId == 47)
        {
            //null
        }
        //�ٶ���
        else if (cardSpecialEffectId == 30119)
        {
            if (jiBan < 80)
            {
                effect.bonus[2] = 0;
            }
        }
        // ����
        else if (cardSpecialEffectId == 30067) {

            if (jiBan < 80)
                effect.bonus[5] = 0;

    }
    // �챦
    else if (cardSpecialEffectId == 30114) {
        if (jiBan < 80)
            effect.bonus[2] = 0;
    }
    // ����
    else if (cardSpecialEffectId == 30078) {
        effect.failRateDrop += 10;
    }
    // ��ñ
    else if (cardSpecialEffectId == 30021) {
        effect.failRateDrop += 7;
        effect.vitalCostDrop += 4;
    }
    // ������
    else if (cardSpecialEffectId == 30156) {
      if (jiBan < 80)
      {
        effect.bonus[2] = 0;
      }
    }
    // ���ɾ�
    else if (cardSpecialEffectId == 30132) {
      int guyouLevel = (game.maxVital - 100) / 4;
      if (guyouLevel > 5)guyouLevel = 5;
      effect.xunLian = 5 + 3 * guyouLevel;
    }
    //����ӥ
    else if (cardSpecialEffectId == 30161)
    {
      if (jiBan < 100)
      {
        for (int i = 0; i < 5; i++)
          effect.bonus[i] -= 1;
      }
    }
    //�ٻƽ�
    else if (cardSpecialEffectId == 30168)
    {
      if (jiBan < 80)
      {
        effect.bonus[0] -= 1;
      }
    }
    else if (cardSpecialEffectId == 30154)
    {
      if (game.turn < 24)
        effect.bonus[0] = 1;
      else if (game.turn < 48)
        effect.bonus[0] = 2;
      else
        effect.bonus[0] = 3;
    }
    else if (cardSpecialEffectId == 30165)
    {
      if (jiBan >= 80)
      {
        effect.bonus[3] += 2;
      }
    }
    else if (cardSpecialEffectId == 30139)
    {
      if (jiBan >= 100)
      {
        effect.bonus[1] += 3;
      }
    }
    else if (cardSpecialEffectId == 30164)
    {
      if (jiBan >= 80)
      {
        effect.bonus[5] += 2;
      }
    }
    else if (cardSpecialEffectId == 30166)
    {
      if (game.turn < 12)
        effect.xunLian += 5.0 + (10.0 / 12) * game.turn;
      else
        effect.xunLian += 15;
    }
    else if (cardSpecialEffectId == 30158)
    {
      if (jiBan >= 100)
      {
        effect.youQing += (100 + effect.youQing) * 0.2;
      }
    }
    else if (cardSpecialEffectId == 30148)
    {
      int t = 0;
      for (int i = 0; i < 5; i++)
        t += game.getTrainingLevel(atTrain);
      int y = 5 + t;
      if (y > 20)y = 20;
      effect.xunLian += y;
    }
    else if (cardSpecialEffectId == 30163)
    {
      if (jiBan >= 80)
      {
        effect.xunLian += 10;
      }
    }
    // �ǲ���
    else if (cardSpecialEffectId == 30157) {
      if (jiBan >= 100)
      {
        effect.bonus[4] = 2;
        effect.bonus[5] = 1;
      }
    }
    //[��]�湭�쳵(id:30149)�Ĺ��������ʵ�ѵ��60�ɾ��ӳɣ������ڰ������ͷ���һ��֮ǰ����֪����û���ʣ���˼���������ͷ֮����Ҫ��������ſ��Ĳ������д���
    //��������д��Game�����ˣ���Ȼ�ܳ�ª����û�뵽ʲô�ð취
    else if (cardSpecialEffectId == 30149)
    {
      //��������ѵ��ʱ���ɾ��ӳ�60
      //���г����ȸ߷��150���
      //ʵ�ʱȹ��г�����100���
      effect.ganJing += 60;
    }
    else
    {
      //  std::cout << "δ֪��";
    }
    if (!isShining)
      effect.youQing = 0;
    return effect;
}
