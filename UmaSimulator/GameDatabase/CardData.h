#pragma once
#include "GameConstants.h"
#include "../SupportCard/CardEffect.h"
#include "../External/json.hpp"
#include "../External/utils.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
using json = nlohmann::json;

class Game;

struct CardValue {
	//ȥ������basic�ֶ�
	bool filled;
	double youQingBasic;//����ӳ�
	double ganJingBasic;//�ɾ��ӳ�
	double xunLianBasic;//ѵ���ӳ�
	double bonusBasic[6];//����������pt�ļӳ�
	int wizVitalBonusBasic;//������Ȧ�����ظ���
	int initialBonus[6];//��������������pt������
	int initialJiBan;//��ʼ�
	double saiHou;//����
	int hintBonus[6];//Ϊ�˼򻯣��Ѻ��ļ��ܵ�Ч�ɶ������ԡ��ۺϿ��Ǽ�����Ч�ʣ�����߷�90%��Ч�����˼���������ƽ���Լ۱����ۿۣ������ظ������ܣ�����30%��
	double hintProbIncrease;//��������������
	double deYiLv;//������
	double failRateDrop; //ʧ���ʽ���
	double vitalCostDrop; //���������½�
};
struct SkillList {
	int skillNum;
	std::vector<int> skillIdList; // ��Ƭӵ�еļ��ܱ��
};
struct SupportCard
{
	int cardID;//֧Ԯ��id�����ֿ����бȽϸ��ӵĹ��У�����id�����
	int cardType;//֧Ԯ�����ͣ�0��1��2��3��4��5���˻��Ŷ�
	std::string cardName; //��Ƭ����

	CardValue level[5];
	// ����ͻ�Ƶȼ�������

	double youQingBasic;//����ӳ�
	double ganJingBasic;//�ɾ��ӳ�
	double xunLianBasic;//ѵ���ӳ�
	double bonusBasic[6];//����������pt�ļӳ�
	int wizVitalBonusBasic;//������Ȧ�����ظ���
	int initialBonus[6];//��������������pt������
	int initialJiBan;//��ʼ�
	double saiHou;//����
	int hintBonus[6];//Ϊ�˼򻯣��Ѻ��ļ��ܵ�Ч�ɶ������ԡ��ۺϿ��Ǽ�����Ч�ʣ�����߷�90%��Ч�����˼���������ƽ���Լ۱����ۿۣ������ظ������ܣ�����30%��
	double hintProbIncrease;//��������������
	double deYiLv;//������
	double failRateDrop; //ʧ���ʽ���
	double vitalCostDrop; //���������½�
	// ������ã����ݿ�Ƭ�ĵȼ�����Ϸ��ʼ�׶θ�ֵ

	SkillList cardSkill;
	//��Ƭӵ�еļ����б�
	
	int effectFactor; // ��Ϊ������д���Ĳ���
	//std::string uniqueText;
	CardTrainingEffect getCardEffect(const Game& game, int atTrain, int jiBan, int effecFactor) const;//������Ϸ״̬����֧Ԯ���ġ����С�

	void cardValueInit(int x) {
		while (x <= 4 && level[x].filled == false)
			x++;
		if (x > 4) {
			std::cout << "δ֪��Ƭ��Ϣ����Ƭ����¼�����: " << cardName << '\n';
			exit(0);
		}

		youQingBasic = level[x].youQingBasic;
		ganJingBasic = level[x].ganJingBasic;
		xunLianBasic = level[x].xunLianBasic;
		for (int i = 0; i < 6; ++i)
			bonusBasic[i] = level[x].bonusBasic[i];
		wizVitalBonusBasic = level[x].wizVitalBonusBasic;
		for (int i = 0; i < 6; ++i)
			initialBonus[i] = level[x].initialBonus[i];
		initialJiBan = level[x].initialJiBan;
		saiHou = level[x].saiHou;
		for (int i = 0; i < 6; ++i)
			hintBonus[i] = level[x].hintBonus[i];
		hintProbIncrease = level[x].hintProbIncrease;
		deYiLv = level[x].deYiLv;
		failRateDrop = level[x].failRateDrop;
		vitalCostDrop = level[x].vitalCostDrop;

		effectFactor = 0;

		return ;

	}

