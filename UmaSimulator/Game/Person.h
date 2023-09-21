#pragma once
#include <cstdint>
#include <random>
#include <string>
struct SupportCard;
struct Game;
struct Person //�κ�һ�����ܳ�����ѵ�������ͷ
{
  int8_t personType;//0����δ���أ�����ǰ�����غϵ�npc����1��������֧Ԯ����R��SSR���У���2������֧ͨԮ����3����npc��ͷ��4���³���5���ߣ�6���������������ݲ�֧����������/�Ŷӿ�
  //int16_t cardId;//֧Ԯ��id������֧Ԯ����0
  int16_t charaId;//npc��ͷ������id������npc��0������дҲ����һ��0��ֻ���ڻ��npc�����֣�

  int8_t cardIdInGame;// Game.cardParam���֧Ԯ����ţ���֧Ԯ��Ϊ-1
  int8_t friendship;//�
  bool atTrain[5];//�Ƿ������ѵ���������ͨ�Ŀ�ֻ��one-hot����ȫ�գ�����ssr��������������true
  bool isShining;//�Ƿ����ʡ��޷����ʵĿ�����npc��Ϊfalse
  int8_t cardRecord;//��¼һЩ��������ʱ����ı�Ĳ�������������ֵĹ���

  bool larc_isLinkCard;//�Ƿ�Ϊlink֧Ԯ��
  int8_t larc_charge;//���ڳ��˼���
  bool larc_atSS;//�Ƿ���ss��ս��
  int8_t larc_statusType;//����������01234
  int8_t larc_specialBuff;//ÿ3����������У����ͬ��Ϸ��
  int8_t larc_level;//����
  int8_t larc_nextThreeBuffs[3];//��ǰ�Լ�����������buff
  
  //int8_t larc_assignedStatusTypeWhenFull;//�����Ӧ��buff�ǡ����Լӳɡ������ĸ�����������ʱ���Ѿ�ȷ��
  //����Ū�ˣ�ֱ���������
  
  std::discrete_distribution<> distribution;//distribution(rand)���Ը��ݵ���������0~5���������������ſ����������������Ǹ롣ssr������������

  Person();//δ���ص���ͷ
  void initAtTurn3(int specialBuff, int statusType);//�����غϳ�ʼ��

  

  void larc_afterSS(std::mt19937_64& rand);//��һ�����������µ�buff
  //int larc_getNextBuff(std::mt19937_64& rand);//����larc_level+3����buff

  void writeSinglePersonNNInput(float* buf) const;//��������������
  std::string getPersonName(const Game& game) const;//�����������
  std::string getPersonNameColored(const Game& game) const;//����������Ʋ���ע��ɫ
};