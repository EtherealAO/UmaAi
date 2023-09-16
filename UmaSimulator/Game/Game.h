#pragma once
#include <random>
#include <array>
#include "../GameDatabase/GameDatabase.h"
#include "../Game/Person.h"

struct Game
{
  //��ʾ���
  bool playerPrint;//�������ʱ����ʾ������Ϣ

  //����״̬����������ǰ�غϵ�ѵ����Ϣ
  int16_t umaId;//�����ţ���KnownUmas.cpp
  int16_t fiveStatusBonus[5];//�������ά���Եĳɳ���
  int16_t turn;//�غ�������0��ʼ����77����
  int16_t vital;//������������vital������Ϊ��Ϸ��������е�
  int16_t maxVital;//��������
  bool isQieZhe;//����
  bool isAiJiao;//����
  int16_t failureRateBias;//ʧ���ʸı�������ϰ����=2����ϰ����=-2
  int16_t fiveStatus[5];//��ά���ԣ�1200���ϲ�����
  //int fiveStatusUmaBonus[5];//��������ӳ�
  int16_t fiveStatusLimit[5];//��ά�������ޣ�1200���ϲ�����
  int16_t skillPt;//���ܵ�
  int16_t skillScore;//�����ܵķ���
  int16_t motivation;//�ɾ�����1��5�ֱ��Ǿ����������õ�
  //int cardId[6];//6�ſ���id
  //int cardJiBan[8];//����ſ��ֱ�012345�����³�6������7
  int16_t trainLevelCount[5];//���ѵ���ĵȼ��ļ�����ʵ��ѵ���ȼ�=min(5,t/4+1)
  int16_t zhongMaBlueCount[5];//����������Ӹ���������ֻ��3��
  int16_t zhongMaExtraBonus[6];//����ľ籾�����Լ����ܰ����ӣ���Ч��pt����ÿ�μ̳мӶ��١�ȫ��ʦ�����ӵ���ֵ��Լ��30��30��200pt
  //bool raceTurns[TOTAL_TURN];//��Щ�غ��Ǳ��� //��umaId�������GameDatabase::AllUmas����
  SupportCard cardParam[6];//���ſ��Ĳ���������Ҳ������������������Ŀ����ѵ��aiʱ����Ҫ����ı俨�Ĳ������³���ԣ�����ÿ��game�Ŀ��Ĳ������ܲ�һ��
  Person persons[18];//����������������Ŷӿ������18��ͷ��������15���ɳ����ͷ������֧Ԯ����˳�����⣩��0~4��5������npc��5��6~14�������³�15������16������17����û��������17��
  bool isRacing;//����غ��Ƿ��ڱ���

  int motivationDropCount;//�������������ˣ���֪ͬһ�������鲻����ֶ�Σ�һ��3���������¼�������֮ǰ����Խ�֮࣬����ĸ���Խ�ͣ�


  //���������

  bool larc_isAbroad;//����غ��Ƿ��ں���
  //int32_t larc_supportPt;//�Լ���֧Ԯpt
  int32_t larc_supportPtAll;//�����ˣ��Լ�+�����ˣ���֧Ԯpt֮�ͣ�ÿ1700֧Ԯpt��Ӧ1%�ڴ���
  int16_t larc_shixingPt;//����pt
  int16_t larc_levels[10];//10���������Եĵȼ���0Ϊδ����
  bool larc_isSSS;//�Ƿ�Ϊsss
  bool larc_ssWinSinceLastSSS;//���ϴ�sss������win������ss����������һ����sss�ĸ��ʣ�
  bool larc_isFirstLarcWin;// ��һ��������ӮûӮ
  bool larc_allowedDebuffsFirstLarc[3][8];//��һ�ο����ſ��Բ�����Щdebuff����ҿ�������3����ϣ�����һ�ּ���

  int16_t larc_zuoyueType;//û��������=0������SSR��=1������R��=2
  int16_t larc_zuoyueCardLevel;//�������ĵȼ�
  bool larc_zuoyueFirstClick;//�����Ƿ�����һ��
  bool larc_zuoyueOutgoingUnlocked;//�����������
  int16_t larc_zuoyueOutgoingUsed;//����������˼�����




  //��ǰ�غϵ�ѵ����Ϣ
  //0֧Ԯ����δ���䣬1֧Ԯ��������ϻ������ʼǰ��2ѵ������������������0�����̶ֹ��¼�������¼���������һ���غ�
  //stageInTurn=0ʱ�������������������ֵ��stageInTurn=1ʱ�����������������policy
  int16_t stageInTurn;
  int16_t personDistribution[5][5];//ÿ��ѵ������Щ��ͷid��personDistribution[�ĸ�ѵ��][�ڼ�����ͷ]������ͷΪ-1
  bool cardHint[6];//���ſ��ֱ���û�������
  int16_t spiritDistribution[5 + 3];//��Ƭ�ֲ�����������ѵ��01234����Ϣ5�����6������7����Ϊ2��Ƭ�����32

  //ͨ�������õ���Ϣ
  int16_t trainValue[5][7];//��һ�����ǵڼ���ѵ�����ڶ���������������������pt����
  int16_t failRate[5];//ѵ��ʧ����
  int16_t trainShiningNum[5];//���ѵ���м�����Ȧ
  int16_t larc_trainBonus;//�ڴ���ѵ���ӳ�
  int16_t larc_ssPersonsCount;//ss�м�����
  int16_t larc_ssPersons[5];//ss���ļ�����
  int16_t larc_ssValue[7];//ss������������pt�������ϲ������Ҳ�㣬���ܻ����pt��
  int16_t larc_ssSpecialEffects[13];//ss������buff��ȥ���������ظ��ġ����硰����+���顱�ڴ˴�ֻ�������飩
  int16_t larc_ssSupportPtGain;//ss��֧Ԯpt�ܹ��Ӷ��٣��Լ�+������ͷ��
  int16_t larc_ssFailRate;//ss��ʧ����



  //��Ϸ����:
  //newGame();
  //for (int t = 0; t < TOTAL_TURN; t++)
  //{
  //  if (!isRacing)//����ѵ���غ�
  //  {
  //    randomDistributeCards();
  //    PLAYER_CHOICE;
  //    applyTraining();
  //    checkEventAfterTrain();
  //  }
  //  else//�����غ�
  //  {
  //    randomDistributeCards();//ֻ��stageInTurn�ĳ�1
  //    if(venusAvailableWisdom!=0)//�Ƿ�ʹ��Ů����ǣ�����ʹ�õ�ʱ��ֱ���������߲�
  //    {
  //      PLAYER_CHOICE;
  //    }
  //    applyTraining();//�������ֻ��Ů�񣬲��ɱ��
  //    checkEventAfterTrain();//�����Ӷ�������������ﴦ��
  //  }
  //}
  //finalScore();
  //



  void newGame(std::mt19937_64& rand,
    bool enablePlayerPrint,
    int newUmaId, 
    int umaStars,
    int newCards[6],
    int newZhongMaBlueCount[5],
    int newZhongMaExtraBonus[6]);//������Ϸ�����֡�umaId��������
  void initHeads();//�����غϳ�ʼ��npc��ͷ

  bool loadGameFromJson(std::string jsonStr);

  void initRandomGenerators();

  void randomDistributeCards(std::mt19937_64& rand);//������俨�����Ƭ
  void calculateTrainingValue();//��������ѵ���ֱ�Ӷ��٣�������ʧ����

