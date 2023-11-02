/*****************************
*�v���O���~���O���K(�e�g���X��)
*****************************/
#include"DxLib.h"
#include"FreamControl.h"
#include"SceneManager.h"
#include"InputControl.h"

/*****************************
*�}�N����`
*****************************/
#define SCREEN_HEIGHT (720)
#define SCREEN_WIDTH (1280)
#define SCREEN_COLORBIT (32)
#define FONT_SIZE (20)
/*****************************
*�^��`
*****************************/

/*****************************
*�v���g�^�C�v�錾
*****************************/

/*****************************
*�v���O�����̊J�n
*****************************/
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SetMainWindowText("�e�g���X");

	ChangeWindowMode(TRUE);

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	FreamControl_Initialize();

	SceneManager_Initialize(E_TITLE);

	InputControl_Initialize();

	SetDrawScreen(DX_SCREEN_BACK);

	SetFontSize(FONT_SIZE);

	while (ProcessMessage() == 0 && GetExitButton() != TRUE && ErrorCheck() == D_NORMALITY)
	{
		FreamControl_Update();

		ClearDrawScreen();

		InputControl_Update();

		SceneManager_Update();

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}

