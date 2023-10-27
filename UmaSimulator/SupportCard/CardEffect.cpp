#include "SupportCard.h"
#include "iostream"
#include "string"
#include "sstream"
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
   /*
    memcpy(initialBonus, sc->initialBonus, sizeof(int) * 6);
    initialJiBan = sc->initialJiBan;
    saiHou = sc->saiHou;
    deYiLv = sc->deYiLv;
    */
}

// ���д�����ͨ�ü��㷽��
// key ��Ӧ enum class UniqueEffectType
CardTrainingEffect& CardTrainingEffect::apply(int key, int value)
{
    switch (key)
    {
        case 1:  // ����-����
            youQing = (100 + youQing) * (100 + value) / 100 - 100;
            break;
        case 2:    // �ɾ�-����
            ganJing += value; break;
        case 8:    // ѵ��-����
            xunLian += value; break;
    //  case 19:  // ������-����
    //     deYiLv = (100 + deYiLv) * (100 + value) / 100 - 100;
    //     break;
    //  case 15:    // ����-����
    //    saiHou += value;
    //    break;
        case 27: // ʧ����-����
            failRateDrop = 100 - (100 - failRateDrop) * (100 - value) / 100;
            break;
        case 28: // ����-����
            vitalCostDrop = 100 - (100 - vitalCostDrop) * (100 - value) / 100;
            break;
        case 31:    // ��������UP-���㣨ɵ��������
            vitalBonus += value; break;
        case 3:    // ������
            bonus[0] += value; break;
        case 4:
            bonus[1] += value; break;
        case 5:
            bonus[2] += value; break;
        case 6:
            bonus[3] += value; break;
        case 7:
            bonus[4] += value; break;
        case 30:
            bonus[5] += value; break;
        /*
        case 9: // ��ʼ����
            initialBonus[0] += value; break;
        case 10:
            initialBonus[1] += value; break;
        case 11:
            initialBonus[2] += value; break;
        case 12:
            initialBonus[3] += value; break;
        case 13:
            initialBonus[4] += value; break;
        case 14:  // ��ʼ�
            initialJiBan += value; break;
        */
        case 17:    // Hint�ȼ�
        case 18:    // Hint�ʣ�������
            break;
        // ��ӥ����
        case 99:  // ����ӥ
            for (int i = 0; i < 5; ++i)
                bonus[i] += 1;
            break;
        default:
            cout << "δ֪����: " << key << " = " << value << endl;
            break;
    }
    return *this;
}

const string& CardTrainingEffect::explain() {
    stringstream ss;
    ss << std::setprecision(4);
    if (youQing > 0)
        ss << "[��Ȧ]����=" << youQing << ' ';
    if (ganJing > 0)
        ss << "�ɾ�=" << ganJing << ' ';
    if (xunLian > 0)
        ss << "ѵ��=" << xunLian << ' ';
    if (failRateDrop > 0)
        ss << "ʧ����-" << failRateDrop << ' ';
    if (vitalCostDrop > 0)
        ss << "��������-" << vitalCostDrop << ' ';
    ss << "������=( ";
    for (int i = 0; i < 6; ++i)
        ss << bonus[i] << ' ';
    ss << ")";
    return ss.str();
}