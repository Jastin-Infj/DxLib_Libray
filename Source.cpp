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
	unsigned int momey;                 //所持金
	unsigned int rate;                  //自分で決めたレート金額
	struct Chara* next;                //リスト構造
	enum Zynken
	{
		N     = 1,							//何もしていない
		GU    = 2,							//グー
		TYOKI = 4,							//チョキ
		PA    = 8							//パー
	};
	int flag;                               //1　ニュートラル　2　グー　4　チョキ　8　パー
	enum WIN_OR_LOSS
	{
		WIN  = 1,								//勝ち
		LOSS = 2,								//負け
		Draw = 3,								//引き分け
	};
}chara;

const int MOMEY = 1000;                     //初期所持金
const int FLAG = 0;
const int RATE = 0;
const int PATTEN[4] = { Chara::N,Chara::GU,Chara::TYOKI,Chara::PA };

//プロトタイプ宣言
void Initialize();
void Title();
void GameMain();
void Ending();
void Initial_Parameter_Display(bool setup);
int Input();
bool Rate();
int Random(Chara* pl);
bool SetUp();//続きかどうかを判定する引数
void Substitution(int value, char* target);
int VictoryDecision();
unsigned int Momey(int w_or_l);
void Debag_Output(char* Debag_name);

//関数ポインタ
void(*Game_Scece[4])() = { Initialize,Title,GameMain,Ending };


