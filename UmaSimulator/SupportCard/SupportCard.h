#pragma once
#include "../GameDatabase/GameConstants.h"
#include "../External/json.hpp"
#include "../External/utils.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
using json = nlohmann::json;

class Game;
class CardTrainingEffect;

struct SupportCard
{
	bool isDBCard;	// ����������ϰ汾��д�Ļ����°汾�Զ���
	int cardID;//֧Ԯ��id
	int charaId;	// ��ɫID
	int cardType;//֧Ԯ�����ͣ�0��1��2��3��4��5�Ŷ�6����
	std::string cardName; //��Ƭ����
	//std::vector<int> cardSkill;	// �����б�

	bool filled;
	double youQingBasic;//����ӳ�
	double ganJingBasic;//�ɾ��ӳ�
	double xunLianBasic;//ѵ���ӳ�
	double bonusBasic[6];//����������pt�ļӳ�
	int wizVitalBonusBasic;//������Ȧ�����ظ���
	int initialBonus[6];//��������������pt������
	int initialJiBan;//��ʼ�***ʵ����Ч����cardEffect�е�
	double saiHou;//����***ʵ����Ч����cardEffect�е�
	int hintBonus[6];//Ϊ�˼򻯣��Ѻ��ļ��ܵ�Ч�ɶ������ԡ��ۺϿ��Ǽ�����Ч�ʣ�����߷�90%��Ч�����˼���������ƽ���Լ۱����ۿۣ������ظ������ܣ�����30%��
	double hintProbIncrease;//��������������
	double deYiLv;//�����ʣ�***ʵ����Ч����cardEffect�е�
	double failRateDrop; //ʧ���ʽ���
	double vitalCostDrop; //���������½�
	// ������ã����ݿ�Ƭ�ĵȼ�����Ϸ��ʼ�׶θ�ֵ

	int uniqueEffectType; //֧Ԯ����������
	std::vector<int> uniqueEffectParam; //֧Ԯ�������������
	std::unordered_map<int, int> uniqueEffectValues;	// ֧Ԯ��������壨���ڲ���������ֵ��

	bool larc_isLink;//�Ƿ�Ϊlink��
	int larc_linkSpecialEffect;//linkЧ��

	// ������Ϸ״̬����֧Ԯ���ġ����С�
    // �󲿷ֹ��ж�������Ĭ�ϲ����¼���
	CardTrainingEffect getCardEffect(const Game& game, int atTrain=-1, int jiBan=0, int effectFactor=0) const;

	void getNNInputV1(float* buf) const;//���������룬size=NNINPUT_CHANNELS_CARD_V1

	void write_to_json(json& j, const std::string cdname, const int id) const;
	void load_from_json(json& j, int x);
};

// �������ݿ��еĹ��д�������Enum
enum class UniqueEffectType {
    None = 0,
    SpecialTagEffectUp = 1,
    MotivationUp = 2,
    TrainingSpeedUp = 3,
    TrainingStaminaUp = 4,
    TrainingPowerUp = 5,
    TrainingGutsUp = 6,
    TrainingWizUp = 7,
    TrainingEffectUp = 8,
    InitialSpeedUp = 9,
    InitialStaminaUp = 10,
    InitialPowerUp = 11,
    InitialGutsUp = 12,
    InitialWizUp = 13,
    InitalEvaluationUp = 14,
    RaceStatusUp = 15,
    RaceFanUp = 16,
    SkillTipsLvUp = 17,
    SkillTipsEventRateUp = 18,
    GoodTrainingRateUp = 19,
    SpeedLimitUp = 20,
    StaminaLimitUp = 21,
    PowerLimitUp = 22,
    GutzLimitUp = 23,
    WizLimitUp = 24,
    EventRecoveryAmountUp = 25,
    EventEffetcUp = 26,
    TrainingFailureRateDown = 27,
    TrainingHPConsumptionDown = 28,
    MinigameEffectUP = 29,
    SkillPointBonus = 30,
    WizRecoverUp = 31,
    AllStatusBonus = 99 // ����ӥ�ֶ����
};

//֧Ԯ����ѵ��Ч��
//�Ȱѿ����䵽��Ӧѵ���Ȼ��ż���CardTrainingEffect
// ��Ϊ��Ҫ�����������Զ�����һ��ͷ�ļ���
class CardTrainingEffect
{
public:
    bool isFixed = false; // ΪTrueʱ��ʾ���ٶԱ�����Ĺ������Խ��и���
    double youQing;//����ӳɣ�û���ʾ���0
    double ganJing;//�ɾ��ӳ�
    double xunLian;//ѵ���ӳ�
    double bonus[6];//����������pt�ļӳ�
    int vitalBonus;//�����ظ�������Ҫ���ǲ�Ȧ��
    double failRateDrop; //ʧ���ʽ���
    double vitalCostDrop; //���������½�

    int initialBonus[6];//��������������pt������
    int initialJiBan;//��ʼ�
    double saiHou;//����
    //int hintBonus[6];//Ϊ�˼򻯣��Ѻ��ļ��ܵ�Ч�ɶ�������
    //double hintProbIncrease;//��������������
    double deYiLv;//������

public:
    CardTrainingEffect() {}
    CardTrainingEffect(const SupportCard* sc);

    // ���д���ͨ�ô�����
    // key ��Ӧ enum class UniqueEffectType
    CardTrainingEffect& applyUniqueEffectLine(int key, int value);
};
