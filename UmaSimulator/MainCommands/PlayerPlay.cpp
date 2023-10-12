#include <iostream>
#include <iomanip> 
#include <random>
#include <sstream>
#include <cassert>
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::seconds
#include "../External/termcolor.hpp"
#include "../Game/Game.h"
#include "../Search/Search.h"
using namespace std;


void main_playerPlay()
{
  GameDatabase::loadUmas("../db/uma");
  GameDatabase::loadCards("../db/card");
  GameDatabase::loadDBCards("../db/cardDB.json");

  const int threadNum = 4;

  cout << termcolor::cyan << "�����￭���ž籾����ģ���� v0.1" << termcolor::reset << endl;
  cout << termcolor::cyan << "���� Sigmoid��QQ: 2658628026" << termcolor::reset << endl;
  cout << termcolor::cyan << "���뿪Դ��" << termcolor::yellow << "https://github.com/hzyhhzy/UmaAi" << termcolor::reset << endl;
  cout << termcolor::bright_cyan << "��ģ�����������ơ�С�ڰ塱��Ϊ�˷��㣬��û�����ܵĹ��ܣ��ѹ��м��ܺ͸��ּ���hint�������pt��ÿpt��Ϊ" << GameConstants::ScorePtRate << "�֣�����" << GameConstants::ScorePtRateQieZhe << "�֣�" << termcolor::reset << endl;
  cout << endl;

  random_device rd;
  auto rand = mt19937_64(rd());

  int umaId = 101101;//���Ϸ�
  int umaStars = 5;
  int cards[6] = { 301604,301344,300104,300194,300114,301074 };//���ˣ��߷壬���������������˾��
  int zhongmaBlue[5] = { 18,0,0,0,0 };
  int zhongmaBonus[6] = { 20,0,40,0,20,0 };
  for (int gamenum = 0; gamenum < 100000; gamenum++)
  {
    Search search;
    vector<Evaluator> evaluators;
    for (int i = 0; i < threadNum; i++)
      evaluators.push_back(Evaluator(NULL, 128));
    Game game;
    game.newGame(rand, true, umaId, umaStars, cards, zhongmaBlue, zhongmaBonus);


    cout << termcolor::bright_blue << "------------------------------------------------------------------------------------------------" << termcolor::reset << endl;
    cout << termcolor::green << "���������ǣ�" << GameDatabase::AllUmas[umaId].name << termcolor::reset << endl;
    cout << termcolor::green << "����俨�ǣ�";
    for (int i = 0; i < 6; i++)
      cout << GameDatabase::AllCards[cards[i]].cardName << ",";
    cout << termcolor::reset << endl;
    {
      cout << termcolor::bright_cyan << "��Enter����ʼ��Ϸ" << termcolor::reset << endl;
      if (gamenum != 0)std::cin.ignore(1000000, '\n');
      std::cin.get();
    }
    cout << endl;

    for (int turn = 0; turn < TOTAL_TURN - 7; turn++)
    {
      //assert(turn == game.turn && "�غ�������ȷ");
      game.randomDistributeCards(rand);
      game.print();
      /*
      //search.runSearch(game, evaluators.data(), 4096, TOTAL_TURN, 27000, threadNum, 0);
      for (int i = 0; i < 2; i++)
      {
        for (int j = 0; j < 8 + 4 + 6; j++)
        {
          cout
            //<< fixed << setprecision(1) << search.allChoicesValue[i][j].winrate * 100 << "%:" 
            << fixed << setprecision(0) << search.allChoicesValue[i][j].avgScoreMinusTarget << " ";
          if (j == 4 || j == 7 || j == 11)cout << endl;
        }
        cout << endl;
        cout << endl;
      }


      {
        auto policy = search.extractPolicyFromSearchResults(1);
        cout << fixed << setprecision(1) << policy.useVenusPolicy * 100 << "% ";
        cout << endl;
        for (int i = 0; i < 8; i++)
          cout << fixed << setprecision(1) << policy.trainingPolicy[i] * 100 << "% ";
        cout << endl;
        for (int i = 0; i < 3; i++)
          cout << fixed << setprecision(1) << policy.threeChoicesEventPolicy[i] * 100 << "% ";
        cout << endl;
        for (int i = 0; i < 6; i++)
          cout << fixed << setprecision(1) << policy.outgoingPolicy[i] * 100 << "% ";
        cout << endl;
      }
      */

      if (game.isRacing)//�����غ�
      {
        assert(false && "�����غ�Ӧ���Ѿ���game��������");
      }
      else//����ѵ���غ�
      {
        bool isRemake = false;
        while (true)
        {
          Action action;
          action.train = -1;
          action.buy50p = false;
          action.buyFriend20 = false;
          action.buyPt10 = false;
          action.buyVital20 = false;


          string s;

          cout << termcolor::cyan << "��ѡ��ѵ����1�٣�2�ͣ�3����4����5�ǣ�6SS��ս��a���˳��У�b��ͨ���У�c��Ϣ��d���������remake�ؿ���cheat����" << termcolor::reset << endl;
          cin >> s;

          if (s == "1")
            action.train = 0;
          else if (s == "2")
            action.train = 1;
          else if (s == "3")
            action.train = 2;
          else if (s == "4")
            action.train = 3;
          else if (s == "5")
            action.train = 4;
          else if (s == "6")
            action.train = 5;
          else if (s == "a")
          {
            cout << termcolor::green << "��ȷ��Ҫ����������yȷ�ϣ�����n����ѡ��" << termcolor::reset << endl;
            cin >> s;
            if (s != "y")
              continue;
            action.train = 7;
          }
          else if (s == "b")
          {
            cout << termcolor::green << "��ȷ��Ҫ����������yȷ�ϣ�����n����ѡ��" << termcolor::reset << endl;
            cin >> s;
            if (s != "y")
              continue;
            action.train = 8;
          }
          else if (s == "c")
          {
            cout << termcolor::green << "��ȷ��Ҫ��Ϣ������yȷ�ϣ�����n����ѡ��" << termcolor::reset << endl;
            cin >> s;
            if (s != "y")
              continue;
            action.train = 6;
          }
          else if (s == "d")
          {
            cout << termcolor::green << "��ȷ��Ҫ����������yȷ�ϣ�����n����ѡ��" << termcolor::reset << endl;
            cin >> s;
            if (s != "y")
              continue;
            action.train = 9;
          }
          else if (s == "remake")
          {
            cout << termcolor::bright_red << "��ȷ��Ҫ�ؿ�������remakeȷ���ؿ������������������ݼ�����Ϸ" << termcolor::reset << endl;
            cin >> s;
            if (s != "remake")
              continue;
            isRemake = true;
            cout << termcolor::red << "���" << termcolor::green << GameDatabase::AllUmas[umaId].name << termcolor::red << "�������⺺����" << termcolor::reset << endl;
            break;
          }
          else if (s == "cheat")//���ÿ������
          {
            cout << termcolor::bright_cyan << "�������·��䣡" << termcolor::reset << endl;
            game.randomDistributeCards(rand);
            game.print();
            continue;
          }
          else
          {
            cout << termcolor::red << "������������������" << termcolor::reset << endl;
            continue;
          }



















          /*

          if (game.venusAvailableWisdom != 0)
          {
            cout << termcolor::cyan << "�Ƿ���Ů����ǣ�y������n������" << termcolor::reset << endl;
            cin >> s;
            if (s == "y")
            {
              useVenus = true;
            }
            else if (s == "n")
            {
              useVenus = false;
            }
            else
            {
              cout << termcolor::red << "������������������" << termcolor::reset << endl;
              continue;
            }
          }


          if (chosenTrain == 7 && game.turn <= 12)
          {
            cout << termcolor::red << "ǰ13�غ��޷�����" << termcolor::reset << endl;
            continue;
          }




          int chosenOutgoing = 5;
          if (chosenTrain == 6 && game.venusCardUnlockOutgoing)
          {
            cout << termcolor::cyan << "��ѡ�������0Ϊ��ͨ��������Ů������ֱ�Ϊ 1 2 3 4 5" << termcolor::reset << endl;
            cin >> s;
            if (s == "0")
              chosenOutgoing = 5;
            else if (s == "1")
              chosenOutgoing = 0;
            else if (s == "2")
              chosenOutgoing = 1;
            else if (s == "3")
              chosenOutgoing = 2;
            else if (s == "4")
              chosenOutgoing = 3;
            else if (s == "5")
              chosenOutgoing = 4;
            else
            {
              cout << termcolor::red << "������������������" << termcolor::reset << endl;
              continue;
            }

            if (!game.isOutgoingLegal(chosenOutgoing))
            {
              cout << termcolor::red << "���Ϸ������������������" << termcolor::reset << endl;
              continue;
            }
          }


          assert(game.cardData[0]->cardType == 5 && "���ſ����ڵ�һ��λ��");


          if (chosenTrain >= 0 && chosenTrain < 5 && game.cardDistribution[chosenTrain][0])//���ſ���ѡ���ѵ��
          {
            if (chosenSpiritColor == -1)
            {
              cout << termcolor::cyan << "�������Ů����ѡһ�¼���ѡ��ʲô��ɫ����Ƭ��q�죬a����z��" << termcolor::reset << endl;
              cin >> s;
              if (s == "q")
              {
                chosenSpiritColor = 0;
              }
              else if (s == "a")
              {
                chosenSpiritColor = 1;
              }
              else if (s == "z")
              {
                chosenSpiritColor = 2;
              }
              else
              {
                cout << termcolor::red << "������������������" << termcolor::reset << endl;
                continue;
              }
            }
            else//��ǰѡ����Ƭ��
              cout << termcolor::cyan << "����ǰѡ����Ƭ��ɫ" << termcolor::reset << endl;

          }*/

          bool suc = game.applyTraining(rand, action);
          assert(suc);
          break;


        }
        if (isRemake)
          break;
      }
      cout << endl;
      game.checkEventAfterTrain(rand);
      std::this_thread::sleep_for(std::chrono::seconds(1));//�ȼ������˿����
    }
    cout << termcolor::red << "���ɽ�����" << termcolor::reset << endl;
    game.printFinalStats();
  }
}