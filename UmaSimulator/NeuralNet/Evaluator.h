#pragma once
#include <vector>
#include "NNInput.h"
#include "Model.h"
#include "../Game/Game.h"

//ÿ���߳�һ��evaluator
//�����̹߳���һ��model
class Evaluator
{
public:
  Model* model;
  //static lock;//���е�evaluator����һ��lock
  int maxBatchsize;

  std::vector<Game> gameInput;

  std::vector<float> inputBuf;
  std::vector<float> outputBuf;

  std::vector<ModelOutputValueV1> valueResults;
  //std::vector<ModelOutputPolicyV1> policyResults;
  std::vector<Action> actionResults;


  //void evaluate(const Game* games, const float* targetScores, int mode, int gameNum);//����games��gameNum����Ϸ����������û��model����ʹ����д�߼�����policy�������ܼ���ǽ���״̬��value
  void evaluateSelf(int mode, float targetScore);//����gameInput����������û��model����ʹ����д�߼�����policy�������ܼ���ǽ���״̬��value
  
  Evaluator();
  Evaluator(Model* model, int maxBatchsize);

  static Action handWrittenStrategy(const Game& game);
};