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
const int threadNum = 16;
const int threadNumInner = 1;
const double radicalFactor = 3;//������
const int searchN = handWrittenEvaluationTest ? 1 : 3072;


const int totalGames = handWrittenEvaluationTest ? 120000 : 10000000;
const int gamesEveryThread = totalGames / threadNum;


int umaId = 103001;//��ԡ
int umaStars = 5;
int cards[6] = { 301604,301344,301614,300194,300114,301074 };//���ˣ��߷壬��ӥ��������������˾��
int zhongmaBlue[5] = { 18,0,0,0,0 };
int zhongmaBonus[6] = { 20,0,40,0,20,150 };


std::atomic<double> totalScore = 0;
std::atomic<double> totalScoreSqr = 0;

std::atomic<int> bestScore = 0;
std::atomic<int> n = 0;
vector<atomic<int>> segmentStats= vector<atomic<int>>(500);//100��һ�Σ�500��


void worker()
{
  random_device rd;
  auto rand = mt19937_64(rd());

  Search search;
  vector<Evaluator> evaluators;
  for (int i = 0; i < threadNumInner; i++)
    evaluators.push_back(Evaluator(NULL, 128));

  for (int gamenum = 0; gamenum < gamesEveryThread; gamenum++)
  {

    Game game;
    game.newGame(rand, false, umaId, umaStars, cards, zhongmaBlue, zhongmaBonus);

    while(!game.isEnd())
    {
      Action action;
      if (handWrittenEvaluationTest) {
        action = Evaluator::handWrittenStrategy(game);
      }
      else {
        search.runSearch(game, evaluators.data(), searchN, TOTAL_TURN, 27000, threadNumInner, radicalFactor);
        assert(false);
        //policy = search.extractPolicyFromSearchResults(1);
      }
      //assert(false);
      //Search::runOneTurnUsingPolicy(rand, game, policy, true);
      game.applyTrainingAndNextTurn(rand, action);
    }
    //cout << termcolor::red << "���ɽ�����" << termcolor::reset << endl;
    int score = game.finalScore();
    //if (score > 37000)
    //  game.printFinalStats();
    n += 1;
    totalScore += score;
    totalScoreSqr += score * score;
    for (int i = 0; i < 500; i++)
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
    if (!handWrittenEvaluationTest)
    {
      game.printFinalStats();
      cout << n << "�֣�������=" << searchN << "��ƽ����" << totalScore / n << "����׼��" << sqrt(totalScoreSqr / n - totalScore * totalScore / n / n) << "����߷�" << bestScore << endl;
      cout
        << "33500�ָ���=" << float(segmentStats[335]) / n << ","
        << "34000�ָ���=" << float(segmentStats[340]) / n << ","
        << "34500�ָ���=" << float(segmentStats[345]) / n << ","
        << "35000�ָ���=" << float(segmentStats[350]) / n << ","
        << "35500�ָ���=" << float(segmentStats[355]) / n << ","
        << "36000�ָ���=" << float(segmentStats[360]) / n << ","
        << "36500�ָ���=" << float(segmentStats[365]) / n << ","
        << "37000�ָ���=" << float(segmentStats[370]) / n << ","
        << "37500�ָ���=" << float(segmentStats[375]) / n << ","
        << "38000�ָ���=" << float(segmentStats[380]) / n << ","
        << "38500�ָ���=" << float(segmentStats[385]) / n << ","
        << "39000�ָ���=" << float(segmentStats[390]) / n << endl;
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