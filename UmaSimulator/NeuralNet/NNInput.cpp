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

  buf[27] = 0.0; //reserve
  buf[28] = 0.0;

  //�Ƿ�link��link�Ĺ̶�buff����Person::getNNInputV1��д


  //����
  assert(isDBCard);
  const int BasicC = 29;
  const int UniqueTypeC = 25;
  const int UniqueEffectC = 13;
  static_assert(BasicC + UniqueTypeC + UniqueEffectC == NNINPUT_CHANNELS_CARD_V1);
  float* bufUniqueType = buf + BasicC;
  float* bufUniqueValue = bufUniqueType + UniqueTypeC;

  auto writeUniqueEffect = [&](int key, double value)
  {
    //�����ǣ�0~4���������ǣ�5pt��6���飬7�ɾ���8ѵ����9ʧ���ʣ�10�������Ѽ��٣�11������Ȧ����
    if (key <= 0)
      return;
    else if (key == 1)
    {
      bufUniqueValue[6] = 0.04 * value;
    }
    else if (key == 2)
    {
      bufUniqueValue[7] = 0.02 * value;
    }
    else if (key == 8)
    {
      bufUniqueValue[8] = 0.05 * value;
    }
    else if (key == 19)
    {
    }
    else if (key == 15)
    {
    }
    else if (key == 27)
    {
      bufUniqueValue[9] = 0.04 * value;
    }
    else if (key == 28)
    {
      bufUniqueValue[10] = 0.05 * value;
    }
    else if (key == 31)
    {
      bufUniqueValue[11] = 0.2 * value;
    }
    else if (key == 3)
    {
      bufUniqueValue[0] = 0.5 * value;
    }
    else if (key == 4)
    {
      bufUniqueValue[1] = 0.5 * value;
    }
    else if (key == 5)
    {
      bufUniqueValue[2] = 0.5 * value;
    }
    else if (key == 6)
    {
      bufUniqueValue[3] = 0.5 * value;
    }
    else if (key == 7)
    {
      bufUniqueValue[4] = 0.5 * value;
    }
    else if (key == 30)
    {
      bufUniqueValue[5] = 0.5 * value;
    }
    else if (key == 41)
    {
      for (int i = 0; i < 5; i++)
        bufUniqueValue[i] = 0.5;
    }
    else
    {
      assert(false);
    }
  };

  if (uniqueEffectType == 0)
  {
    bufUniqueType[0] = 1.0;
  }
  else if (uniqueEffectType == 1 || uniqueEffectType == 2)
  {
    if (uniqueEffectParam[1] == 80)
      bufUniqueType[1] = 1.0;
    else if (uniqueEffectParam[1] == 100)
      bufUniqueType[2] = 1.0;
    else
      assert(false);

    writeUniqueEffect(uniqueEffectParam[2], uniqueEffectParam[3]);
    writeUniqueEffect(uniqueEffectParam[4], uniqueEffectParam[5]);
    if (cardID / 10 == 30137)
    {
      writeUniqueEffect(1, 10);
      writeUniqueEffect(2, 15);
    }
  }
  else if (uniqueEffectType == 3)
  {
    bufUniqueType[3] = 1.0;
  }
  else if (uniqueEffectType == 4)
  {
    bufUniqueType[4] = 1.0;
  }
  else if (uniqueEffectType == 5)
  {
    
  }
  else if (uniqueEffectType == 6)
  {
    bufUniqueType[6] = 1.0;
    writeUniqueEffect(1, uniqueEffectParam[1] * uniqueEffectParam[3]);
  }
  else if (uniqueEffectType == 7)
  {
    bufUniqueType[7] = 1.0;
  }
  else if (uniqueEffectType == 8)
  {
    bufUniqueType[8] = 1.0;
  }
  else if (uniqueEffectType == 9)
  {
    bufUniqueType[9] = 1.0;
  }
  else if (uniqueEffectType == 10)
  {
    bufUniqueType[10] = 1.0;
  }
  else if (uniqueEffectType == 11)
  {
    bufUniqueType[11] = 1.0;
  }
  else if (uniqueEffectType == 12)
  {
    bufUniqueType[12] = 1.0;
  }
  else if (uniqueEffectType == 13)
  {
    bufUniqueType[13] = 1.0;
  }
  else if (uniqueEffectType == 14)
  {
    bufUniqueType[14] = 1.0;
  }
  else if (uniqueEffectType == 15)
  {
  }
  else if (uniqueEffectType == 16)
  {
    bufUniqueType[16] = 1.0;
    writeUniqueEffect(uniqueEffectParam[2], 5 * uniqueEffectParam[3]);
  }
  else if (uniqueEffectType == 19)
  {
    bufUniqueType[19] = 1.0;
    writeUniqueEffect(uniqueEffectParam[2], 3 * uniqueEffectParam[3]);
  }
  else if (uniqueEffectType == 20)
  {
    bufUniqueType[20] = 1.0;
    writeUniqueEffect(uniqueEffectParam[2], 3 * uniqueEffectParam[3]);
  }
  else if (uniqueEffectType == 17)
  {
    bufUniqueType[17] = 1.0;
    writeUniqueEffect(8, uniqueEffectParam[3]);
  }
  else if (uniqueEffectType == 18)
  {
    bufUniqueType[18] = 1.0;
  }
  else
  {
    assert(false);
  }



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