bool SetUp()                            //初期化処理　データの読み込み
{
    cout << "パラメータの設定をします" << endl << endl;
	if ((chara.flag & (Chara::GU | Chara::TYOKI | Chara::PA)) != 0)//もう一度続けた場合
	{
		chara.flag = FLAG;
		chara.next = (Chara*)malloc(sizeof(Chara));       // もう一人のキャラを追加する
	}
	else
	{
		chara.momey = MOMEY;
		chara.flag = FLAG;
		chara.next = (Chara*)malloc(sizeof(Chara));       // もう一人のキャラを追加する
	}
    return true;
}
void Initialize()                       //初期化処理
{
    cout << "初期化処理をします" << endl << endl;
    bool setup = SetUp();
    if (setup)
    {
        cout << "セットアップ完了" << endl << endl;
        //trueなら
        Game_Scece[TITLE]();
    }
}
void Title()                            //タイトル画面での主な動き
{
    cout << "タイトル画面" << endl << endl;
    Game_Scece[GAMEMAIN]();
}
bool Rate()
{
    unsigned int rate = 0;
    cout << "賭けるお金を入力してください。" << endl;
    cin >> rate;
    if ((signed)rate > 0)
    {
        if ((signed)chara.momey - (signed)rate < 0)
        {
            cout << "所持金が足りません" << endl;
            return false;                               //失敗
        }
        else
        {
            chara.rate = (signed)rate;
            chara.momey -= rate;
            cout << "掛け金を設定しました" << endl;
            cout << "残り金額:" << (signed)chara.momey << "円" << endl;
            return true;
        }
    }
    else if (rate == 0)
    {
        cout << "掛け金が０円です" << endl;
        return false;
    }
    else
    {
        cout << "ーの符号は登録できません" << endl;
        cout << "もう一度入力をしてください。" << endl;
    }
    return false;
}
int Input()
{
    int value = 0;
    cout << "じゃんけんは何を出しますか？" << endl;
    cout << "1 グー　2　チョキ　3　パー" << "    ";
    cin >> value;
    if (value >= 1 && value < 4)
    {
        return value;
    }
    cout << "入力失敗　もう一度入力してください" << endl << endl;
    return 0;
}
void Initial_Parameter_Display(bool setup)
{
    cout << "現在の所持金" << chara.momey <<"円" << endl;
    if (setup)
    {
        cout << "現在のレート" << chara.rate << "円" << endl;
    }
}
int Random(Chara* pl)
{
	//現在の時によって乱数表を決定する
	srand((unsigned int)time(NULL));

	//Enemyのじゃんけんパターンを入力する
	int random = rand() % 3 + 1;
	pl->next->flag = FLAG;			//不定値から０にする
	return random;					//乱数で返る値を返す
}
void Substitution(int value ,char* target)
{
	if (target == "Chara")
	{
		chara.flag |= PATTEN[0];
		chara.flag &= FLAG;	  //状態を全て０にする
		chara.flag |= PATTEN[value];
	}
	if (target == "Enemy")
	{
		chara.next->flag |= PATTEN[0];
		chara.next->flag &= FLAG;//状態を全て０にする
		chara.next->flag |= PATTEN[value];
	}
}
int VictoryDecision()
{
	if (chara.flag & chara.next->flag)  //if文の性質上　0以外ならtrue
	{
		cout << "あいこ" << endl;
		return Chara::Draw;	
	}
	switch (chara.flag)
	{
	case Chara::GU:
		if ((chara.next->flag & Chara::TYOKI) == Chara::TYOKI)
		{
			cout << "あなたの勝ちです" << endl;
			return Chara::WIN;
		}
		else
		{
			cout << "あなたの負けです" << endl;
			return Chara::LOSS;
		}
		break;
	case Chara::TYOKI:
		if ((chara.next->flag & Chara::PA) == Chara::PA)
		{
			cout << "あなたの勝ちです" << endl;
			return Chara::WIN;
		}
		else
		{
			cout << "あなたの負けです" << endl;
			return Chara::LOSS;
		}
		break;
	case Chara::PA:
		if ((chara.next->flag & Chara::GU) == Chara::GU)
		{
			cout << "あなたの勝ちです" << endl;
			return Chara::WIN;
		}
		else
		{
			cout << "あなたの負けです" << endl;
			return Chara::LOSS;
		}
		break;
	}
	return Chara::N;
}
unsigned int Momey(int w_or_l)
{
	switch (w_or_l)
	{
	case Chara::WIN:
		return chara.rate*2;
		break;
	case Chara::LOSS:
		return 0;
		break;
	case Chara::Draw:
		return chara.rate;
		break;
	}
	return 0;
}
void GameMain()                         //関数ポインタで行う　マスター更新処理
{
    //じゃんけん
    int value = 0;
    bool rate = false;
    cout << "ゲーム本編" << endl << endl;
    Initial_Parameter_Display(false);
    //入力の処理
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
			Substitution(value, "Chara");//自分で決めたじゃんけんパターンを取得する
            break;
        }
    }
    Initial_Parameter_Display(true);
    
	Substitution(Random(&chara), "Enemy");//相手の乱数からじゃんけんパターンを決める

	Debag_Output("2bit");//デバッグモード

	int win_or_loss = VictoryDecision();//勝敗をジャッジする

	chara.momey += Momey(win_or_loss);//勝敗からお金のやりとりをする

	Debag_Output("お金");
    Game_Scece[ENDING]();
}
void Ending()
{
    cout << "エンディング" << endl << endl;
    free(chara.next);
    chara.next = nullptr;
	int value = 0;
	while (!value)
	{
		if (chara.momey <= 0)
		{
			cout << "ゲーム終了　所持金がなくなりました" << endl;
			chara.flag = Chara::N;
			cout << "ゲームをやり直しますか？" << endl;
		}
		else
		{
			cout << "ゲームを続けますか？" << endl << endl;
		}
		cout << "１　はい　２　いいえ  ";
		cin >> value;
		if (!(value == 1 || value == 2))
		{
			cout << "もう一度行ってください" << endl;
		}
		else
		{
			break;
		}
	}
	if (value == 1)
	{
		Game_Scece[INITIALIZE]();
	}
}
void main()
{
    cout << "ゲーム開始します" << endl << endl;
    Game_Scece[INITIALIZE]();             //初期化処理
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
			str = "ニュートラル";
			break;
		case Chara::GU:
			str = "グー";
			break;
		case Chara::TYOKI:
			str = "チョキ";
			break;
		case Chara::PA:
			str = "パー";
			break;
		}
		cout << "自分のじゃんけんパターン :  " << str << endl;
		switch (chara.next->flag)
		{
		case Chara::N:
			str1 = "ニュートラル";
			break;
		case Chara::GU:
			str1 = "グー";
			break;
		case Chara::TYOKI:
			str1 = "チョキ";
			break;
		case Chara::PA:
			str1 = "パー";
			break;
		}
		cout << "相手のじゃんけんパターン :  " << str1 << endl;
	}
	if (Debag_name == "お金")
	{
		cout << "現在のお金：　"<<(signed)chara.momey << "円" << endl;
	}
	cout << endl;
}
