#pragma once
#include <string>


const int TOTAL_TURN = 78;//һ��78�غ�
static const int SHENTUAN_ID = 1;//���ſ���id

class GameConstants
{
public:
  //��Ϸ��ʼ��
  static const int TrainingBasicValue[5][6][7]; //TrainingBasicValue[�ڼ���ѵ��][LV��][����������pt����]����Ů����LV6
  static const int BasicFiveStatusLimit[5];//��ʼ���ޣ�1200���Ϸ���

  //������Ϸ����
  static const int NormalRaceFiveStatusBonus;//����������Լӳ�=3
  static const int NormalRacePtBonus;//�������pt�ӳ�

  //�籾�����
  static const double FriendUnlockOutgoingProbEveryTurn;//ÿ�غϽ�������ĸ���
  static const double FriendEventProb;//�����¼�����
  static const double ZuoyueVitalBonusSSR[5];//����SSR���Ļظ�������������1.8��
  static const double ZuoyueVitalBonusR[5];//����R���Ļظ�������
  static const double ZuoyueStatusBonusSSR[5];//����SSR�����¼�Ч������������1.2��
  static const double ZuoyueStatusBonusR[5];//����R�����¼�Ч������
  

  //�籾���
  static const int UpdateId50pEachTrain[5];//+50%�ֱ��Ӧ�ڼ�������
  static const int SupportPtEvery5Percent;//ÿ5%�ڴ��ȶ�Ӧ����������pt��=8500
  static const int LArcTrainBonusEvery5Percent[41];//�ڴ��ȶ�Ӧ��ѵ���ӳɣ�ÿ5%һ��

  //����
  static const int FiveStatusFinalScore[1200+800*2+1];//��ͬ���Զ�Ӧ������
  static const double ScorePtRate;//Ϊ�˷��㣬ֱ����Ϊÿ1pt��Ӧ���ٷ֡�
  static const double ScorePtRateQieZhe;//Ϊ�˷��㣬ֱ����Ϊÿ1pt��Ӧ���ٷ֡�����


};