void Game::getNNInputV1(float* buf, const SearchParam& param) const
{
  for (int i = 0; i < NNINPUT_CHANNELS_V1; i++)
    buf[i] = 0.0;
  int c = 0;

  for (int i = 0; i < 5; i++)
  {
    buf[c + i] = fiveStatusBonus[i] * 0.05;
  }
  c += 5;

  assert(turn < TOTAL_TURN);
  buf[c + turn] = 1.0;
  c += TOTAL_TURN;

  buf[c] = (vital - 50) * 0.01;
  c++;
  buf[c] = (maxVital - 100) * 0.1;
  c++;
  if (isQieZhe)
    buf[c] = 1.0;
  c++;
  if (isAiJiao)
    buf[c] = 1.0;
  c++;

  buf[c] = failureRateBias * 0.5;
  c++;

  for (int i = 0; i < 5; i++)
    buf[c + i] = fiveStatus[i] * 0.001;
  c += 5;
  for (int i = 0; i < 5; i++)
    buf[c + i] = (fiveStatusLimit[i] - GameConstants::BasicFiveStatusLimit[i]) * 0.01;
  c += 5;

  buf[c] = getSkillScore() * 0.0002;
  c++;

  assert(motivation >= 1 && motivation <= 5);
  buf[c + motivation - 1] = 1.0;
  c += 5;

  if (isPositiveThinking)
    buf[c] = 1.0;
  c++;

  for (int i = 0; i < 5; i++)
    buf[c + i] = (fiveStatusLimit[i] - GameConstants::BasicFiveStatusLimit[i]) * 0.01;
  c += 5;


  for (int i = 0; i < 5; i++)
  {
    assert(trainLevelCount[i] <= 16);
    int a = trainLevelCount[i] / 4;
    int b = trainLevelCount[i] % 4;
    buf[c + a] = 1.0;
    buf[c + b + 4] = 1.0;
    c += 8;
  }

  for (int i = 0; i < 5; i++)
  {
    buf[c + i] = zhongMaBlueCount[i] * 0.1;
  }
  c += 5;

  for (int i = 0; i < 5; i++)
  {
    buf[c + i] = zhongMaExtraBonus[i] * 0.03;
  }
  c += 5;
  buf[c] = zhongMaExtraBonus[5] * 0.01;
  c++;

  buf[c + normalCardCount] = 1.0;
  c += 7;

  buf[c] = saihou * 0.03;
  c++;

  assert(!isRacing);
  assert(motivationDropCount == 0);//not used

  if (larc_isAbroad)
    buf[c] = 1.0;
  c++;

  buf[c] = larc_supportPtAll / 170000.0;
  c++;

  int larc_trainBonusLevel = (larc_supportPtAll + 85) / 8500;
  if (larc_trainBonusLevel > 40)larc_trainBonusLevel = 40;
  double larc_trainBonus = GameConstants::LArcTrainBonusEvery5Percent[larc_trainBonusLevel];
  buf[c] = larc_trainBonus * 0.05;
  c++;

  int trainBonusLevel20p = larc_trainBonusLevel / 4;
  if (trainBonusLevel20p > 5)trainBonusLevel20p = 5;
  buf[c + trainBonusLevel20p] = 1.0;
  c += 6;

  buf[c] = larc_shixingPt * 0.002;
  c++;


  for (int i = 0; i < 10; i++)
  {
    int lv = larc_levels[i];
    assert(lv <= 3);
    if (lv > 0)
      buf[c + lv - 1] = 1.0;
    c += 3;
  }

  if (larc_isSSS)
    buf[c] = 1.0;
  c++;

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