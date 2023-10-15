//����ѵ������ֵ�㷨
#include <iostream>
#include <random>
#include <sstream>
#include <cassert>
#include "../Game/Game.h"
#include "../External/termcolor.hpp"
using namespace std;


void main_test3()
{
  /*
  //���򵥵��߼�

  random_device rd;
  auto rand = mt19937_64(rd());
  int umaId = 1;
  int cards[6] = { 1,2,3,4,5,6 };
  int zhongmaBlue[5] = { 18,0,0,0,0 };
  int zhongmaBonus[6] = { 30,0,30,0,30,200 };

  double totalScore = 0;
  double totalScoreSqr = 0;
  int bestScore = 0;
  int n = 0;
  for (int gamenum = 0; gamenum < 10000; gamenum++)
  {

    Game game;
    game.newGame(rand, false, umaId, cards, zhongmaBlue, zhongmaBonus);
    for (int turn = 0; turn < TOTAL_TURN; turn++)
    {
      assert(turn == game.turn && "�غ�������ȷ");
      game.randomDistributeCards(rand);
      //game.print();
      if (game.isRacing)//�����غ�
      {
        bool useVenus = game.venusAvailableWisdom == 1;
        bool suc = game.applyTraining(rand, -1, useVenus, -1, -1);
        assert(suc);
      }
      else//����ѵ���غ�
      {
        bool useVenus = game.venusAvailableWisdom != 0;
        int chosenSpiritColor = 0;
        int chosenTrain = -1;
        int chosenOutgoing = -1;
        if (game.vital < 50 && game.venusAvailableWisdom != 1)//��Ů������������û�о���Ϣ
        {
          if (game.venusCardUnlockOutgoing && !game.venusCardOutgoingUsed[4] && !game.isXiaHeSu())
          {
            chosenTrain = 6;
            chosenOutgoing =
              (!game.venusCardOutgoingUsed[2]) ? 2 :
              (!game.venusCardOutgoingUsed[0]) ? 0 :
              (!game.venusCardOutgoingUsed[1]) ? 1 :
              (!game.venusCardOutgoingUsed[3]) ? 3 :
              4;
          }
          else
            chosenTrain = 5;
        }
        else
        {
          double bestValue = -10000;
          int bestTrain = -1;
          for (int item = 0; item < 5; item++)
          {
            const double jibanValue = 2;
            const double venusValue_first = 40;
            const double venusValue_beforeOutgoing = 6;
            const double venusValue_afterOutgoing = 30;
            const double venusValue_activated = 6;
            const double spiritValue = 20;
            const double vitalValue = 0.9;
            const double ptValue = 0.4;
            

            double expectSpiritNum = int(game.spiritDistribution[item] / 32) + 1;
            double value = 0;
            assert(game.cardId[0] == SHENTUAN_ID && "���ſ����ڵ�һ��λ��");
            for (int head = 0; head < 6; head++)
            {
              if (!game.cardDistribution[item][head])
                continue;
              if (head == 0)
              {
                if (!game.venusCardFirstClick)
                  value += venusValue_first;
                else if (!game.venusCardUnlockOutgoing)
                {
                  expectSpiritNum += 0.5;
                  value += venusValue_beforeOutgoing;
                }
                else if (!game.venusCardIsQingRe)
                {
                  expectSpiritNum += 0.5;
                  value += venusValue_afterOutgoing;
                }
                else
                {
                  expectSpiritNum += 1;
                  value += venusValue_activated;
                }
              }
              else
              {
                if (game.cardJiBan[head] < 80)
                {
                  int jibanAdd = 7;
                  if (game.cardHint[head])
                    jibanAdd += 5;
                  value += jibanAdd * jibanValue;
                }
              }
            }
            if (game.venusSpiritsBottom[7] > 0)
              expectSpiritNum = 0;
            else if (game.venusSpiritsBottom[6] > 0)
              expectSpiritNum = min(1, expectSpiritNum);
            else if (game.venusSpiritsBottom[5] > 0)
              expectSpiritNum = min(2, expectSpiritNum);
            value += spiritValue * expectSpiritNum;
            
            for (int i = 0; i < 5; i++)
              value += game.trainValue[item][i];

            value += ptValue * game.trainValue[item][5];
            value += vitalValue * game.trainValue[item][6];

            if (value > bestValue)
            {
              bestValue = value;
              bestTrain = item;
            }
          }
          chosenTrain = bestTrain;
        }


        bool suc = game.applyTraining(rand, chosenTrain, useVenus, chosenSpiritColor, chosenOutgoing);
        if (!suc)
        {
          cout << chosenTrain << useVenus << chosenSpiritColor << chosenOutgoing;
        }
        assert(suc);
      }
      game.checkEventAfterTrain(rand);
    }
    //cout << termcolor::red << "���ɽ�����" << termcolor::reset << endl;
    int score = game.finalScore();
    n += 1;
    totalScore += score;
    totalScoreSqr += score * score;
    if (score > bestScore)bestScore = score;
    //game.print();
    //game.printFinalStats();
  }
  cout << n << "�֣�ƽ����" << totalScore / n << "����׼��" << sqrt(totalScoreSqr / n - totalScore * totalScore / n / n) << "����߷�" << bestScore << endl;
  */
}