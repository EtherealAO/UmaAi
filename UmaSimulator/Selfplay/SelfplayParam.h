#pragma once
#include <vector>
#include <string>
struct SelfplayParam
{
  int threadNum;
  int threadNumInner;
  int maxSampleNum;
  int batchsize;
  std::string modelPath;
  int sampleNumEachFile;

  //log(searchN)~��̬�ֲ�(searchN_logmean,searchN_logstdev)
  double searchN_logmean;
  double searchN_logstdev;
  int searchN_max;

  //radicalFactor=radicalFactor_scale * (1/pow(rand(),radicalFactor_pow) - 1)
  double radicalFactor_scale;
  double radicalFactor_pow;
  double radicalFactor_max;

  //maxDepth_fullProb��������������ֽ�����������log(maxDepth)~��̬�ֲ�(logmean,logstdev)
  double maxDepth_logmean;
  double maxDepth_logstdev;
  int maxDepth_min;
  double maxDepth_fullProb;
  
};