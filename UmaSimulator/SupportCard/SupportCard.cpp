#include <iostream>
#include "SupportCard.h"
#include "../NeuralNet/NNInput.h"
#include "../GameDatabase/GameConstants.h"
#include "../Game/Game.h"

using namespace std;

void SupportCard::write_to_json(json& j, const std::string cdname, const int id) const
{
	j["cardId"] = id / 10;
	j["cardType"] = cardType;

	j["cardName"] = string_To_UTF8(cdname);

	j["cardValue"][1]["filled"] = false;
	j["cardValue"][2]["filled"] = false;
	j["cardValue"][3]["filled"] = false;

	int x = id % 10;
	j["cardValue"][x]["filled"] = filled;
	if (filled == true) {
		j["cardValue"][x]["filled"] = true;
		j["cardValue"][x]["youQing"] = youQingBasic;
		j["cardValue"][x]["ganJing"] = ganJingBasic;
		j["cardValue"][x]["xunLian"] = xunLianBasic;
		j["cardValue"][x]["bonus"] = bonusBasic;
		j["cardValue"][x]["wizVitalBonus"] = wizVitalBonusBasic;
		j["cardValue"][x]["initialBonus"] = initialBonus;
		j["cardValue"][x]["initialJiBan"] = initialJiBan;
		j["cardValue"][x]["saiHou"] = saiHou;
		j["cardValue"][x]["hintBonus"] = hintBonus;
		j["cardValue"][x]["hintProbIncrease"] = hintProbIncrease;
		j["cardValue"][x]["deYiLv"] = deYiLv;
		j["cardValue"][x]["failRateDrop"] = failRateDrop;
		j["cardValue"][x]["vitalCostDrop"] = vitalCostDrop;
	}

	//j["cardSkill"]["skillNum"] = 0;
	//j["cardSkill"]["SkillList"] = NULL;

}

void SupportCard::load_from_json(json& j, int x) {

	cardID = j.value("cardID", 0);
	cardID = cardID * 10 + x;
	cardType = j.value("cardType", -1);
	cardName = UTF8_To_string(j.value<std::string>("cardName", ""));
	//j.at("cardSkill").get_to(cardSkill);	// �����뼼�ܣ���Ϊ��ʽ��ͬ
	charaId = j.value("charaId", -1);
	// ������С�û�и�keyʱ��Ĭ��ֵ
	uniqueEffectType = j.value("uniqueEffectType", 0);
	uniqueEffectParam = j.value("uniqueEffectParam", vector<int>());

	filled = j["cardValue"][x].value("filled", false);
	if (filled) {
        youQingBasic = j["cardValue"][x].value<double>("youQing", 0);
		ganJingBasic = j["cardValue"][x].value<double>("ganJing", 0);
		xunLianBasic = j["cardValue"][x].value<double>("xunLian", 0);
		wizVitalBonusBasic = j["cardValue"][x].value("wizVitalBonus", 0);
		initialJiBan = j["cardValue"][x].value("initialJiBan", 0);
		saiHou = j["cardValue"][x].value<double>("saiHou", 0);
		hintProbIncrease = j["cardValue"][x].value<double>("hintProbIncrease", 0);
		deYiLv = j["cardValue"][x].value<double>("deYiLv", 0);
		failRateDrop = j["cardValue"][x].value<double>("failRateDrop", 0);
		vitalCostDrop = j["cardValue"][x].value<double>("vitalCostDrop", 0);

		j["cardValue"][x].at("bonus").get_to(bonusBasic);
		j["cardValue"][x].at("initialBonus").get_to(initialBonus);
		j["cardValue"][x].at("hintBonus").get_to(hintBonus);		
	}

	if (j.contains("larc_isLink"))
		larc_isLink = j["larc_isLink"];
	else
		larc_isLink = false;

	if (j.contains("larc_linkSpecialEffect"))
		larc_linkSpecialEffect = j["larc_linkSpecialEffect"];
	else
		larc_linkSpecialEffect = 0;
	return;
}

void SupportCard::getNNInputV1(float* buf) const
{
	//ÿ�ſ��ĳ�ʼ���Լӳɡ���ʼ�����ӳɲ���Ҫ���������磬ֻ����������
	for (int ch = 0; ch < NNINPUT_CHANNELS_CARD_V1; ch++)
		buf[ch] = 0;

	//0~6 cardtype
	buf[cardType] = 1.0;

	//����ӳ�䵽0~1��Χ
	buf[7] = youQingBasic * 0.04;
	buf[8] = ganJingBasic * 0.02;
	buf[9] = xunLianBasic * 0.05;
	for (int i = 0; i < 6; i++)
		buf[10 + i] = bonusBasic[i] * 0.5;
	buf[16] = wizVitalBonusBasic * 0.2;
	for (int i = 0; i < 6; i++)
		buf[17 + i] = hintBonus[i] * 0.05;
	buf[23] = hintProbIncrease * 0.02;
	buf[24] = deYiLv * 0.02;
	buf[25] = failRateDrop * 0.04;
	buf[26] = vitalCostDrop * 0.05;

	buf[27] = larc_isLink ? 1.0 : 0.0;
	if (larc_linkSpecialEffect != 0)//��Χ3~12
		buf[28 + (larc_linkSpecialEffect - 3)] = 1.0;

	assert(false && "todo���д���");
	buf[38] = 0.0;
}