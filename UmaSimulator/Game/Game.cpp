#include <iostream>
#include <cassert>
#include "Game.h"
using namespace std;
static bool randBool(mt19937_64& rand, double p)
{
  return rand() % 65536 < p * 65536;
}

void Game::newGame(mt19937_64& rand, bool enablePlayerPrint, int newUmaId, int umaStars, int newCards[6], int newZhongMaBlueCount[5], int newZhongMaExtraBonus[6])
{
  playerPrint = enablePlayerPrint;

  umaId = newUmaId;
  for (int i = 0; i < 5; i++)
    fiveStatusBonus[i] = GameDatabase::AllUmas[umaId].fiveStatusBonus[i];
  turn = 0;
  vital = 100;
  maxVital = 100;
  isQieZhe = false;
  isAiJiao = false; 
  failureRateBias = 0;
  skillPt = 120;
  skillScore = umaStars >= 3 ? 170 * (1 + umaStars) : 120 * (3 + umaStars);//���м���
  motivation = 3;
  isPositiveThinking = false;
  for (int i = 0; i < 5; i++)
    trainLevelCount[i] = 0;
  isRacing = false;

  larc_zuoyueType = 0;
  larc_zuoyueVitalBonus = 0;
  larc_zuoyueStatusBonus = 0;
  for (int i = 0; i < 18; i++)
  {
    persons[i] = Person();
  }
  persons[15].personType = 4;
  persons[16].personType = 5;
  persons[17].personType = 6;
  for (int i = 0; i < 18; i++)
  {
    if (persons[i].personType == 0)
      persons[i].personType = 3;
  }


  normalCardCount = 0;//���������ǿ�������
  saihou = 0;
  for (int i = 0; i < 6; i++)
  {
    int cardId = newCards[i];
    cardParam[i] = GameDatabase::AllCards[cardId];
    SupportCard& cardP = cardParam[i];
    saihou += cardP.saiHou;
    int cardType = cardP.cardType;
    if (cardType == 5 || cardType == 6)
    {

      int realCardId = cardId / 10;
      if (realCardId == 30160 || realCardId == 10094)//������
      {
        if (realCardId == 30160)
          larc_zuoyueType = 1;
        else 
          larc_zuoyueType = 2;

        int zuoyueLevel = cardId % 10;
        if (larc_zuoyueType==1)
        {
          larc_zuoyueVitalBonus = GameConstants::ZuoyueVitalBonusSSR[zuoyueLevel];
          larc_zuoyueStatusBonus = GameConstants::ZuoyueStatusBonusSSR[zuoyueLevel];
        }
        else
        {
          larc_zuoyueVitalBonus = GameConstants::ZuoyueVitalBonusR[zuoyueLevel];
          larc_zuoyueStatusBonus = GameConstants::ZuoyueStatusBonusR[zuoyueLevel];
        }
        larc_zuoyueVitalBonus += 1e-10;
        larc_zuoyueStatusBonus += 1e-10;//�Ӹ�С����������Ϊ�����������

        persons[17].personType = 1;
        persons[17].cardIdInGame = i;
      }
      else
      {
        throw string("��֧�ִ�������������˻��Ŷӿ�");
      }
    }
    else//���������ǿ�
    {
      Person& p = persons[normalCardCount];
      normalCardCount += 1;
      p.personType = 2;
      p.cardIdInGame = i;
      p.friendship = cardP.initialJiBan;
      p.larc_isLinkCard = cardP.larc_isLink;

      std::vector<int> probs = { 100,100,100,100,100,50 }; //�������ʣ����������Ǹ�
      probs[cardP.cardType] += cardP.deYiLv;
      p.distribution = std::discrete_distribution<>(probs.begin(), probs.end());
    }
  }


  motivationDropCount = 0;

  larc_isAbroad = false;
  larc_supportPtAll = 0;
  larc_shixingPt = 0;
  for (int i = 0; i < 10; i++)larc_levels[i] = 0;
  larc_isSSS = false;
  larc_ssWin = 0;
  larc_ssWinSinceLastSSS = 0;
  larc_isFirstLarcWin = false;
  for (int i = 0; i < 3; i++)for (int j = 0; j < 8; j++)
    larc_allowedDebuffsFirstLarc[i][j] = false;

  //larc_zuoyueType
  //larc_zuoyueCardLevel
  larc_zuoyueFirstClick = false;
  larc_zuoyueOutgoingUnlocked = false; 
  larc_zuoyueOutgoingRefused = false;
  larc_zuoyueOutgoingUsed = 0;


  for (int i = 0; i < 5; i++)
    zhongMaBlueCount[i] = newZhongMaBlueCount[i];
  for (int i = 0; i < 6; i++)
    zhongMaExtraBonus[i] = newZhongMaExtraBonus[i];


  for (int i = 0; i < 5; i++)
    fiveStatusLimit[i] = GameConstants::BasicFiveStatusLimit[i]; //ԭʼ��������
  for (int i = 0; i < 5; i++)
    fiveStatusLimit[i] += int(zhongMaBlueCount[i] * 5.34 * 2); //��������--�������ֵ

  //�����μ̳е����飬��ʱ����˵
  //for (int i = 0; i < 5; i++)
   // fiveStatusLimit[i] += rand() % 20; //��������--�����μ̳��������


  for (int i = 0; i < 5; i++)
    fiveStatus[i] = GameDatabase::AllUmas[umaId].fiveStatusInitial[i]; //�������ʼֵ
  for (int i = 0; i < 6; i++)//֧Ԯ����ʼ�ӳ�
  {
    for (int j = 0; j < 5; j++)
      addStatus(j, cardParam[i].initialBonus[j]);
    skillPt += cardParam[i].initialBonus[5];
  }
  for (int i = 0; i < 5; i++)
    addStatus(i, zhongMaBlueCount[i] * 7); //����



  //initRandomGenerators();

  stageInTurn = 0;
  larc_ssPersonsCountLastTurn = 0;
  randomDistributeCards(rand); 
}

