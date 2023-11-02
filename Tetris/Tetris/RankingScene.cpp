#include "RankingScene.h"
#include "DxLib.h"
#include "InputControl.h"
#include "SceneManager.h"

/*****************************
*マクロ定義
*****************************/
#define RANKING_FILE     ("dat/rankingdate.csv")
#define RANKING_MAX      (10)
#define RANKING_NAME_LEN (11)
/*****************************
*型定義
*****************************/
typedef struct
{
	int rank;                    //ランク
	char name[RANKING_NAME_LEN]; //名前
	int score;                   //スコア
}T_RANKING;

typedef struct
{
	int x;
	int y;
}T_CURSOR;
/*****************************
*グローバル変数宣言
*****************************/
T_RANKING Ranking_Data[RANKING_MAX];   //ランキングデータ
T_RANKING New_Score;                   //新しいスコアデータ
int DispMode;                          //表示モード

T_CURSOR Cursor;                       //カーソル用変数
int name_num;
/*****************************
*プロトタイプ宣言
*****************************/
void file_read(void);         //ファイル読み込み
void file_write(void);        //ファイル書き込み
void ranking_sort(void);      //ランキングソート処理
void ranking_input_name(void); //名前入力処理
void ranking_input_name_draw(void);//名前入力描画処理

/*****************************
*ランキング画面：初期化処理
* 引数：なし
* 戻り値：エラー情報（－1：以上有、－1以外：正常終了
*****************************/
int RankingScene_Initialize(void)
{
	int ret = 0;
	file_read();

	switch (DispMode)
	{
	case RANKING_INPUT_MODE:
		Cursor.x = 0;
		Cursor.y = 0;
		name_num = 0;
		break;
	case RANKING_DISP_MODE:
	default:
		break;
	}
	return ret;
}

/*****************************
*ランキング画面：更新処理
* 引数：なし
* 戻り値：なし
*****************************/
void RankingScene_Update(void)
{
	switch (DispMode)
	{
	case RANKING_INPUT_MODE:
		ranking_input_name();
		break;
	case RANKING_DISP_MODE:
	default:
		if (GetButtonDown(XINPUT_BUTTON_B))
		{
			Change_Scene(E_TITLE);
		}
		break;
	}
}
/*****************************
*変数定義
*****************************/

/*****************************
*変数定義
*****************************/