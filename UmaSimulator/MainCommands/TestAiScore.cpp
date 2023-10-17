//����ѵ������ֵ�㷨
#include <iostream>
#include <random>
#include <sstream>
#include <cassert>
#include <thread>
#include <atomic>
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
#include "../Search/Search.h"
#include "../External/termcolor.hpp"

#include "../GameDatabase/GameDatabase.h"
#include "../GameDatabase/GameConfig.h"

using namespace std;

const bool handWrittenEvaluationTest = true;
const int threadNum = 8;
const int threadNumInner = 1;
const double targetScore = 0;
//const double radicalFactor = 3;//������
const int searchN = handWrittenEvaluationTest ? 1 : 1024;
const bool recordGame = false;


const int totalGames = handWrittenEvaluationTest ? 1000000 : 10000000;
const int gamesEveryThread = totalGames / threadNum;


int umaId = 108401;//��ˮ��30���ӳ�
int umaStars = 5;
int cards[6] = { 301604,301344,301614,300194,300114,301074 };//���ˣ��߷壬��ӥ��������������˾��
int zhongmaBlue[5] = { 18,0,0,0,0 };
int zhongmaBonus[6] = { 20,0,40,0,20,150 };
bool allowedDebuffs[9] = { false, false, false, false, true, false, false, false, false };//�ڶ�����Բ����ڼ���debuff������������������߸���ǿ����


std::atomic<double> totalScore = 0;
std::atomic<double> totalScoreSqr = 0;

std::atomic<int> bestScore = 0;
std::atomic<int> n = 0;
vector<atomic<int>> segmentStats= vector<atomic<int>>(700);//100��һ�Σ�700��


void worker()
{
  random_device rd;
  auto rand = mt19937_64(rd());

  Search search(NULL, 16, threadNumInner);

  vector<Game> gameHistory;
  if (recordGame)
    gameHistory.resize(TOTAL_TURN);

  for (int gamenum = 0; gamenum < gamesEveryThread; gamenum++)
  {

    Game game;
    game.newGame(rand, false, umaId, umaStars, cards, zhongmaBlue, zhongmaBonus);
    for (int i = 0; i < 9; i++)
      game.larc_allowedDebuffsFirstLarc[i] = allowedDebuffs[i];

    while(!game.isEnd())
    {
      if (recordGame)
        gameHistory[game.turn] = game;
      Action action;
      if (handWrittenEvaluationTest) {
        action = Evaluator::handWrittenStrategy(game);
      }
      else {
        action = search.runSearch(game, searchN, TOTAL_TURN, targetScore, rand);
      }
      game.applyTrainingAndNextTurn(rand, action);
    }
    //cout << termcolor::red << "���ɽ�����" << termcolor::reset << endl;
    int score = game.finalScore();
    if (score > 39200)
    {
      if (recordGame)
        for (int i = 0; i < TOTAL_TURN; i++)
          if (!GameConstants::LArcIsRace[i])
            gameHistory[i].print();
      game.printFinalStats();
    }
    n += 1;
    totalScore += score;
    totalScoreSqr += score * score;
    for (int i = 0; i < 700; i++)
    {
      int refScore = i * 100;
      if (score >= refScore)
      {
        segmentStats[i] += 1;
      }
    }

    int bestScoreOld = bestScore;
    while (score > bestScoreOld && !bestScore.compare_exchange_weak(bestScoreOld, score)) {
      // ���val����old_max������max_val��ֵ����old_max����ô�ͽ�max_val��ֵ����Ϊval
      // ���max_val��ֵ�Ѿ��������̸߳��£���ô�Ͳ����κ����飬����old_max�ᱻ����Ϊmax_val����ֵ
      // Ȼ�������ٴν��бȽϺͽ���������ֱ���ɹ�Ϊֹ
    }


    //game.print();
    if (!handWrittenEvaluationTest || n == totalGames)
    {
      if(!handWrittenEvaluationTest)
        game.printFinalStats();
      cout << n << "�֣�������=" << searchN << "��ƽ����" << totalScore / n << "����׼��" << sqrt(totalScoreSqr / n - totalScore * totalScore / n / n) << "����߷�" << bestScore << endl;
      cout
        << "UE7����=" << float(segmentStats[327]) / n << ","
        << "UE8����=" << float(segmentStats[332]) / n << ","
        << "UE9����=" << float(segmentStats[338]) / n << ","
        << "UD0����=" << float(segmentStats[344]) / n << ","
        << "UD1����=" << float(segmentStats[350]) / n << ","
        << "UD2����=" << float(segmentStats[356]) / n << ","
        << "UD3����=" << float(segmentStats[362]) / n << ","
        << "UD4����=" << float(segmentStats[368]) / n << ","
        << "UD5����=" << float(segmentStats[375]) / n << ","
        << "UD6����=" << float(segmentStats[381]) / n << ","
        << "UD7����=" << float(segmentStats[387]) / n << ","
        << "UD8����=" << float(segmentStats[394]) / n << ","
        << "UD9����=" << float(segmentStats[400]) / n << ","
        << "UC0����=" << float(segmentStats[407]) / n << endl;
    }
  }

}

void main_testAiScore()
{


    // ��鹤��Ŀ¼
    GameDatabase::loadUmas("../db/uma");
    GameDatabase::loadCards("../db/card");
    GameDatabase::loadDBCards("../db/cardDB.json");


  for (int i = 0; i < 200; i++)segmentStats[i] = 0;


  std::vector<std::thread> threads;
  for (int i = 0; i < threadNum; ++i) {
    threads.push_back(std::thread(worker));
  }
  for (auto& thread : threads) {
    thread.join();
  }

  cout << n << "�֣�������=" << searchN << "��ƽ����" << totalScore / n << "����׼��" << sqrt(totalScoreSqr / n - totalScore * totalScore / n / n) << "����߷�" << bestScore << endl;

}