  // ����ѵ����ı仯��������Ϸ����򷵻�false���ұ�֤�����κ��޸�
  // ���У�chosenTrain����ѡ���ѵ����01234�ֱ������������ǣ�5����Ϣ��6�������7�Ǳ�����
  // useVenus�Ǽ���Ů���������Ƿ���Ů��
  // chosenSpiritColor�Ǽ������Ů����ѡһ�¼���ѡ�����Ƭ��ɫ�������Ʒֱ�012
  // chosenOutgoing����������ѡ��������Ŀ�������������ֱ���01234����ͨ�����5��
  //ע����ͨ�غ���14�ֿ��ܣ�5��ѵ��������һ��ѵ�����ܻ����Ů����ѡһ��������������Ϣ��������5�ֳ��У��������غ�ֻ�п�����Ů������ѡ��
  // forceThreeChoicesEvent��ǿ���ٻ���ѡһ�¼���1Ϊǿ���ٻ���-1Ϊǿ�Ʋ��ٻ���0Ϊ�������������ٻ����������ý�����ai����
  bool applyTraining(
    std::mt19937_64& rand, 
    int chosenTrain, 
    bool useVenus, 
    int chosenSpiritColor, 
    int chosenOutgoing,
    int forceThreeChoicesEvent = 0);
  void checkEventAfterTrain(std::mt19937_64& rand);//���̶��¼�������¼�����������һ���غ�

  void applyTrainingAndNextTurn(
    std::mt19937_64& rand,
    int chosenTrain,
    bool useVenus,
    int chosenSpiritColor,
    int chosenOutgoing,
    int forceThreeChoicesEvent = 0);//һֱ������У�ֱ����һ����Ҫ��Ҿ���

  int finalScore() const;//�����ܷ�
  bool isEnd() const;//

  //��������
  void activateVenusWisdom();//ʹ��Ů�����
  int getTrainingLevel(int item) const;//����ѵ���ȼ�����0��ʼ����Ϸ���k����������k-1������Ů����5��
  bool isOutgoingLegal(int chosenOutgoing) const;//�������Ƿ��ǿ��Խ��е�
  bool isXiaHeSu() const;//�Ƿ�Ϊ�ĺ���
  double getThreeChoicesEventProb(bool useVenusIfFull) const;//�����Ů����¼��ĸ���
  //void runTestGame();

  void getNNInputV1(float* buf, float targetScore, int mode) const;//���������룬mode=0��value��1��policy
  void print() const;//�ò�ɫ������ʾ��Ϸ����
  float getSkillScore() const;//���ܷ֣�����������֮ǰҲ������ǰ��ȥ
  void printFinalStats() const;//��ʾ���ս��

  void addStatus(int idx, int value);//��������ֵ�����������
  void addAllStatus(int value);//�����������ֵ
  void addVital(int value);//�������������������
  void addMotivation(int value);//��������
  void addJiBan(int idx,int value);//����������ǰ���
  void addTrainingLevelCount(int item, int value);//����ѵ���ȼ�������ÿ12Ϊ1����ѵ��+2����Ƭ+1���������
  void addSpirit(std::mt19937_64& rand, int s);//�����Ƭ
  void clearSpirit();//�����Ƭ
  int calculateFailureRate(int trainType) const;//����ѵ��ʧ����
  void calculateVenusSpiritsBonus();//������Ƭ�ӳ�  
  std::array<int,6> calculateBlueVenusBonus(int trainType) const;//���㿪��Ů��ļӳ�
  void runRace(int basicFiveStatusBonus, int basicPtBonus);//�ѱ��������ӵ����Ժ�pt�ϣ������ǲ�������ӳɵĻ���ֵ


  //һЩ���ڸ��ӵ��¼���������
  void handleVenusOutgoing(int chosenOutgoing);//Ů�����
  void handleVenusThreeChoicesEvent(std::mt19937_64& rand, int chosenColor);//Ů����ѡһ�¼�

  //��ʾ�¼�
  void printEvents(std::string s) const;//����ɫ������ʾ�¼�

  void calculateTrainingValueSingle(int trainType);//����ÿ��ѵ���Ӷ���
};

