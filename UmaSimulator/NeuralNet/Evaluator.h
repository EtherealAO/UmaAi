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
  std::vector<ModelOutputValueV1> valueResults;
  std::vector<ModelOutputPolicyV1> policyResults;
  std::vector<float> inputBuf;
  std::vector<float> outputBuf;

  void evaluate(const Game* games, const float* targetScores, int mode, int gameNum);//����gamesBuf��gameNum����Ϸ����������û��model����ʹ����д�߼�����policy�������ܼ���ǽ���״̬��value
  
  Evaluator(Model* model, int maxBatchsize);

  static Action handWrittenStrategy(const Game& game);
};