void Game::initNPCsTurn3(std::mt19937_64& rand)
{
  int allSpecialBuffsNum[13] = {0,0,0,1,1,2,2,4,1,1,0,0,3};
  int specialBuffEveryPerson[15];
  for (int i = 0; i < 15; i++)specialBuffEveryPerson[i] = 0;

  //�������ſ��̶�������buff
  for (int i = 0; i < normalCardCount; i++)
  {
    assert(persons[i].personType == 2);
    int s = cardParam[persons[i].cardIdInGame].larc_linkSpecialEffect;
    if (s != 0)
    {
      specialBuffEveryPerson[i] = s;
      allSpecialBuffsNum[s] -= 1;
      assert(allSpecialBuffsNum[s] >= 0);
    }
    
  }

  //û�̶�����buff�ľ������������buff
  vector<int> specialBuffNotAssigned;
  for (int i = 0; i < 13; i++)
  {
    int n = allSpecialBuffsNum[i];
    if (n >= 0)
    {
      for (int j = 0; j < n; j++)
        specialBuffNotAssigned.push_back(i);
    }
  }
  std::shuffle(specialBuffNotAssigned.begin(), specialBuffNotAssigned.end(), rand);
  int c = 0;
  for (int i = 0; i < 15; i++)
  {
    if (specialBuffEveryPerson[i] == 0)
    {
      specialBuffEveryPerson[i] = specialBuffNotAssigned[c];
      c += 1;
    }
  }

  //��ͷ����
  vector<int> s = { 0,0,0,1,1,1,2,2,2,3,3,3,4,4,4 };
  assert(s.size() == 15);
  std::shuffle(s.begin(), s.end(), rand);

  //��ʼ��
  for (int i = 0; i < 15; i++)
  {
    persons[i].initAtTurn3(rand, specialBuffEveryPerson[i], s[i]);
  }


}

void Game::randomDistributeCards(std::mt19937_64& rand)
{
  //assert(stageInTurn == 0 || turn == 0);
  stageInTurn = 1;

  //�����غϵ���ͷ����ͱ���/Զ���غϵ�ss������Ҫ���㣬��Ϊ������������
  if (isRacing)
    return;//�������÷��俨�飬��Ҫ��stageInTurn
  
  //�Ƚ�6�ſ����䵽ѵ����
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      personDistribution[i][j] = -1;

  int headCountEveryTrain[5] = { 0,0,0,0,0 };//���ѵ���ֱ��ж����ˣ�����5��Ҳ������

  //��һ����ͷ����ĳ��ѵ��������������5���и�������ߵ�һ��
  auto setHead = [&](int head, int whichTrain)
  {
    if (whichTrain >= 5)return;
    int p = headCountEveryTrain[whichTrain];
    if (p < 5)//��û5��ͷ
    {
      personDistribution[whichTrain][p] = head;
    }
    else
    {
      // ��Ϲ�µĹ�ʽ�����Ա�֤������ͷ��λƽ�ȣ�5/(p+1)�ĸ����滻������һ��ͷ��(p-4)/(p+1)�ĸ��ʸ�
      int r = rand() % (p + 1);
      if (r < 5)//�������һ����
        personDistribution[whichTrain][r] = head;
    }
    headCountEveryTrain[whichTrain] += 1;//�����Ƿ��������ѵ�����������һ
  };


  for (int i = 0; i < 18; i++)
  {
    if (turn < 2 && persons[i].personType != 2)//ǰ���غ�û��������npc
      continue;
    if (larc_isAbroad && (i == 15 || i == 16))//Զ��ʱ���³����߲���
      continue;

    if (i == 17 && larc_zuoyueType == 1 && persons[i].friendship >= 60)//ssr�������60���������˵�������
    {
      int whichTrain1 = rand() % 6;
      setHead(i, whichTrain1);
      if (whichTrain1 == 5)
      {
        int whichTrain2 = rand() % 6;
        setHead(i, whichTrain2);
      }
      else
      {
        //����ڶ���λ�ú͵�һ������ײ����������ʵ����ʱȽϽӽ�
        int whichTrain2 = rand() % 5;
        if (whichTrain2 >= whichTrain1)whichTrain2++;
        setHead(i, whichTrain2);
      }
    }
    else
    {
      int whichTrain = persons[i].distribution(rand);
      setHead(i, whichTrain);
    }

    //�Ƿ���hint
    if (persons[i].personType == 2)
    {
      double hintProb = 0.06 * (1 + 0.01 * cardParam[persons[i].cardIdInGame].hintProbIncrease);
      bernoulli_distribution d(hintProb);
      persons[i].isHint = d(rand);
    }
  }
  
  //����ss��ͷ
  if (!larc_isAbroad)
  {
    int fullNum = 0;
    for (int i = 0; i < 5; i++)
      larc_ssPersons[i] = -1;
    for (int i = 0; i < 15; i++)
    {
      if (persons[i].larc_charge == 3)
      {
        if (fullNum < 5)
          larc_ssPersons[fullNum] = i;
        else
        {
          int r = rand() % (fullNum + 1);
          if (r < 5)//�������һ����
            larc_ssPersons[r] = i;
        }
        fullNum += 1;
      }
    }
    larc_ssPersonsCount = fullNum > 5 ? 5 : fullNum;

    bool isNewFullSS = larc_ssPersonsCount >= 5 && larc_ssPersonsCountLastTurn < 5;//Ϊ�˱�����10����������ssʱ�������ʹ��ss��ʱ����������
    larc_ssPersonsCountLastTurn = larc_ssPersonsCount;
    if (isNewFullSS)
    {
      larc_isSSS = randBool(rand, sssProb(larc_ssWinSinceLastSSS));
    }
  }
}

