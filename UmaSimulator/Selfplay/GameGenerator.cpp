#include "GameGenerator.h"
#include "../Search/SearchParam.h"
using namespace std;

GameGenerator::GameGenerator(SelfplayParam param, Model* model) :param(param)
{
  evaluator = Evaluator(model, param.batchsize);
  std::random_device rd;
  rand = std::mt19937_64(rd());
  gameBuf.resize(param.batchsize);
  nextGamePointer = param.batchsize;
}

Game GameGenerator::randomOpening()
{
  std::exponential_distribution<double> expDistr(1.0);
  std::normal_distribution<double> normDistr(0.0, 1.0);
  std::uniform_real_distribution<double> uniDistr(0.0, 1.0);

  Game game;
  int umaId = 101101;//���Ϸ�
  int umaStars = 5;
  int cards[6] = { 301604,301724,301614,301304,300114,300374 };//���ˣ�������������ӥ������˹�������񣬸��ʵ�
  int zhongmaBlue[5] = { 18,0,0,0,0 };
  int zhongmaBonus[6] = { 10,10,30,0,10,70 };

  if (param.cardRandType == 0)
  {
  }
  else if (param.cardRandType == 1)
  {
    //�����
    umaId = -1;
    while (!GameDatabase::AllUmas.count(umaId))
      umaId = 100000 + (rand() % 200) * 100 + rand() % 2 + 1;

    //������˿��ȼ�
    if (rand() % 2 == 0)
      cards[0] = 301600 + rand() % 5;
    else
      cards[0] = 301604;

    //�����5�ű��SSR��
    for (int c = 1; c < 6; c++)
    {
      int cardId = -1;
      while (true)
      {
        cardId = 30000 + rand() % 300;
        int cardIdLv50 = cardId * 10 + 4;
        if (!GameDatabase::AllCards.count(cardIdLv50))
        {
          continue;
        }
        if (GameDatabase::AllCards[cardIdLv50].cardType >= 5)
        {
          continue;
        }
        //����Ƿ����ظ���larc link
        if (GameDatabase::AllCards[cardIdLv50].larc_isLink)
        {
          bool repeatedLink = false;
          for (int i = 0; i < c; i++)
          {
            if (GameDatabase::AllCards[cards[i]].larc_linkSpecialEffect == GameDatabase::AllCards[cardIdLv50].larc_linkSpecialEffect)
            {
              repeatedLink = true;
              break;
            }
          }
          if (repeatedLink)
            continue;
        }
        break;
      }
      if (rand() % 8 == 0)
        cardId = cardId * 10 + rand() % 5;
      else
        cardId = cardId * 10 + 4;
      cards[c] = cardId;
    }

    //��������Ӻ;籾����
    {
      vector<int> blueStarProb = { 1,1,2,15 };
      vector<int> blueTypeProb = { 15,1,3,1,1 };
      std::discrete_distribution<> blueStarProbDis(blueStarProb.begin(), blueStarProb.end());
      std::discrete_distribution<> blueTypeProbDis(blueTypeProb.begin(), blueTypeProb.end());
      for (int i = 0; i < 5; i++)
        zhongmaBlue[i] = 0;
      for (int i = 0; i < 6; i++)
      {
        zhongmaBlue[blueTypeProbDis(rand)] += blueStarProbDis(rand);
      }

      vector<int> scenarioStarProb = { 1,2,5,15 };
      vector<int> scenarioTypeProb = { 1,1,1 };//�ഺ�� ��ʦ�� ������
      std::discrete_distribution<> scenarioStarProbDis(scenarioStarProb.begin(), scenarioStarProb.end());
      std::discrete_distribution<> scenarioTypeProbDis(scenarioTypeProb.begin(), scenarioTypeProb.end());
      for (int i = 0; i < 5; i++)
        zhongmaBonus[i] = 0;
      for (int i = 0; i < 6; i++)
      {
        int type = scenarioTypeProbDis(rand);
        int star = scenarioStarProbDis(rand);
        int bonus = star == 3 ? 8 :
          star == 2 ? 4 :
          star == 1 ? 2 :
          0;
        if (type == 0)
        {
          zhongmaBonus[2] += bonus;
          zhongmaBonus[4] += bonus;
        }
        else if (type == 1)
        {
          zhongmaBonus[0] += bonus;
          zhongmaBonus[2] += bonus;
        }
        else if (type == 2)
        {
          zhongmaBonus[1] += bonus;
          zhongmaBonus[2] += bonus;
        }
        else assert(false);
      }
      zhongmaBonus[5] = expDistr(rand) * 70.0;
      if (zhongmaBonus[5] > 300)zhongmaBonus[5] = 300;
    }
  }





  
  game.newGame(rand, false, umaId, umaStars, cards, zhongmaBlue, zhongmaBonus);

  //�����Լ����
  int r = rand() % 100;
  if (r < 10)//���������
  {
    double mean = -expDistr(rand) * 20;
    for (int i = 0; i < 5; i++)
      game.addStatus(i, int(expDistr(rand) * mean));
    game.skillPt += int(2 * expDistr(rand) * mean);
  }
  else if (r < 80)//��������ԣ�ģ�����
  {
    double mean = expDistr(rand) * 30;
    for (int i = 0; i < 5; i++)
      game.addStatus(i, int(expDistr(rand) * mean));
    game.skillPt += int(2 * expDistr(rand) * mean);
  }
  if (game.skillPt < 0)game.skillPt = 0;

  //��Щdebuff���Բ���
  r = rand() % 100;
  if (r < 30)//
  {
    game.larc_allowedDebuffsFirstLarc[4] = true;
  }
  else if (r < 60)//
  {
    game.larc_allowedDebuffsFirstLarc[6] = true;
  }
  else if (r < 90)//
  {
    game.larc_allowedDebuffsFirstLarc[4] = true;
    game.larc_allowedDebuffsFirstLarc[6] = true;
  }


  if (rand() % 8 == 0)
    game.isQieZhe = true;
  if (rand() % 8 == 0)
    game.isAiJiao = true;

  game.motivation = rand() % 5 + 1;

  for (int i = 0; i < 6; i++)
  {
    int cardPerson = i < game.normalCardCount ? i : 17;
    if (rand() % 2)
    {
      int delta = int(expDistr(rand) * 5);
      game.addJiBan(cardPerson, delta);
    }
  }
  for (int i = 0; i < 5; i++)
  {
    if (rand() % 2)
    {
      int delta = int(expDistr(rand) * 1);
      game.addTrainingLevelCount(i, delta);
    }
  }

  if (rand() % 2)
  {
    int delta = int(expDistr(rand) * 4);
    game.maxVital += delta;
  }

  return game;
}

