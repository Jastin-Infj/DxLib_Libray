#include <iostream>
#include <time.h>
#include <bitset>
#include <fstream>

using namespace std;

#define INITIALIZE 0
#define TITLE      1
#define GAMEMAIN   2
#define ENDING     3

struct Chara
{
	unsigned int momey;                 //������
	unsigned int rate;                  //�����Ō��߂����[�g���z
	struct Chara* next;                //���X�g�\��
	enum Zynken
	{
		N     = 1,							//�������Ă��Ȃ�
		GU    = 2,							//�O�[
		TYOKI = 4,							//�`���L
		PA    = 8							//�p�[
	};
	int flag;                               //1�@�j���[�g�����@2�@�O�[�@4�@�`���L�@8�@�p�[
}chara;

const int MOMEY = 1000;                     //����������
const int FLAG = 0;
const int RATE = 0;
const int PATTEN[4] = { Chara::N,Chara::GU,Chara::TYOKI,Chara::PA };

//�v���g�^�C�v�錾
void Initialize();
void Title();
void GameMain();
void Ending();
void Initial_Parameter_Display(bool setup);
int Input();
bool Rate();
int Random(Chara* pl);
bool SetUp();
void Substitution(int value, char* target);
void VictoryDecision();
void Debag_Output(char* Debag_name);

//�֐��|�C���^
void(*Game_Scece[4])() = { Initialize,Title,GameMain,Ending };


bool SetUp()                            //�����������@�f�[�^�̓ǂݍ���
{
    cout << "�p�����[�^�̐ݒ�����܂�" << endl << endl;
    chara.momey = MOMEY;
    chara.flag = FLAG;
    chara.next = (Chara*)malloc(sizeof(Chara));       // ������l�̃L������ǉ�����
    return true;
}
void Initialize()                       //����������
{
    cout << "���������������܂�" << endl << endl;
    bool setup = SetUp();
    if (setup)
    {
        cout << "�Z�b�g�A�b�v����" << endl << endl;
        //true�Ȃ�
        Game_Scece[TITLE]();
    }
}
void Title()                            //�^�C�g����ʂł̎�ȓ���
{
    cout << "�^�C�g�����" << endl << endl;
    Game_Scece[GAMEMAIN]();
}
bool Rate()
{
    unsigned int rate = 0;
    cout << "�q���邨������͂��Ă��������B" << endl;
    cin >> rate;
    if ((signed)rate > 0)
    {
        if ((signed)chara.momey - (signed)rate < 0)
        {
            cout << "������������܂���" << endl;
            return false;                               //���s
        }
        else
        {
            chara.rate = (signed)rate;
            chara.momey -= rate;
            cout << "�|������ݒ肵�܂���" << endl;
            cout << "�c����z:" << (signed)chara.momey << "�~" << endl;
            return true;
        }
    }
    else if (rate == 0)
    {
        cout << "�|�������O�~�ł�" << endl;
        return false;
    }
    else
    {
        cout << "�[�̕����͓o�^�ł��܂���" << endl;
        cout << "������x���͂����Ă��������B" << endl;
    }
    return false;
}
int Input()
{
    int value = 0;
    cout << "����񂯂�͉����o���܂����H" << endl;
    cout << "1 �O�[�@2�@�`���L�@3�@�p�[" << "    ";
    cin >> value;
    if (value >= 1 && value < 4)
    {
        return value;
    }
    cout << "���͎��s�@������x���͂��Ă�������" << endl << endl;
    return 0;
}
void Initial_Parameter_Display(bool setup)
{
    cout << "���݂̏�����" << chara.momey <<"�~" << endl;
    if (setup)
    {
        cout << "���݂̃��[�g" << chara.rate << "�~" << endl;
    }
}
int Random(Chara* pl)
{
	//���݂̎��ɂ���ė����\�����肷��
	srand((unsigned int)time(NULL));

	//Enemy�̂���񂯂�p�^�[������͂���
	int random = rand() % 3 + 1;
	pl->next->flag = FLAG;			//�s��l����O�ɂ���
	return random;					//�����ŕԂ�l��Ԃ�
}
void Substitution(int value ,char* target)
{
	if (target == "Chara")
	{
		chara.flag |= PATTEN[0];
		chara.flag &= FLAG;	  //��Ԃ�S�ĂO�ɂ���
		chara.flag |= PATTEN[value];
	}
	if (target == "Enemy")
	{
		chara.next->flag |= PATTEN[0];
		chara.next->flag &= FLAG;//��Ԃ�S�ĂO�ɂ���
		chara.next->flag |= PATTEN[value];
	}
}
void VictoryDecision()
{
	if (chara.flag & chara.next->flag)  //if���̐�����@0�ȊO�Ȃ�true
	{
		cout << "������" << endl;
		return;	
	}
	if (chara.flag & (chara.GU | (chara.next->flag & chara.GU)))//���肪�O�[�ł���@����ł��Ȃ�
	{
		
	}
}
void GameMain()                         //�֐��|�C���^�ōs���@�}�X�^�[�X�V����
{
    //����񂯂�
    int value = 0;
    bool rate = false;
    cout << "�Q�[���{��" << endl << endl;
    Initial_Parameter_Display(false);
    //���͂̏���
    while (rate == false)
    {
        rate = Rate();
        if (rate == true)
        {
            break;
        }
    }
    while (value == 0)
    {
        value = Input();
        if (value != 0)
        {
			Substitution(value, "Chara");
            break;
        }
    }
    Initial_Parameter_Display(true);
    
	Substitution(Random(&chara), "Enemy");

	Debag_Output("2bit");//�f�o�b�O���[�h

	VictoryDecision();
    //���݂̎��Ԃ��擾���ė����\�Ƃ��ē��Ă͂߂�
    Game_Scece[ENDING]();
}
void Ending()
{
    cout << "�G���f�B���O" << endl << endl;
    free(chara.next);
    chara.next = nullptr;
    cout << "�Q�[�����I�����܂�" << endl << endl;
}
void main()
{
    cout << "�Q�[���J�n���܂�" << endl << endl;
    Game_Scece[INITIALIZE]();             //����������
    system("pause");
}
void Debag_Output(char* Debag_name)
{
	char* str = "";
	char* str1 = "";
	if (Debag_name == "2bit")
	{
		switch (chara.flag)
		{
		case Chara::N:
			str = "�j���[�g����";
			break;
		case Chara::GU:
			str = "�O�[";
			break;
		case Chara::TYOKI:
			str = "�`���L";
			break;
		case Chara::PA:
			str = "�p�[";
			break;
		}
		cout << "�����̂���񂯂�p�^�[�� :  " << str << endl;
		switch (chara.next->flag)
		{
		case Chara::N:
			str1 = "�j���[�g����";
			break;
		case Chara::GU:
			str1 = "�O�[";
			break;
		case Chara::TYOKI:
			str1 = "�`���L";
			break;
		case Chara::PA:
			str1 = "�p�[";
			break;
		}
		cout << "����̂���񂯂�p�^�[�� :  " << str1 << endl;
	}
	cout << endl;
}