void Game::calculateTrainingValue()
{
  //�ȼ������Եȼ��ӳ�
  for (int i = 0; i < 6; i++)
    larc_staticBonus[i] = 0;
  for (int i = 0; i < 5; i++)
    if (larc_levels[GameConstants::UpdateId50pEachTrain[i]] >= 1)
      larc_staticBonus[i] += 3;

  if (larc_levels[5] >= 1)
    larc_staticBonus[5] += 10;
  if (larc_levels[5] >= 3)
    larc_staticBonus[5] += 10;
  if (larc_levels[6] >= 1)
    larc_staticBonus[1] += 3;
  if (larc_levels[7] >= 1)
    larc_staticBonus[3] += 3;

  int larc_trainBonusLevel = larc_supportPtAll / GameConstants::SupportPtEvery5Percent;
  if (larc_trainBonusLevel > 40)larc_trainBonusLevel = 40;
  larc_trainBonus = GameConstants::LArcTrainBonusEvery5Percent[larc_trainBonusLevel];



  for (int trainType = 0; trainType < 5; trainType++)
  {
    calculateTrainingValueSingle(trainType);
  }
  calculateSS();
}
void Game::addStatus(int idx, int value)
{
  fiveStatus[idx] += value;
  if (fiveStatus[idx] > fiveStatusLimit[idx])
    fiveStatus[idx] = fiveStatusLimit[idx];
  if (fiveStatus[idx] < 1)
    fiveStatus[idx] = 1;
}
void Game::addVital(int value)
{
  vital += value;
  if (vital > maxVital)
    vital = maxVital;
  if (vital < 0)
    vital = 0;
}
void Game::addMotivation(int value)
{
  if (value < 0)
  {
    if (isPositiveThinking)
      isPositiveThinking = false;
    else
    {
      motivation += value;
      if (motivation < 1)
        motivation = 1;
    }
  }
  else
  {
    motivation += value;
    if (motivation > 5)
      motivation = 5;
  }
}
void Game::addJiBan(int idx, int value)
{
  auto& p = persons[idx];
  if (p.personType == 1 || p.personType == 2)
  {
    if (isAiJiao)value += 2;
  }
  else if (p.personType == 4 || p.personType == 5 || p.personType == 6)
  {

  }
  else
    value = 0;
  p.friendship += value;
  if (p.friendship > 100)p.friendship = 100;
}
void Game::addTrainingLevelCount(int item, int value)
{
  assert(item >= 0 && item < 5 && "addTrainingLevelCount���Ϸ�ѵ��");
  trainLevelCount[item] += value;
  if (trainLevelCount[item] > 48)trainLevelCount[item] = 48;
}
void Game::addAllStatus(int value)
{
  for (int i = 0; i < 5; i++)addStatus(i, value);
}
int Game::calculateFailureRate(int trainType) const
{
  //������ϵ�ѵ��ʧ���ʣ����κ��� A*x^2 + B*x + C + 0.5 * trainLevel
  //���Ӧ����2%����
  static const double A = 0.0245;
  static const double B[5] = { -3.77,-3.74,-3.76,-3.81333,-3.286 };
  static const double C[5] = { 130,127,129,133.5,80.2 };

  double f = A * vital * vital + B[trainType] * vital + C[trainType] + 0.5 * getTrainingLevel(trainType);
  int fr = round(f);
  if (vital > 60)fr = 0;//�����Ƕ��κ�������������103ʱ�������fr����0��������Ҫ�ֶ�����
  if (fr < 0)fr = 0;
  if (fr > 99)fr = 99;//����ϰ���֣�ʧ�������99%
  //fr += failureRateBias;
  //if (fr < 0)fr = 0;
  //if (fr > 100)fr = 100;
  return fr;
}
void Game::runRace(int basicFiveStatusBonus, int basicPtBonus)
{
  double raceMultiply = 1 + 0.01 * saihou;
  int fiveStatusBonus = floor(raceMultiply * basicFiveStatusBonus);
  int ptBonus = floor(raceMultiply * basicPtBonus);
  addAllStatus(fiveStatusBonus);
  skillPt += basicPtBonus;
}

void Game::addStatusZuoyue(int idx, int value)
{
  value = int(value * larc_zuoyueStatusBonus);
  if (idx == 5)skillPt += value;
  else addStatus(idx, value);
}

void Game::addVitalZuoyue(int value)
{
  value = int(value * larc_zuoyueVitalBonus);
  addVital(value);
}


