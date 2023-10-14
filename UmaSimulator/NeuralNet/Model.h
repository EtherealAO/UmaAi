#pragma once
#include <vector>
#include "NNInput.h"
#include "../Game/Game.h"


struct ModelOutputPolicyV1
{
  float trainingPolicy[10];
  float buy50p;//�����Ӧѵ����+50%
  float buyPt10;//����pt+10
  float buyFriend20;//��������+20%
  float buyVital20;//������������-20%
  float expectShixingPtCost;//����pt���ĵ�����ֵ���������Ŀ���Ǳ���ֻ����һ��������������������£�����buy��ǡ�ô���50%
};
static_assert(sizeof(ModelOutputPolicyV1) == sizeof(float) * NNOUTPUT_CHANNELS_POLICY_V1, "NNOUTPUT_CHANNELS_POLICY_V1����");


struct ModelOutputValueV1
{
  float scoreMean;//score��ƽ��ֵ
  float scoreStdev;//score�ı�׼��
  float winRate;//score>=target�ĸ���
  float scoreOverTargetMean;//max(target,score)��ƽ��ֵ
  float scoreOverTargetStdev;//max(target,score)�ı�׼��
  float extract(int i);
};
static_assert(sizeof(ModelOutputValueV1) == sizeof(float) * NNOUTPUT_CHANNELS_VALUE_V1, "NNOUTPUT_CHANNELS_VALUE_V1����");

struct ModelOutputV1
{
  ModelOutputValueV1 value;
  ModelOutputPolicyV1 policy;
};
static_assert(sizeof(ModelOutputV1) == sizeof(float) * NNOUTPUT_CHANNELS_V1,"NNOUTPUT_CHANNELS_V1����");

class Model
{
  //Model* model;
  //static lock;//���е�evaluator����һ��lock
  //void evaluate(const Game* gamesBuf, const float* otherInputsBuf, int gameNum);//����gamesBuf��gameNum����Ϸ������������outputBuf

};