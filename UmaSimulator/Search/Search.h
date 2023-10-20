#pragma once
#include <vector>
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
//һ����Ϸ��һ��search
class Search
{
public:
  int threadNumInGame;//һ��search���漸���߳�
  int batchSize;
  std::vector<Evaluator> evaluators;
  //Game rootGame;//��ǰҪ�����ľ���


  //10��Ӧaction.train
  //��Զ��(turn<=35 || 43<=turn<=59)��allChoicesValue[0]��Ч��allChoicesValue[1~3]��Ч��allChoicesValue[0][action.train]
  //��һ��Զ�������ǰ(36<=turn<=39)��allChoicesValue[0~3]��Ч��allChoicesValue[buy50 + 2*buyPt10][action.train]
  //��һ��Զ������ͺ�(turn==41)��allChoicesValue[0~1]��Ч��allChoicesValue[buy50][action.train]�������￪ʼ��20%�����10ptȫ�Զ�
  //�ڶ���Զ��(turn>=60)��allChoicesValue[0~3]��Ч��allChoicesValue[buy50 + 2*buyVital20][action.train]
  static int buyBuffChoiceNum(int turn);//�м��ֿ��ܵ���buff�ķ�ʽ
  static Action buyBuffAction(int idx, int turn);//������buff�ķ�ʽ
  ModelOutputValueV1 allChoicesValue[4][10];

  std::vector<ModelOutputValueV1> resultBuf;


  //����ÿ�����ܵ���������40�֣���ÿ������ģ��eachSamplingNum�֣�ģ��maxDepth�غϺ󷵻����������������������ǴﵽtargetScore�ĸ��ʣ�
  //���ڶ��̣߳��ݶ��ķ������£�
  // 1.Model����������Ѿ�����õ�float����*batchsize�����Ҳ������*batchsize��Model��������ͬʱֻ�ܼ���һ��
  // 2.����evaluateSingleAction����eachSamplingNum����Ϸ���threadNumInGame�飬ÿ��һ���߳�(Evaluator)��ÿ���̷ֳ߳�eachSamplingNum/(threadNumInGame*batchsize)С�飬ÿС��batchsize����Ϸ�����μ���ÿ��С��ķ��������������֮����������
  // 3.���Ҫ�ܺܶ�֣��������ף�����ͬʱ��threadGame�֣����߳���ΪthreadGame*threadNumInGame����eachSamplingNum��Сbatchsize�ϴ󣬿�����threadNumInGame=1
  // Ƕ�׽ṹ��Search(threadGame��)->Evaluator(threadGame*threadNumInGame��)->Model(1��)
  
  Search(Model* model, int batchSize, int threadNumInGame);



  Action runSearch(const Game& game, 
    int samplingNum, int maxDepth, int targetScore,
    std::mt19937_64& rand);

  //���㵥��action����ֵ
  ModelOutputValueV1 evaluateSingleAction(
    const Game& game,
    int samplingNum, int maxDepth, int targetScore,
    std::mt19937_64& rand,
    Action action);

  //���㵥��action����ֵ�������߳�
  void evaluateSingleActionThread(
    int threadIdx,
    ModelOutputValueV1* resultBuf, //��ÿ�ֵĽ�����浽����
    const Game& game,
    int samplingNum, int maxDepth, int targetScore,

    std::mt19937_64& rand,
    Action action
  );

  //�����������ѡ�����ѡ��policyҲ��һ������
  //mode=0�Ǹ���ʤ�ʣ�=1�Ǹ���ƽ����
  ModelOutputPolicyV1 extractPolicyFromSearchResults(int mode, float delta = 0);

};