void Game::handleFriendOutgoing()
{
  assert(larc_zuoyueOutgoingUnlocked && larc_zuoyueOutgoingUsed < 5);
  if (larc_zuoyueOutgoingUsed == 0)
  {
    addVitalZuoyue(30);
    addMotivation(1);
    addStatus(3, 5);
    addJiBan(17, 5);
  }
  else if (larc_zuoyueOutgoingUsed == 1)
  {
    addVitalZuoyue(25);
    addMotivation(1);
    addStatus(2, 5);
    addStatus(3, 5);
    addJiBan(17, 5);
  }
  else if (larc_zuoyueOutgoingUsed == 2)
  {
    addVitalZuoyue(35);
    addMotivation(1);
    addStatus(3, 15);
    isPositiveThinking = true;
    addJiBan(17, 5);
  }
  else if (larc_zuoyueOutgoingUsed == 3)
  {
    addVitalZuoyue(25);
    addStatus(3, 20);
    addJiBan(17, 5);
  }
  else if (larc_zuoyueOutgoingUsed == 4)//��Ϊ��ɹ��ͳɹ���ȡ��ƽ��
  {
    addVitalZuoyue(37);
    addStatus(3, 7);
    addMotivation(1);
    addJiBan(17, 5);
  }
  else assert(false && "δ֪�ĳ���");
  larc_zuoyueOutgoingUsed += 1;
}
void Game::calculateTrainingValueSingle(int trainType)
{
  //�������ˣ��������������Լ�ֵ
  //failRate[trainType] = 

  double failRateBasic = calculateFailureRate(trainType);//�������ʧ����

  int personCount = 0;//��+npc����ͷ�������������³��ͼ���
  vector<CardTrainingEffect> effects;

  double vitalCostDrop = 1;

  for (int i = 0; i < 5; i++)
  {
    int p = personDistribution[trainType][i];
    if (p < 0)break;//û��
    int personType = persons[p].personType;
    if (personType == 1 || personType == 2)//��
    {
      personCount += 1; 
      effects.push_back(cardParam[persons[p].cardIdInGame].getCardEffect(*this, trainType, persons[p].friendship, persons[p].cardRecord));

    }
    else if (personType == 3)//npc
    {
      personCount += 1;
    }
      
  }

  trainShiningNum[trainType] = 0;
  for (int i = 0; i < effects.size(); ++i) {
    failRateBasic *= (1 - 0.01 * effects[i].failRateDrop);//ʧ�����½�
    vitalCostDrop *= (1 - 0.01 * effects[i].vitalCostDrop);//���������½�
    if (effects[i].youQing > 0)trainShiningNum[trainType] += 1;//ͳ�Ʋ�Ȧ��
  }

  int fr= round(failRateBasic);
  fr += failureRateBias;
  if (fr < 0)fr = 0;
  if (fr > 100)fr = 100;
  failRate[trainType] = fr;

  if (larc_isAbroad)
    larc_shixingPtGainAbroad[trainType] = personCount * 20 + trainShiningNum[trainType] * 20 + (trainType == 4 ? 30 : 50);
  else
    larc_shixingPtGainAbroad[trainType] = 0;

  //�����²���ֵ
  int cardNum = effects.size();
  //1.��ͷ�����ʣ�npcҲ��
  double cardNumMultiplying = 1 + 0.05 * personCount;
  //2.��Ȧ(����ѵ��)���ʣ�ע���Ƿ������Ѿ���getCardEffect�￼�ǹ���
  double youQingMultiplying = 1;
  for (int i = 0; i < cardNum; i++)
    youQingMultiplying *= (1 + 0.01 * effects[i].youQing);
  //3.ѵ������
  double xunLianBonusTotal = 0;
  for (int i = 0; i < cardNum; i++)
    xunLianBonusTotal += effects[i].xunLian;
  double xunLianMultiplying = 1 + 0.01 * xunLianBonusTotal;
  //4.�ɾ�����
  double ganJingBasic = 0.1 * (motivation - 3);
  double ganJingBonusTotal = 0;
  for (int i = 0; i < cardNum; i++)
    ganJingBonusTotal += effects[i].ganJing;
  double ganJingMultiplying = 1 + ganJingBasic * (1 + 0.01 * ganJingBonusTotal);

  //�벻ͬ�����޹ص��ܱ���
  double totalMultiplying = cardNumMultiplying * youQingMultiplying * xunLianMultiplying * ganJingMultiplying;

  //5.����ֵ
  int trainLv = getTrainingLevel(trainType);
  int basicValue[6] = { 0,0,0,0,0,0 };
  for (int i = 0; i < cardNum; i++)
  {
    for (int j = 0; j < 6; j++)
      basicValue[j] += effects[i].bonus[j];
  }
  for (int j = 0; j < 6; j++)
  {
    int b = GameConstants::TrainingBasicValue[trainType][trainLv][j];
    if(b>0)//��������
      basicValue[j] += b;
    else
      basicValue[j] = 0;
  }

  //6.�ɳ���
  double growthRates[6] = { 1,1,1,1,1,1 };
  for (int j = 0; j < 5; j++)
    growthRates[j] = 1.0 + 0.01 * fiveStatusBonus[j];
    //growthRates[j] = 1.0 + 0.01 * GameDatabase::AllUmas[umaId].fiveStatusBonus[j];

  //�²�����ֵ
  int totalValueLower[6];
  for (int j = 0; j < 6; j++)
  {
    int v = int(totalMultiplying * basicValue[j] * growthRates[j]);//����ȡ����
    if (v > 100)v = 100;
    totalValueLower[j] = v;
  }
  
  //7.�ϲ�
  double upperRate = 1;
  upperRate += 0.01 * larc_trainBonus;//�ڴ��ȼӳ�
  if (larc_isAbroad && larc_levels[GameConstants::UpdateId50pEachTrain[trainType]] >= 3)
    upperRate += 0.5;//����+50%
  if (larc_levels[8] >= 1)//�����ڶ���������ѵ��+5%
    upperRate += 0.05;
  if (larc_levels[7] >= 3 && trainShiningNum[trainType] > 0)//����+20%
    upperRate *= 1.2;


  for (int j = 0; j < 6; j++)
  {
    int lower = totalValueLower[j];
    if (lower == 0)continue;
    int total = int(double(lower + larc_staticBonus[j]) * upperRate);
    int upper = total - lower;
    if (upper > 100)upper = 100;
    trainValue[trainType][j] = lower + upper;
  } 
    


  //����
  double vitalChange=GameConstants::TrainingBasicValue[trainType][trainLv][6];
  for (int i = 0; i < cardNum; i++)
    vitalChange += effects[i].vitalBonus;
  if (vitalChange < 0)//��������ʱ������Ƿ�������-20%
  {
    vitalChange *= vitalCostDrop;
    if (larc_isAbroad && larc_levels[6] >= 3)//����-20%
    vitalChange *= 0.8;
  }


  trainValue[trainType][6] = round(vitalChange);
}
void Game::calculateSS()
{
  for (int i = 0; i < 5; i++)larc_ssValue[i] = 0;
  int linkn = 0;
  for (int i = 0; i < larc_ssPersonsCount; i++)
  {
    if (persons[larc_ssPersons[i]].larc_isLinkCard)
      linkn += 1;
  }
  int p = larc_ssPersonsCount;

  int totalValue =
    turn < 40 ?
    5 * p + (4 * p + 2 * linkn) * (0.8 + larc_supportPtAll * 6e-6) :
    5 * p + (5 * p + 2 * linkn) * (1.0 + larc_supportPtAll * 6e-6);//�ճ�����ϵĹ�ʽ�����ͦ��ĵ�Ӧ�ò�̫Ӱ�����
  if (larc_isSSS)totalValue += 75;

  totalValue -= (4 * p + 2 * linkn);//4 * p + 2 * linkn�ǰ�����ͷ���Է���
  //ʣ�µ�ƽ������
  int div5 = totalValue / 5;
  for (int i = 0; i < 5; i++)larc_ssValue[i] = div5;
  for (int i = 0; i < totalValue - div5 * 5; i++)larc_ssValue[i] += 1;

  //��ͷ����
  for (int i = 0; i < larc_ssPersonsCount; i++)
  {
    auto& p = persons[larc_ssPersons[i]];
    larc_ssValue[p.larc_statusType] += (p.larc_isLinkCard ? 6 : 4);
  }
}
bool Game::applyTraining(std::mt19937_64& rand, int chosenTrain, bool useVenus, int chosenSpiritColor, int chosenOutgoing, int forceThreeChoicesEvent)
{
  assert(stageInTurn == 1);
  stageInTurn = 2;
  if (isRacing)
  {
    if (useVenus)
    {
      if (venusAvailableWisdom != 0)
        activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
      else
        return false;
    }
    if (turn != TOTAL_TURN - 1)//����GrandMaster
      runRace(GameConstants::NormalRaceFiveStatusBonus, GameConstants::NormalRacePtBonus);
    addJiBan(6, 4);//���³��+4

    int newSpirit = (rand() % 6 + 1) + (rand() % 3) * 8;//�����������Ƭ
    addSpirit(rand, newSpirit);
    addSpirit(rand, newSpirit);
    return true;//GUR,WBC,SWBC,GrandMaster�ĳ�������checkEventAfterTrain()�ﴦ�������������
  }
  if (chosenTrain == 5)//��Ϣ
  {
    if (useVenus)
    {
      if (venusAvailableWisdom != 0)
        activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
      else
        return false;
    }
    if (isXiaHeSu())
    {
      addVital(40);
      addMotivation(1);
    }
    else
    {
      int r = rand() % 100;
      if (r < 25)
        addVital(70);
      else if (r < 82)
        addVital(50);
      else
        addVital(30);
    }

    addSpirit(rand, spiritDistribution[chosenTrain]);
  }
  else if (chosenTrain == 7)//����
  {
    if (turn <= 12 || turn >= 72)
    {
      printEvents("ǰ13�غϺ����6�غ��޷�����");
      return false;
    }
    if (useVenus)
    {
      if (venusAvailableWisdom != 0)
        activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
      else
        return false;
    }
    runRace(2, 40);//���ԵĽ���
    addSpirit(rand, spiritDistribution[chosenTrain]);
  }
  else if (chosenTrain == 6)//���
  {
    if (isXiaHeSu())
    {
      printEvents("�ĺ���ֻ����Ϣ���������");
      return false;
    }
    if(!isOutgoingLegal(chosenOutgoing))
    {
      printEvents("���Ϸ������");
      return false;
    }
    assert(!isXiaHeSu() && "�ĺ��޲��������");
    assert(isOutgoingLegal(chosenOutgoing) && "���Ϸ������");
    if (useVenus)
    {
      if (venusAvailableWisdom != 0)
        activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
      else
        return false;
    }
    if (chosenOutgoing < 5)//�������
      handleVenusOutgoing(chosenOutgoing);
    else if (chosenOutgoing == 6)//��ͨ���
    {
      //���ò�����ˣ���50%��2���飬50%��1����10����
      if (rand() % 2)
        addMotivation(2);
      else
      {
        addMotivation(1);
        addVital(10);
      }
    }

    addSpirit(rand, spiritDistribution[chosenTrain]);
  }
  else if (chosenTrain <= 4 && chosenTrain >= 0)//����ѵ��
  {
    if (useVenus)
    {
      if (venusAvailableWisdom != 0)
        activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
      else
        return false;
    }
    if (rand() % 100 < failRate[chosenTrain])//ѵ��ʧ��
    {
      if (failRate[chosenTrain] >= 20 && (rand() % 100 < failRate[chosenTrain]))//ѵ����ʧ�ܣ�������Ϲ�µ�
      {
        printEvents("ѵ����ʧ�ܣ�");
        addStatus(chosenTrain, -10);
        if (fiveStatus[chosenTrain] > 1200)
          addStatus(chosenTrain, -10);//��Ϸ��1200���Ͽ����Բ��۰룬�ڴ�ģ�������Ӧ1200���Ϸ���
        //�����2��10�������ĳ�ȫ����-4���������
        for (int i = 0; i < 5; i++)
        {
          addStatus(i, -4);
          if (fiveStatus[i] > 1200)
            addStatus(i, -4);//��Ϸ��1200���Ͽ����Բ��۰룬�ڴ�ģ�������Ӧ1200���Ϸ���
        }
        addMotivation(-3);
        addVital(10);
      }
      else//Сʧ��
      {
        printEvents("ѵ��Сʧ�ܣ�");
        addStatus(chosenTrain, -5);
        if (fiveStatus[chosenTrain] > 1200)
          addStatus(chosenTrain, -5);//��Ϸ��1200���Ͽ����Բ��۰룬�ڴ�ģ�������Ӧ1200���Ϸ���
        addMotivation(-1);
      }
    }
    else
    {
      //�ȼ���ѵ��ֵ
      for (int i = 0; i < 5; i++)
        addStatus(i, trainValue[chosenTrain][i]);
      skillPt += trainValue[chosenTrain][5];
      addVital(trainValue[chosenTrain][6]);

      //�
      for (int i = 0; i < 8; i++)
      {
        if (cardDistribution[chosenTrain][i])
        {
          assert(cardData[0]->cardType == 5 && "���ſ����ڵ�һ��λ��");
          if (i == 0) //���ŵ�һ��+4�
            addJiBan(i, 4);
          else
            addJiBan(i, 7);
          if (i == 6)skillPt += 2;//���³�
          if (i >= 6)continue;//���³��ͼ���
        }
      }

      //���(hint)
      vector<int> hintCards;
      for (int i = 0; i < 6; i++)
      {
        if (cardDistribution[chosenTrain][i] && cardHint[i])
        {
          hintCards.push_back(i);
        }
      }

      auto applyHint= [this](int i)  {
        addJiBan(i, 5);
        auto& hintBonus = cardData[i]->hintBonus;
        for (int i = 0; i < 5; i++)
          addStatus(i, hintBonus[i]);
        skillPt += hintBonus[5];
      };

      
      //����
      if (venusIsWisdomActive && venusAvailableWisdom == 2)
      {
        auto blueVenusBonus = calculateBlueVenusBonus(chosenTrain);
        for (int i = 0; i < 5; i++)
          addStatus(i, blueVenusBonus[i]);
        skillPt += blueVenusBonus[5];

        for (auto i = 0; i < hintCards.size(); i++)
          applyHint(hintCards[i]);
      }
      else//һ��ֻ��һ�ſ��ĺ�������Ч�����ǿ���
      {
        if (hintCards.size() != 0)
        {
          int hintCard = hintCards[rand() % hintCards.size()];
          applyHint(hintCard);
        }
      }

      //����Ƭ
      addSpirit(rand, spiritDistribution[chosenTrain]);

      //�����Ů�����ڵ�ѵ��
      assert(cardData[0]->cardType == 5 && "���ſ����ڵ�һ��λ��");
      if (cardDistribution[chosenTrain][0])
      {
        if (!venusCardFirstClick)//��һ�ε�
        {
          printEvents("��һ�ε�Ů��");
          venusCardFirstClick = true;
          addAllStatus(3);
          addVital(10);
          addJiBan(0, 10);
        }
        else
        {
          //��ѡһ�¼����ʣ���ʱ�²�Ϊ40%*(1+��Ů��ȼ��ӳ�)
          bool activateThreeChoicesEvent = randBool(rand,getThreeChoicesEventProb(useVenus));
          if (forceThreeChoicesEvent == 1)
            activateThreeChoicesEvent = true;
          else if (forceThreeChoicesEvent == -1)
              activateThreeChoicesEvent = false;
          if (activateThreeChoicesEvent)
            handleVenusThreeChoicesEvent(rand, chosenSpiritColor);
        }

      }

      //ѵ���ȼ�����+2
      if(!isXiaHeSu())
        addTrainingLevelCount(chosenTrain, 2);
    }
  }
  else
  {
    printEvents("δ֪��ѵ����Ŀ");
    return false;
  }
  return true;
}

