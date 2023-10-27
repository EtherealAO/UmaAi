#include <iostream>
#include <cassert>
#include"NNInput.h"
#include "../Game/Game.h"
using namespace std;


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


  //�Ƿ�link��link�Ĺ̶�buff����Person::getNNInputV1��д

  //buf[27] = larc_isLink ? 1.0 : 0.0;
  //if (larc_linkSpecialEffect != 0)//��Χ3~12
  //  buf[28 + (larc_linkSpecialEffect - 3)] = 1.0;


  assert(false && "todo���д���");
  buf[27] = 0.0;

}


void Person::getNNInputV1(float* buf, const Game& game, int index) const
{

  for (int ch = 0; ch < NNINPUT_CHANNELS_PERSON_V1; ch++)
    buf[ch] = 0;


  //PersonType����д�����ڹ̶�λ�þ���
  //charaId����д
  //cardIdInGame����д���Ϳ����������ͬһ��λ�þ���
  buf[0] = double(friendship) / 100.0;
  buf[1] = friendship >= 80 ? 1.0 : 0.0;
  buf[2] = friendship >= 100 ? 1.0 : 0.0;
  buf[3] = isShining ? 1.0 : 0.0;
  buf[4] = isHint ? 1.0 : 0.0;
  buf[5] = 0.0;//Ԥ��cardRecord
  buf[6] = 0.0;//Ԥ��cardRecord
  
  //���ĸ�ѵ��
  for (int tr = 0; tr < 5; tr++)
  {
    for (int i = 0; i < 5; i++)
    {
      if (game.personDistribution[tr][i] == index)
        buf[7 + tr] = 1.0;
    }
  }

  //�Ƿ���ss
  for (int i = 0; i < 5; i++)
    if(game.larc_ssPersons[i]==index)
      buf[12] = 1.0;


  if (game.turn >= 2 && index < 15)
  {
    buf[13] = larc_isLinkCard ? 1.0 : 0.0;
    buf[14 + larc_charge] = 1.0;
    buf[17 + larc_statusType] = 1.0;
    assert(larc_specialBuff >= 3 && larc_specialBuff <= 12);
    buf[22 + larc_specialBuff - 3] = 1.0;
    //larc_level��ȫ����
    buf[32 + (larc_buffLevel % 3)] = 1.0;
    buf[35 + larc_nextThreeBuffs[0]] = 1.0;
    buf[48 + larc_nextThreeBuffs[1]] = 1.0;
    buf[61 + larc_nextThreeBuffs[2]] = 1.0;
  }
  else
  {
    if (personType == 2)
    {
      bool islink = game.cardParam[cardIdInGame].larc_isLink;
      if (islink)
      {
        buf[13] = 1.0;
        int specialBuff = game.cardParam[cardIdInGame].larc_linkSpecialEffect;
        assert(specialBuff >= 3 && specialBuff <= 12);
        buf[22 + specialBuff - 3] = 1.0;

      }
    }
  }

  //total 74

}







