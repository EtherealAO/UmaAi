#pragma once

#define UMAAI_MAINAI   //ʹ��ai
//#define UMAAI_TESTSCORE   //����ai����
//#define UMAAI_SIMULATOR   //����ģ����

#if defined UMAAI_TESTSCORE || defined UMAAI_SIMULATOR 
#define PRINT_GAME_EVENT
#endif

const int MAX_SCORE = 70000;//�������ķ�����70000��larc�籾�϶��ܹ�����