float Game::getSkillScore() const
{
  float scorePtRate = isQieZhe ? GameConstants::ScorePtRateQieZhe : GameConstants::ScorePtRate;
  return scorePtRate * skillPt;
}

int Game::finalScore() const
{
  int total = 0;
  for (int i = 0; i < 5; i++)
    total += GameConstants::FiveStatusFinalScore[min(fiveStatus[i],fiveStatusLimit[i])];
  
  total += getSkillScore();
  return total;
}

bool Game::isEnd() const
{
  return turn >= TOTAL_TURN;
}

int Game::getTrainingLevel(int item) const
{
  int level ;
  if (venusIsWisdomActive && venusAvailableWisdom == 1)//��Ů��
    level = 5;
  else if(isXiaHeSu())
    level = 4;
  else
  {
    assert(trainLevelCount[item] <= 48, "ѵ���ȼ���������48");
    level = trainLevelCount[item] / 12;
    if (level > 4)level = 4;
  }
  return level;
}

double Game::sssProb(int ssWinSinceLastSSS) const
{
  return ssWinSinceLastSSS >= 8 ? 1.0 : 0.12 + 0.056 * ssWinSinceLastSSS;
}

bool Game::isOutgoingLegal(int chosenOutgoing) const
{
  assert(chosenOutgoing >= 0 && chosenOutgoing <= 5 && "δ֪�����");
  if (isXiaHeSu())return false;//�ĺ��޲��������
  if (chosenOutgoing == 5)return true;//��ͨ���
  //ʣ�µ����������
  if (!venusCardUnlockOutgoing)return false;
  if (venusCardOutgoingUsed[chosenOutgoing])return false;
  if (chosenOutgoing == 0 || chosenOutgoing == 1 || chosenOutgoing == 2)return true;
  else if (chosenOutgoing == 3)
    return venusCardOutgoingUsed[0] && venusCardOutgoingUsed[1] && venusCardOutgoingUsed[2];
  else if (chosenOutgoing == 4)
    return venusCardOutgoingUsed[3];
  else return false;//δ֪



}

