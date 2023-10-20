#pragma once
#include "../GameDatabase/GameConstants.h"
#include "../SupportCard/CardEffect.h"
#include "../External/json.hpp"
#include "../External/utils.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
using json = nlohmann::json;

class Game;

struct SkillList {
	int skillNum;
	std::vector<int> skillIdList; // ��Ƭӵ�еļ��ܱ��
};
struct SupportCard
{
	int cardID;//֧Ԯ��id�����ֿ����бȽϸ��ӵĹ��У�����id�����
	int cardType;//֧Ԯ�����ͣ�0��1��2��3��4��5�Ŷ�6����
	std::string cardName; //��Ƭ����

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
	// ������ã����ݿ�Ƭ�ĵȼ�����Ϸ��ʼ�׶θ�ֵ

	int uniqueEffectType; //֧Ԯ�����д�����������Ŵ�����
	int uniqueEffectParam[10]; //֧Ԯ������Ч��������������pt �� ѵ �ɾ� ����������Ŵ�����


	bool larc_isLink;//�Ƿ�Ϊlink��
	int larc_linkSpecialEffect;//linkЧ��

	//SkillList cardSkill;
	//��Ƭӵ�еļ����б�

	//std::string uniqueText;
	CardTrainingEffect getCardEffect(const Game& game, int atTrain, int jiBan, int effectFactor) const;//������Ϸ״̬����֧Ԯ���ġ����С�

	void getNNInputV1(float* buf) const;//���������룬size=NNINPUT_CHANNELS_CARD_V1

	void write_to_json(json& j, const std::string cdname, const int id) const;
	void load_from_json(json& j, int x);
};