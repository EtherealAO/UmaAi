#pragma once
#include <cstdint>
#include <random>
#include <string>
struct SupportCard;
struct Person //�κ�һ�����ܳ�����ѵ�������ͷ
{
  int8_t personType;//1����������R��SSR���У���2������֧ͨԮ����3����npc��ͷ��4���³���5���ߣ�6���������������ݲ�֧����������/�Ŷӿ�
  int16_t cardId;//֧Ԯ��id������֧Ԯ����0
  int16_t charaId;//npc��ͷ������id������npc��0������дҲ����һ��0��ֻ���ڻ��npc�����֣�

  SupportCard* cardData;// ��ǰ���ĸ��ֲ�����ָ��ָֻ��̬���ݣ��൱�ڴ��Int64���ݣ��������⸴��
  int8_t friendship;//�
  bool atTrain[5];//�Ƿ������ѵ���������ͨ�Ŀ�ֻ��one-hot����ȫ�գ�����ssr��������������true
  bool isShining;//�Ƿ����ʡ��޷����ʵĿ�����npc��Ϊfalse
  int8_t cardRecord;//��¼һЩ��������ʱ����ı�Ĳ�������������ֵĹ���

  int8_t larc_isLinkCard;//�Ƿ�Ϊlink֧Ԯ��
  int8_t larc_charge;//���ڳ��˼���
  bool larc_atSS;//�Ƿ���ss��ս��
  int8_t larc_statusType;//����������01234
  int8_t larc_specialBuff;//ÿ3����������У����ͬ��Ϸ��
  int8_t larc_level;//����
  int8_t larc_nextThreeBuffs[3];//��ǰ�Լ�����������buff
  int8_t larc_assignedStatusTypeWhenFull;//�����Ӧ��buff�ǡ����Լӳɡ������ĸ�����������ʱ���Ѿ�ȷ��

  
  std::discrete_distribution<> distribution;//distribution(rand)���Ը��ݵ���������0~5���������������ſ����������������Ǹ롣ssr������������

  void writeSinglePersonNNInput(float* buf);//��������������
  std::string getPersonName();//�����������
};