bool Game::isXiaHeSu() const
{
  return (turn >= 36 && turn <= 39) || (turn >= 60 && turn <= 63);
}

double Game::getThreeChoicesEventProb(bool useVenusIfFull) const
{
  if (!venusCardFirstClick)return 0.0;
  if (venusCardIsQingRe)return 1.0;
  if (venusAvailableWisdom == 2 && useVenusIfFull)return 1.0;
  //��ѡһ�¼����ʣ���ʱ�²�Ϊ40%*(1+��Ů��ȼ��ӳ�)
  return GameConstants::VenusThreeChoicesEventProb * (1 + 0.01 * GameConstants::BlueVenusLevelHintProbBonus[venusLevelBlue]);

}



void Game::checkEventAfterTrain(std::mt19937_64& rand)
{
  assert(stageInTurn == 2);
  stageInTurn = 0;
  //Ů��᲻������
  if (venusCardFirstClick && (!venusCardUnlockOutgoing))
  {
    if (randBool(rand, GameConstants::VenusUnlockOutgoingProbEveryTurn))//����
    {
      printEvents("Ů�����������");
      //���Ƥ�ѧ����Ů��
      venusCardUnlockOutgoing = true;
      venusCardIsQingRe = true;
      addAllStatus(6);
      skillPt += 12;
      assert(cardData[0]->cardType == 5 && "���ſ����ڵ�һ��λ��");
      addJiBan(0, 5);
    }
  }

  //Ů�������Ƿ����
  if(venusCardIsQingRe)
  {
    if (randBool(rand, GameConstants::VenusQingReDeactivateProb[venusCardQingReContinuousTurns]))
    {
      printEvents("Ů�����Ƚ���");
      venusCardIsQingRe = false;
      venusCardQingReContinuousTurns = 0;
    }
    else venusCardQingReContinuousTurns++;
  }

  //������̶ֹ��¼�
  if (turn == 23)//��һ�����
  {
    //GUR
    int raceFiveStatusBonus = 10;
    int racePtBonus = 50;
    if (venusLevelYellow >= 1)
      raceFiveStatusBonus += 2;//���1������+10���ĳ�ƽ��
    if (venusLevelRed >= 1)
      racePtBonus += 20;
    runRace(raceFiveStatusBonus, racePtBonus);
    if (venusLevelBlue >= 1)
      skillPt += 10;//���1���ܷ�����

    //��ѵ���ȼ�
    for (int i = 0; i < 5; i++)
      addTrainingLevelCount(i, 8);

    //�����ѡһ�¼���ѡ���Ի�������
    //Ϊ�˼򻯣�ֱ����Ϊȫ����+5
    if (maxVital - vital >= 50)
      addVital(20);
    else 
      addAllStatus(5);

    //Ů�񤵤ޡ��ҤȤ䤹��
    if (venusCardUnlockOutgoing)
    {
      addVital(19);
      skillPt += 36;
      skillPt += 50;//���ܵ�Ч
      assert(cardData[0]->cardType == 5 && "���ſ����ڵ�һ��λ��");
      addJiBan(0, 5);
    }

    printEvents("GUR����");

  }
  else if (turn == 29)//�ڶ���̳�
  {
    for (int i = 0; i < 5; i++)
      addStatus(i, zhongMaBlueCount[i] * 6); //�����ӵ���ֵ
    for (int i = 0; i < 5; i++)
      addStatus(i, zhongMaExtraBonus[i]); //�籾���ӵ���ֵ
    skillPt += zhongMaExtraBonus[5];

    printEvents("�ڶ���̳�");
  }
  else if (turn == 47)//�ڶ������
  {
    //WBC
    int raceFiveStatusBonus = 15;
    int racePtBonus = 60;
    if (venusLevelYellow >= 2)
      raceFiveStatusBonus += 4;//���2������+10���ĳ�ƽ��
    if (venusLevelRed >= 2)
      racePtBonus += 30;
    runRace(raceFiveStatusBonus, racePtBonus);
    if (venusLevelBlue >= 2)
      skillPt += 20;//���2���ܷ�����

    //��ѵ���ȼ�
    for (int i = 0; i < 5; i++)
      addTrainingLevelCount(i, 8);

    //�����ѡһ�¼���ѡ���Ի�������
    if (maxVital - vital >= 50)
      addVital(30);
    else
      addAllStatus(8);
    printEvents("WBC����");
  }
  else if (turn == 48)//�齱
  {
    int rd = rand() % 100;
    if (rd < 16)//��Ȫ��һ�Ƚ�
    {
      addVital(30);
      addAllStatus(10);
      addMotivation(2);

      printEvents("�齱�����������Ȫ/һ�Ƚ�");
    }
    else if (rd < 16 + 27)//���Ƚ�
    {
      addVital(20);
      addAllStatus(5);
      addMotivation(1);
      printEvents("�齱��������˶��Ƚ�");
    }
    else if (rd < 16 + 27 + 46)//���Ƚ�
    {
      addVital(20);
      printEvents("�齱������������Ƚ�");
    }
    else//��ֽ
    {
      addMotivation(-1);
      printEvents("�齱��������˲�ֽ");
    }
  }
  else if (turn == 49)//������
  {
    skillPt += 170 / GameConstants::ScorePtRate;//����ֱ�ӵȼ۳�pt
    printEvents("���еȼ�+1");
  }
  else if (turn == 53)//������̳�&���³�������
  {
    for (int i = 0; i < 5; i++)
      addStatus(i, zhongMaBlueCount[i] * 6); //�����ӵ���ֵ
    for (int i = 0; i < 5; i++)
      addStatus(i, zhongMaExtraBonus[i]); //�籾���ӵ���ֵ
    skillPt += zhongMaExtraBonus[5];
    printEvents("������̳�");
    if (cardJiBan[6] >= 60)//����������
    {
      addMotivation(1);
      skillPt += 170 / GameConstants::ScorePtRate;//����ֱ�ӵȼ۳�pt
      printEvents("���еȼ�+1");
    }
    else
    {
      addVital(-5);
      skillPt += 25;
    }
  }
  else if (turn == 70)//������
  {
    skillPt += 170 / GameConstants::ScorePtRate;//����ֱ�ӵȼ۳�pt
    printEvents("���еȼ�+1");
  }
  else if (turn == 71)//���������
  {
    //SWBC
    int raceFiveStatusBonus = 20;
    int racePtBonus = 70;
    if (venusLevelYellow >= 3)
      raceFiveStatusBonus += 6;//���3������+10���ĳ�ƽ��
    if (venusLevelRed >= 3)
      racePtBonus += 45;
    runRace(raceFiveStatusBonus, racePtBonus);
    if (venusLevelBlue >= 3)
      skillPt += 30;//���3���ܷ�����

    //��ѵ���ȼ�
    for (int i = 0; i < 5; i++)
      addTrainingLevelCount(i, 8);

    printEvents("SWBC����");
  }
  else if (turn == 76)//���һսǰ����ѡһ
  {
    int totalLevel = venusLevelRed + venusLevelBlue + venusLevelYellow;
    int maxLevel = max(venusLevelRed, max(venusLevelBlue, venusLevelYellow));
    if (maxLevel >= 4)
    {
      addAllStatus(10);
      skillPt += 50;//���ܵ�Ч
      if (maxLevel >= 5)
        skillPt += 20;//�����ۿ�
      if (totalLevel >= 12)
        skillPt += 40;//�����ۿ�
    }
  }
  else if (turn == 77)//���һս
  {
    //GrandMasters
    int raceFiveStatusBonus = 20;
    int racePtBonus = 80;
    runRace(raceFiveStatusBonus, racePtBonus);

    //Ů���¼�
    if (venusCardOutgoingUsed[4])//����������
    {
      addAllStatus(12);
      skillPt += 12;
    }
    else
    {
      addAllStatus(8);
    }

    //����
    if (cardJiBan[7] >= 100)
    {
      addAllStatus(5);
      skillPt += 20;
    }
    else if (cardJiBan[7] >= 80)
    {
      addAllStatus(3);
      skillPt += 10;
    }
    else
    {
      skillPt += 5;
    }

    //�������߰���
    addAllStatus(25);
    skillPt += 80;

  }

  //ģ���������¼�

  //֧Ԯ�������¼��������һ������5�
  if (randBool(rand, 0.3))
  {
    int card = rand() % 6;
    addJiBan(card, 5);

    printEvents("ģ������¼���" + GameDatabase::AllCards[cardId[card]].cardName + " ���+5");
  }

  //ģ�����߰���������¼�
  if (turn < 72)
  {
    addAllStatus(1);
    printEvents("ģ������¼���ȫ����+1");
  }

  //������
  if (randBool(rand, 0.1))
  {
    addVital(10);
    printEvents("ģ������¼�������+10");
  }

  //������
  if (randBool(rand, 0.02))
  {
    addMotivation(1);
    printEvents("ģ������¼�������+1");
  }

  //������
  if (randBool(rand, 0.04))
  {
    addMotivation(-1);
    printEvents("ģ������¼���\033[0m\033[33m����-1\033[0m\033[32m");
  }
  
  //�����Ů�������Ƭ
  if (venusIsWisdomActive)
  {
    venusIsWisdomActive = false;
    venusAvailableWisdom = 0;
    for (int i = 0; i < 8; i++)
      venusSpiritsBottom[i] = 0;
    for (int i = 0; i < 6; i++)
      venusSpiritsUpper[i] = 0;
    for (int i = 0; i < 6; i++)
      spiritBonus[i] = 0;
  }

  //�غ���+1
  turn++;
  if (turn < TOTAL_TURN)
  {
    isRacing = umaData->races[turn] & TURN_RACE;
    //isRacing = GameDatabase::AllUmas[umaId].races[turn] & TURN_RACE;
  }
  else
  {
    printEvents("���ɽ���!");
    printEvents("��ĵ÷��ǣ�" + to_string(finalScore()));
  }

}

void Game::applyTrainingAndNextTurn(std::mt19937_64& rand, int chosenTrain, bool useVenus, int chosenSpiritColor, int chosenOutgoing, int forceThreeChoicesEvent)
{
  assert(stageInTurn == 1);
  assert(turn < TOTAL_TURN && "Game::applyTrainingAndNextTurn��Ϸ�ѽ���");
  bool suc = applyTraining(rand, chosenTrain, useVenus, chosenSpiritColor, chosenOutgoing, forceThreeChoicesEvent);
  assert(suc && "Game::applyTrainingAndNextTurnѡ���˲��Ϸ���ѵ��");

  checkEventAfterTrain(rand);
  if (isEnd()) return;

  if (isRacing)
  {
    if (venusAvailableWisdom == 0)//�����غ����޷���Ů���ٽ���һ���غ�
    {
      randomDistributeCards(rand);//��stageInTurn�ĳ�1
      bool useVenus = false;
      applyTrainingAndNextTurn(rand, -1, useVenus, -1, -1, -1);
    }
  }

  randomDistributeCards(rand);

}
