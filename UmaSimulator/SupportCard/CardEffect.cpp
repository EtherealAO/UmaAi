#include "SupportCard.h"
#include "iostream"
using namespace std;

CardTrainingEffect::CardTrainingEffect(const SupportCard* sc)
{
    youQing = sc->youQingBasic;
    ganJing = sc->ganJingBasic;
    xunLian = sc->xunLianBasic;
    memcpy(bonus, sc->bonusBasic, sizeof(double) * 6);
    vitalBonus = sc->wizVitalBonusBasic;
    failRateDrop = sc->failRateDrop;
    vitalCostDrop = sc->vitalCostDrop;
    memcpy(initialBonus, sc->initialBonus, sizeof(int) * 6);
    initialJiBan = sc->initialJiBan;
    saiHou = sc->saiHou;
    deYiLv = sc->deYiLv;
}

// ���д�����ͨ�ü��㷽��
// key ��Ӧ enum class UniqueEffectType
CardTrainingEffect& CardTrainingEffect::applyUniqueEffectLine(int key, int value)
{
    switch (key)
    {
        case (int)UniqueEffectType::SpecialTagEffectUp:  // ����-����
            youQing = (100 + youQing) * (100 + value) / 100 - 100;
            break;
        case (int)UniqueEffectType::MotivationUp:    // �ɾ�-����
            ganJing += value;
            break;
        case (int)UniqueEffectType::GoodTrainingRateUp:  // ������-����
            deYiLv = (100 + deYiLv) * (100 + value) / 100 - 100;
            break;
        case (int)UniqueEffectType::TrainingEffectUp:    // ѵ��-����
            xunLian += value; break;
        case (int)UniqueEffectType::RaceStatusUp:    // ����-����
            saiHou += value;
            break;
        case (int)UniqueEffectType::TrainingFailureRateDown: // ʧ����-����
            failRateDrop = 100 - (100 - failRateDrop) * (100 - value) / 100;
            break;
        case (int)UniqueEffectType::TrainingHPConsumptionDown: // ����-����
            vitalCostDrop = 100 - (100 - vitalCostDrop) * (100 - value) / 100;
            break;
        case (int)UniqueEffectType::WizRecoverUp:    // ��������UP-���㣨ɵ��������
            vitalBonus += value; break;
        case (int)UniqueEffectType::TrainingSpeedUp:    // ������
            bonus[0] += value; break;
        case (int)UniqueEffectType::TrainingStaminaUp:
            bonus[1] += value; break;
        case (int)UniqueEffectType::TrainingPowerUp:
            bonus[2] += value; break;
        case (int)UniqueEffectType::TrainingGutsUp:
            bonus[3] += value; break;
        case (int)UniqueEffectType::TrainingWizUp:
            bonus[4] += value; break;
        case (int)UniqueEffectType::SkillPointBonus:
            bonus[5] += value; break;
        case (int)UniqueEffectType::InitialSpeedUp: // ��ʼ����
            initialBonus[0] += value; break;
        case (int)UniqueEffectType::InitialStaminaUp:
            initialBonus[1] += value; break;
        case (int)UniqueEffectType::InitialPowerUp:
            initialBonus[2] += value; break;
        case (int)UniqueEffectType::InitialGutsUp:
            initialBonus[3] += value; break;
        case (int)UniqueEffectType::InitialWizUp:
            initialBonus[4] += value; break;
        case (int)UniqueEffectType::InitalEvaluationUp:  // ��ʼ�
            initialJiBan += value; break;
            // ���Ե�����
        case (int)UniqueEffectType::RaceFanUp:   // ��˿
        case (int)UniqueEffectType::SkillTipsEventRateUp:    // Hint��
        case (int)UniqueEffectType::SkillTipsLvUp:   // Hint�ȼ�
            break;
            // ��ӥ����
        case (int)UniqueEffectType::AllStatusBonus:  // ����ӥ
            for (int i = 0; i < 5; ++i)
                bonus[i] += 1;
            break;
        default:
            cout << "δ֪���д���ID: " << key << " = " << value << endl;
            break;
    }
    return *this;
}
