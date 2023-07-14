#pragma once
#include <vector>
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
//一局游戏是一个search
class Search
{
  Game rootGame;
  std::vector<Game> gamesBuf;
  //2代表是否开女神，1为开，0为不开或者没法开。
  //8代表五训练+休息外出比赛
  //4代表女神三选一事件的四种状态：不来，红，蓝，黄 （注，后三项相当于获得了“只要点女神肯定出来的buff“，但不代表不会训练失败，训练失败无法触发三选一事件）
  //6代表6种外出，前5个是女神外出，最后一个是普通外出
  double allChoicesValue[2][8 + 4 + 6];

  //对于每个可能的情况（最多2*(8+4+6)种），每个往后模拟eachSamplingNum局，模拟maxDepth回合后返回神经网络输出（神经网络输出是达到targetScore的概率）
  void runSearch(Evaluator* evaluator, int eachSamplingNum, int maxDepth, int targetScore);
};