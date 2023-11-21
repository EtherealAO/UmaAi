#pragma once
#include <vector>
#include "SelfplayParam.h"
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
class GameGenerator
{
  //���������һЩ���֣�Ȼ������������һЩ�غ�����������gameBuf���ڷ�����֮ǰ�ټ������
  SelfplayParam param;
  Evaluator evaluator;
  
  std::mt19937_64 rand;
  std::vector<Game> gameBuf;
  int nextGamePointer;

  Game randomOpening();
  Game randomizeBeforeOutput(const Game& game0);
  void newGameBatch();
  bool isVaildGame(const Game& game);
public:
  GameGenerator(SelfplayParam param, Model* model);
  Game get();
};