Game GameGenerator::randomizeBeforeOutput(const Game& game0)
{
  Game game = game0;
  std::exponential_distribution<double> expDistr(1.0);

  //�����Լ����
  int type0 = rand() % 100;
  if (type0 < 30)//���������
  {
    double mean = -expDistr(rand) * 30;
    for (int i = 0; i < 5; i++)
      game.addStatus(i, int(expDistr(rand) * mean));
    game.skillPt += int(2 * expDistr(rand) * mean);
  }
  else if (type0 < 80)//��������ԣ�ģ�����
  {
    double mean = expDistr(rand) * 50;
    for (int i = 0; i < 5; i++)
      game.addStatus(i, int(expDistr(rand) * mean));
    game.skillPt += int(2 * expDistr(rand) * mean);
  }
  if (game.skillPt < 0)game.skillPt = 0;

  if (rand() % 2 == 0)
  {
    int delta= expDistr(rand) * 10;
    delta *= 10;
    if (rand() % 2 == 0)
      delta = -delta;

    game.larc_shixingPt += delta;
    if (game.larc_shixingPt < 0)
      game.larc_shixingPt = 0;
  }

  return game;
}

void GameGenerator::newGameBatch()
{
  evaluator.gameInput.resize(param.batchsize);
  for (int i = 0; i < param.batchsize; i++)
  {
    evaluator.gameInput[i] = randomOpening();
  }
  //�������һЩ�غ�
  //std::normal_distribution<double> norm(0.0, 1.0);
  int maxTurn = TOTAL_TURN - 8;//7�������غϣ�������ѵ��TOTAL_TURN - 7��
  int gameTurn = rand() % maxTurn;
  SearchParam defaultSearchParam = { 1024,TOTAL_TURN,5.0 };//�����������ȡ��ֻ�������ɿ���ʱ����������
  for (int depth = 0; depth < gameTurn; depth++)
  {
    evaluator.evaluateSelf(1, defaultSearchParam);//����policy


    for (int i = 0; i < param.batchsize; i++)
    {
      assert(!evaluator.gameInput[i].isEnd());//�Ժ�ľ籾������Ա�֤���������ɾ�����assert
      evaluator.gameInput[i].applyTrainingAndNextTurn(rand, evaluator.actionResults[i]);
      assert(isVaildGame(evaluator.gameInput[i]));//�Ժ�ľ籾������Ա�֤���������ɾ�����assert
    }
  }
  for (int i = 0; i < param.batchsize; i++)
  {
    gameBuf[i] = randomizeBeforeOutput(evaluator.gameInput[i]);
  }
}
bool GameGenerator::isVaildGame(const Game& game)
{
  if (game.isEnd())return false;
  if (game.isRacing)return false;
  return true;
}

Game GameGenerator::get()
{
  while (true)
  {
    if (nextGamePointer >= param.batchsize)
    {
      nextGamePointer = 0;
      newGameBatch();
    }
    if (!isVaildGame(gameBuf[nextGamePointer]))
    {
      nextGamePointer += 1;
      continue;
    }
    else
    {
      nextGamePointer += 1;
      return gameBuf[nextGamePointer - 1];
    }
  }
}
