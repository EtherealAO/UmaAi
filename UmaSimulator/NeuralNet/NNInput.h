#pragma once
const int LATEST_NNINPUT_VERSION = 1;


//����������ṹ
//һ��һά����������Ϊ
//(ȫ����Ϣ)(����������)(֧Ԯ��1)...(֧Ԯ��6)(npc1)...(npc10)(���³�)(����)(�޿�����)
//֧Ԯ��1~6ÿ�����������֣�CARD��PERSON
//npcֻ����PERSON����
//��������������� ֧Ԯ��6���޿����� Ϊ��
//���û���������� ������������npc10Ϊ��
const int NNINPUT_CHANNELS_SEARCHPARAM_V1 = 3; //��������ռ���ٸ�ͨ��
const int NNINPUT_CHANNELS_GAMEGLOBAL_V1 = 300;//Game����˿�����ͷ��ͨ����
const int NNINPUT_CHANNELS_CARD_V1 = 67; //ÿ��֧Ԯ������ռ����ͨ���������������ͷ���֮���
const int NNINPUT_CHANNELS_PERSON_V1 = 74; //ÿ����ͷ����ͨ����������֧Ԯ������
const int NNINPUT_CHANNELS_V1 = NNINPUT_CHANNELS_SEARCHPARAM_V1 +
                                NNINPUT_CHANNELS_GAMEGLOBAL_V1 +
                                NNINPUT_CHANNELS_CARD_V1 * 6 +
                                NNINPUT_CHANNELS_PERSON_V1 * 18;//��ͨ����








const int NNOUTPUT_CHANNELS_POLICY_V1 = 18;
const int NNOUTPUT_CHANNELS_VALUE_V1 = 3;
const int NNOUTPUT_CHANNELS_V1 = NNOUTPUT_CHANNELS_POLICY_V1 + NNOUTPUT_CHANNELS_VALUE_V1;