	void write_to_json(json& j,const std::string cdname,const int id)
	{
		j["cardId"] = id;
		j["cardType"] = cardType;

		j["cardName"] = string_To_UTF8(cdname);

		j["cardValue"][0]["filled"] = false;
		j["cardValue"][1]["filled"] = false;
		j["cardValue"][2]["filled"] = false;
		j["cardValue"][3]["filled"] = false;

		for (int x = 0; x < 5; ++x) {
			if (j["cardValue"][x]["filled"] == false) {
				j["cardValue"][x]["filled"] = false;
				continue;
			}
			j["cardValue"][x]["filled"] = true;
			j["cardValue"][x]["youQing"] = level[x].youQingBasic;
			j["cardValue"][x]["ganJing"] = level[x].ganJingBasic;
			j["cardValue"][x]["xunLian"] = level[x].xunLianBasic;
			j["cardValue"][x]["bonus"] = level[x].bonusBasic;
			j["cardValue"][x]["wizVitalBonus"] = level[x].wizVitalBonusBasic;
			j["cardValue"][x]["initialBonus"] = level[x].initialBonus;
			j["cardValue"][x]["initialJiBan"] = level[x].initialJiBan;
			j["cardValue"][x]["saiHou"] = level[x].saiHou;
			j["cardValue"][x]["hintBonus"] = level[x].hintBonus;
			j["cardValue"][x]["hintProbIncrease"] = level[x].hintProbIncrease;
			j["cardValue"][x]["deYiLv"] = level[x].deYiLv;
			j["cardValue"][x]["failRateDrop"] = level[x].failRateDrop;
			j["cardValue"][x]["vitalCostDrop"] = level[x].vitalCostDrop;
		}

		j["cardSkill"]["skillNum"] = 0;
		j["cardSkill"]["SkillList"] = NULL;

	}

	void load_from_json(json& j) {

		j.at("cardId").get_to(cardID);
		j.at("cardType").get_to(cardType);
		std::string st;
		j.at("cardName").get_to(st);
		cardName = UTF8_To_string(st);

		for (int x = 0; x < 5; ++x) {

			j["cardValue"][x].at("filled").get_to(level[x].filled);
			if (level[x].filled == false) continue;

			level[x].youQingBasic = j["cardValue"][x].value<double>("youQing", 0);
			level[x].ganJingBasic = j["cardValue"][x].value<double>("ganJing", 0);
			level[x].xunLianBasic = j["cardValue"][x].value<double>("xunLian", 0);
			j["cardValue"][x].at("bonus").get_to(level[x].bonusBasic);
			level[x].wizVitalBonusBasic = j["cardValue"][x].value("wizVitalBonus", 0);
			j["cardValue"][x].at("initialBonus").get_to(level[x].initialBonus);
			level[x].initialJiBan = j["cardValue"][x].value("initialJiBan", 0);
			level[x].saiHou = j["cardValue"][x].value<double>("saiHou", 0);
			j["cardValue"][x].at("hintBonus").get_to(level[x].hintBonus);
			level[x].hintProbIncrease = j["cardValue"][x].value<double>("hintProbIncrease", 0);
			level[x].deYiLv = j["cardValue"][x].value<double>("deYiLv", 0);
			level[x].failRateDrop = j["cardValue"][x].value<double>("failRateDrop", 0);
			level[x].vitalCostDrop = j["cardValue"][x].value<double>("vitalCostDrop", 0);
		}

		cardSkill.skillNum = j["cardSkill"]["skillNum"];

		cardSkill.skillIdList.resize(cardSkill.skillNum);
		for (int i = 0; i < cardSkill.skillNum; ++i) {
			cardSkill.skillIdList[i] = j["cardSkill"]["skillList"][i];
		}
		/*
		if (j["uniqueEffect"].is_array()) {
			j["uniqueEffect"].at(0).get_to(uniqueText);
			uniqueText = UTF8_To_string(uniqueText);
		}
		*/
		return;
	}
};