void Game::getNNInputV1(float* buf, float targetScore, int mode) const
{
  /*
  for (int i = 0; i < NNINPUT_CHANNELS_V1; i++)buf[i] = 0.0;
  if (isEnd())return;
  //stageInTurn=0�ǻ�û���俨�飬�����������һ�´����ʣ�value����stageInTurn=1�Ƿ����˿���ȴ����ѡ�������������ÿ��ѡ�������Ž�ĸ��ʣ�policy��
  assert((stageInTurn == 0 && mode == 0) || (stageInTurn == 1 && mode == 1));
  
  //0~77 �غ���
  assert(turn >= 0 && turn < TOTAL_TURN);
  buf[turn] = 1.0;

  //78 ����
  buf[78] = (vital-50.0) / 30.0;

  //79 ��������
  buf[79] = (maxVital-100.0) / 10.0;

  //80 ����
  buf[80] = isQieZhe;

  //81 ����
  buf[81] = isAiJiao;

  //82~83 ��ϰ���ֺ���ϰ����
  if (failureRateBias > 0)
    buf[82] = failureRateBias / 2.0;
  else if (failureRateBias < 0)
    buf[83] = - failureRateBias / 2.0;

  //84~88 ��ά����
  for (int i = 0; i < 5; i++)
    buf[84 + i] = fiveStatus[i] / 1000.0;

  //89~93 ��ά��������
  for (int i = 0; i < 5; i++)
    buf[89 + i] = (fiveStatusLimit[i]-GameConstants::BasicFiveStatusLimit[i]) / 200.0;

  //94 ���ܵ�
  // ֱ�ӻ��㵽Ŀ��������ˣ����Բ���Ҫ�����ˡ�
  //buf[94] = skillPt / 2000.0;

  //95~99 �ɾ�
  buf[95 - 1 + motivation] = 1.0;

  //v1�汾��cardId����Ϊ����

  //100~105 �
  for (int i = 0; i < 6; i++)
    buf[100 + i] = cardJiBan[i] / 100.0;

  //106~111 ��Ƿ񲻵���80
  for (int i = 0; i < 6; i++)
    buf[106 + i] = cardJiBan[i] >= 80;

  //112~116 ѵ���ȼ�����
  for (int i = 0; i < 5; i++)
    buf[112 + i] = trainLevelCount[i] / 48.0;

  //117~146 ѵ���ȼ���5x6��
  for (int i = 0; i < 5; i++)
    buf[117 + i * 6 + getTrainingLevel(i)] = 1.0;

  //147~151 ����������
  for (int i = 0; i < 5; i++)
    buf[147 + i] = zhongMaBlueCount[i] / 9.0;

  //152~157 �����μ̳�����
  for (int i = 0; i < 5; i++)
    buf[152 + i] = zhongMaExtraBonus[i] / 30.0;
  buf[157] = zhongMaExtraBonus[5] / 200.0;

  //158 �Ƿ�Ϊ����
  if (isRacing)
    buf[158] = 1.0;

  //159~176 Ů��ȼ�
  buf[159 + venusLevelRed] = 1.0;
  buf[165 + venusLevelBlue] = 1.0;
  buf[171 + venusLevelYellow] = 1.0;

  //177~302 ������Ƭ
  //��Ƭ��ÿ����Ƭ��6ͨ����ʾ���ԣ�3ͨ����ʾ��ɫ
  //һ��(8+4+2)*9=126ͨ��

  //177~248 �ײ���Ƭ
  for (int i = 0; i < 8; i++)
  {
    int s = venusSpiritsBottom[i];
    if (s == 0)continue;
    int channel0 = 177 + i * 9;
    int type = s % 8 - 1;
    assert(type >= 0 && type < 6);
    int color = s / 8;
    assert(color >= 0 && color < 3);

    buf[channel0 + type] = 1.0;
    buf[channel0 + 6 + color] = 1.0;
  }

  //249~302 �ϲ���Ƭ
  for (int i = 0; i < 6; i++)
  {
    int s = venusSpiritsUpper[i];
    if (s == 0)continue;
    int channel0 = 249 + i * 9;
    int type = s % 8 - 1;
    assert(type >= 0 && type < 6);
    int color = s / 8;
    assert(color >= 0 && color < 3);

    buf[channel0 + type] = 1.0;
    buf[channel0 + 6 + color] = 1.0;
  }

  //303~305 ���õ�Ů�����
  if (venusAvailableWisdom != 0)
    buf[303 - 1 + venusAvailableWisdom] = 1.0;

  //306 Ů������Ƿ���
  buf[306] = venusIsWisdomActive;

  //307 �Ƿ���������
  buf[307] = venusCardFirstClick;

  //308 �����Ƿ�������
  buf[308] = venusCardUnlockOutgoing;

  //309 �����Ƿ�����
  buf[309] = venusCardIsQingRe;

  //310~320 ���������˼����غ�
  buf[310 + venusCardQingReContinuousTurns] = 1.0;

  //321~325 Ů����������ļ���
  for (int i = 0; i < 5; i++)
    buf[321 + i] = venusCardOutgoingUsed[i];

  //326~330 stageInTurn(��ʱ����)
  assert(stageInTurn >= 0 && stageInTurn < 5);
  buf[326 + stageInTurn] = 1.0;

  if (mode == 1)
  {
    //331~370 ֧Ԯ���ֲ�
    for (int i = 0; i < 5; i++)
      for (int j = 0; j < 8; j++)
        buf[331 + 8 * i + j] = cardDistribution[i][j];

    //371~376 ֧Ԯ�����
    for (int i = 0; i < 6; i++)
      buf[371 + i] = cardHint[i];
  }

  //377~456 ѵ����Ƭ
  //ÿ����Ƭ��6ͨ����ʾ���ԣ�3ͨ����ʾ��ɫ��1ͨ����ʾ�Ƿ�˫��
  //һ��8*10=80ͨ��
  for (int i = 0; i < 8; i++)
  {
    int s = venusSpiritsUpper[i];
    if (s == 0)continue;
    bool doubled = s >= 32;
    s = s % 32;
    int channel0 = 377 + i * 10;
    int type = s % 8 - 1;
    assert(type >= 0 && type < 6);
    int color = s / 8;
    assert(color >= 0 && color < 3);

    buf[channel0 + type] = 1.0;
    buf[channel0 + 6 + color] = 1.0;
    buf[channel0 + 9] = doubled;
  }

  //457~462 ��Ƭ�ӳ�
  for (int i = 0; i < 6; i++)
    buf[457 + i] = spiritBonus[i] / 10.0;

  if (mode == 1)
  {
    //463~492 ѵ������
    for (int i = 0; i < 5; i++)
      for (int j = 0; j < 6; j++)
        buf[463 + 6 * i + j] = trainValue[i][j] / 50.0;

    //493~497 ѵ������
    for (int i = 0; i < 5; i++)
      buf[493 + i] = trainValue[i][6] / 20.0;

    //498~502 ѵ��ʧ����
    for (int i = 0; i < 5; i++)
      buf[498 + i] = failRate[i] / 30.0;

    //503~507 ѵ��ʧ����>0
    for (int i = 0; i < 5; i++)
      buf[503 + i] = failRate[i] > 0;

    //508~512 ѵ��ʧ����>=20
    for (int i = 0; i < 5; i++)
      buf[508 + i] = failRate[i] >= 20;
  }

  //513~599 ����id
  //static_assert(GameDatabase::ALL_UMA_NUM <= 599 - 513, "����v1��������֧������������");
  assert(umaId >= 0 && umaId + 513 <= 599);
  buf[513 + umaId] = 1.0;

  //600~899 ֧Ԯ��id����i��λ��֧Ԯ��Ϊj����buf[600+j*6+i]=1
  //assert(GameDatabase::ALL_SUPPORTCARD_NUM <= 50, "����v1��������֧��֧Ԯ��������");
  for (int i = 0; i < 6; i++)
    buf[600 + cardId[i] * 6 + i] = 1.0;
  
  //900 Ŀ�����
  float remainScoreExceptPt = targetScore - getSkillScore();
  buf[900] = (remainScoreExceptPt - 25000) / 2000.0;

  //901~902 ģʽ
  assert(901 + mode < NNINPUT_CHANNELS_V1);
  buf[901 + mode] = 1.0;



  */










}