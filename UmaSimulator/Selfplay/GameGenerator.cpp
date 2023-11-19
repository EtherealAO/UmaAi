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
  assert(false);
  return Game();
}

Game GameGenerator::randomizeBeforeOutput(const Game& game)
{
  assert(false);
  return Game();
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
