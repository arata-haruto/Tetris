#include"Block.h"
#include"DxLib.h"
#include"InputControl.h"

/******************************
*�}�N����`
*******************************/ 
#define FIELD_HEIGHT (21)      //�t�B�[���h�̃}�X�̍���
#define FIELD_WIDTH  (12) //�t�B�[���h�̃}�X��
#define BLOCK_TROUT_SIZE (4)//�u���b�N�̃}�X�T�C�Y
#define BLOCK_SIZE (36)    //1�u���b�N������̃T�C�Y
#define BLOCK_TYPE_MAX (7)//�����Ă���u���b�N�̎�ށ@
#define BLOCK_NEXT_POS_X (700)//���̃u���b�N�̍��W(X���W�j
#define BLOCK_NEXT_POS_Y (500)//���̃u���b�N�̍��W(Y���W�j
#define BLOCK_STOCK_POS_X (500)//�X�g�b�N���ꂽ�u���b�N�̍��W(X���W)
#define BLOCK_STOCK_POS_Y (350)//�X�g�b�N���ꂽ�u���b�N�̍��W(Y���W)
#define DROP_BLOCK_INIT_X (4)//�����Ă���u���b�N�̏���X���W
#define DROP_BLOCK_INIT_Y (-1)//�����Ă���u���b�N�̏���Y���W
#define DROP_SPEED (60)//��������
#define TURN_CROCKWICE (0)//���v����ɉ�]������
#define TURN_ANTICROKWICE (1)//�����v����ɉ�]������

/*****************************
*�^��`
*****************************/
enum BLOCK_STATE
{
	E_BLOCK_EMPTY,       //��u���b�N
	E_BLOCK_LIGHT_BLUE,//���F
	E_BLOCK_YELLOW_GREEEN,//����
	E_BLOCK_YELLOW,//���F
	E_BLOCK_ORANGE,//�I�����W
	E_BLOCK_BLUE,//��
	E_BLOCK_PINK,//�s���N
	E_BLOCK_RED,//��
	E_BLOCK_GLAY,//�D�F
	E_BLOCK_WALL,//��
	E_BLOCK_IMAGE_MAX,
};
/********************************
*�ϐ���`
*********************************/
const int C_BLOCK_TABLE[BLOCK_TYPE_MAX][BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE] = {
	{
		  {0,0,0,0},
		  {0,1,1,0},
		  {0,1,1,0},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {0,0,0,0},
		  {2,2,2,2},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {3,0,0,0},
		  {3,3,3,0},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {0,0,0,4},
		  {0,4,4,4},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {0,5,5,0},
		  {0,0,5,5},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {0,6,6,0},
		  {6,6,0,0},
		  {0,0,0,0}
	},
	{
		  {0,0,0,0},
		  {0,7,0,0},
		  {7,7,7,0},
		  {0,0,0,0}
		  }
};

/****************************
*�ϐ��錾
*****************************/
int BlockImage[E_BLOCK_IMAGE_MAX];//�u���b�N�摜
BLOCK_STATE FieId[FIELD_HEIGHT][FIELD_WIDTH];//�t�B�[���h�z��
BLOCK_STATE Next[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE]; //�ҋ@��Ԃ̃u���b�N
BLOCK_STATE Stock[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE]; //�X�g�b�N�̃u���b�N
BLOCK_STATE DropBlock[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE]; //������u���b�N
int DropBlock_X; //������u���b�N��X���W
int DropBlock_Y;//������u���b�N��Y���W

int WaitTime; //�ҋ@����
int Stock_Flg; //�X�g�b�N�t���O
int Generate_Flg; //���Y�t���O
int DeleteLine; //���������C���̐�
int SoundEffect[3]; //SE

/******************************
*�v���g�^�C�v�錾
*******************************/

void create_field(void);//�t�B�[���h�̐��Y����
void create_block(void);//�u���b�N�̐��Y����
void move_block(void);//�u���b�N�̈ړ�����
void change_block(void);//�X�g�b�N��������
void turn_block(int clockwise);//�u���b�N��]����
int check_overlap(int x, int y);//�͈͊O�`�F�b�N����
void lock_block(int x, int y);//���n�����u���b�N���Œ�ς݂ɕύX���鏈��
void check_line(void);//�u���b�N�̉����m�F����

/******************************
*�u���b�N�@�\�F����������
*�����F�Ȃ�
*�߂�l�F�G���[���i-1�F�ȏ�A����ȊO�F����j
********************************/

int Block_Initialize(void)
{
	int ret = 0;  //�߂�l
	int i = 0;

	//�u���b�摜�̓Ǎ���
	ret = LoadDivGraph("images/block.png", E_BLOCK_IMAGE_MAX, 10, 1,BLOCK_SIZE, BLOCK_SIZE, BlockImage);

	//SE�̓ǂݍ���
	SoundEffect[0] = LoadSoundMem("sounds/SE3.mp3");
	SoundEffect[1] = LoadSoundMem("sounds/SE4.mp3");
	SoundEffect[2] = LoadSoundMem("sounds/SE5.wav");

	//���ʂ̒���
	ChangeVolumeSoundMem(150, SoundEffect[0]);
	ChangeVolumeSoundMem(150, SoundEffect[1]);
	ChangeVolumeSoundMem(130, SoundEffect[2]);

	//�t�B�[���h�̐��Y
	create_field();

	//�u���b�N���Y
	create_block();
	create_block();

	//�ҋ@���Ԃ̏�����
	WaitTime = 0;
	//�X�g�b�N�t���O�̏�����
	Stock_Flg = FALSE;
	//���Y�t���O�̏�����
	Generate_Flg = TRUE;
	//���������C���̐��̏�����
	 DeleteLine = 0;

	//�G���[�`�F�b�N
	for (i = 0; i < 3; i++)
	{
		if (SoundEffect[i] == -1)
		{
			ret = -1;
			break;
		}
	}
	return ret;
}

/*******************************
*�u���b�N�@�\�F�X�V����
*�����F�Ȃ�
*�߂�l�F�Ȃ�
********************************/

void Block_Update(void)
{
	//�u���b�N�̈ړ�����
	move_block();

	//�u���b�N�̃X�g�b�N
	if ((GetButtonDown(XINPUT_BUTTON_LEFT_SHOULDER) == TRUE) ||
		(GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER) == TRUE))
	{
		//���Y�\�ł����
		if (Generate_Flg == TRUE)
		{
			change_block();//�X�g�b�N��������
			//�u���b�N�̉�]�𐳈ʒu�ɂ���
		}
	}
	//�u���b�N�̉�]�i�����v����j
	if ((GetButtonDown(XINPUT_BUTTON_A) == TRUE) ||
		 (GetButtonDown(XINPUT_BUTTON_Y) == TRUE))
	{
        turn_block(TURN_ANTICROKWICE);
	}
	//�u���b�N�̉�]�i���v���j
	if ((GetButtonDown(XINPUT_BUTTON_B) == TRUE) ||
		(GetButtonDown(XINPUT_BUTTON_X) == TRUE))
	{
		turn_block(TURN_CROCKWICE);
	}

	//��������
	WaitTime++;  //�J�E���^�̍X�V
	if (WaitTime > DROP_SPEED)
	{
		if (check_overlap(DropBlock_X, DropBlock_Y + 1) == TRUE)
		{
			DropBlock_Y++;
		}
		else
		{
			//�u���b�N�̌Œ�
			lock_block(DropBlock_X, DropBlock_Y);
			//�u���b�N�̏����ƃu���b�N�����낷����
			check_line();
			//�V�����u���b�N�̐��Y
			create_block();
		}
		//�J�E���^�̏�����
		WaitTime = 0;
	}
}

/**************************
*�u���b�N�@�\�F�`�揈��
*�����F�Ȃ�
*�߂�l�F�Ȃ�
***************************/

void Block_Draw(void)
{
	int i, j;   //���[�v�J�E���^

	//�t�B�[���h�̃u���b�N��`��
	for (i = 0; i < FIELD_HEIGHT; i++)
	{
		for (j = 0; j < FIELD_WIDTH; j++)
		{
			if (FieId[i][j] != E_BLOCK_WALL)
			{
				DrawGraph(j * BLOCK_SIZE, i * BLOCK_SIZE, BlockImage[FieId[i][j]],
				TRUE);
			}
		}

	}
	//���̃u���b�N�ƃX�g�b�N���ꂽ�u���b�N��`��
	for (i = 0; i < BLOCK_TROUT_SIZE; i++)
	{
		for (j = 0; j < BLOCK_TROUT_SIZE; j++)
		{
			//���̃u���b�N��`��
			DrawGraph(BLOCK_SIZE * j + BLOCK_NEXT_POS_X, BLOCK_SIZE * i +
				BLOCK_NEXT_POS_Y, BlockImage[Next[i][j]], TRUE);
			//�X�g�b�N���ꂽ�u���b�N��`��
			DrawGraph(BLOCK_SIZE * j + BLOCK_STOCK_POS_X, BLOCK_SIZE * i +
				BLOCK_STOCK_POS_Y, BlockImage[Stock[i][j]], TRUE);
		}
	}
	//�����Ă���u���b�N�̕`��
	for (i = 0; i < BLOCK_TROUT_SIZE; i++)
	{
		for (j = 0; j < BLOCK_TROUT_SIZE; j++)
		{
			DrawGraph((DropBlock_X + j) * BLOCK_SIZE, (DropBlock_Y + i) * BLOCK_SIZE,
				BlockImage[DropBlock[i][j]], TRUE);
		}
	}
}

/**************************
*�u���b�N�@�\�F�u���b�N�̐��Y���菈��
*�����F�Ȃ�
*�߂�l�FTRUE�i�u���b�N�̐��Y���ł���j�AFALSE�i���Y�s�j
***************************/
int Get_GenerateFlg(void)
{
	return Generate_Flg;
}

/***************************
*�u���b�N�@�\�F���������C���̐��擾����
*�����F�Ȃ�
*�߂�l�F���������C���̐�
*****************************/
int Get_Line(void)
{
	return DeleteLine;
}

/*****************************
*�u���b�N�@�\�F�t�B�[���h���Y����
*�����F�Ȃ�
*�߂�l�F�Ȃ�
*******************************/

void create_field(void)
{
	int i, j; //���[�v�J�E���^

	//�t�B�[���h�̐��Y
	for (i	= 0; i < FIELD_HEIGHT; i++)
	{
		for (j = 0; j < FIELD_WIDTH; j++)
		{
			//�t�B�[���h�l�̐ݒ�
			if (j == 0 || j == FIELD_WIDTH - 1 || i == FIELD_HEIGHT - 1)
			{
				FieId[i][j] = E_BLOCK_WALL; //�Ǐ�Ԃɂ���
			}
			else
			{
				FieId[i][j] = E_BLOCK_EMPTY; //���Ԃ̂���
			}
		}
	}
}

/*************************
*�u���b�N�@�\�F�u���b�N���Y����
*�����F�Ȃ�
*�߂�l�F�Ȃ�
**************************/

void create_block(void)
{
	int i, j; //���[�v�J�E���^
	int block_type; //���ɏo��������u���b�N�^�C�v

	//���ɏo��������u���b�N�̌��肷��
	block_type = GetRand(BLOCK_TYPE_MAX - 1);

	//�V�����u���b�N���Z�b�g�����̃u���b�N�𐶎Y
	for (i = 0; i < BLOCK_TROUT_SIZE; i++)
	{
		for (j = 0; j < BLOCK_TROUT_SIZE; j++)
		{
			DropBlock[i][j] = Next[i][j];
			Next[i][j] = (BLOCK_STATE)C_BLOCK_TABLE[block_type][i][j];
		}
	}

	//�o���ʒu�̐ݒ�
	DropBlock_X = DROP_BLOCK_INIT_X;
	DropBlock_Y = DROP_BLOCK_INIT_Y;

	//���Y�ł��Ȃ��������A�Q�[���I�[�o�[�ɓ]�ڂ���
	if (check_overlap(DropBlock_X, DropBlock_Y) == FALSE)
	{
		Generate_Flg = FALSE;
	}
}

/************************
*�u���b�N�@�\�F�u���b�N�̈ړ�����
*�����F�Ȃ�
*�߂�l�F�Ȃ�
**************************/

void move_block(void)
{
	//�����͎�
	if (GetButtonDown(XINPUT_BUTTON_DPAD_LEFT))
	{
		if (check_overlap(DropBlock_X - 1, DropBlock_Y) == TRUE)
		{
			DropBlock_X--;
		}
	}

	//�E���͎�
	if (GetButtonDown(XINPUT_BUTTON_DPAD_RIGHT))
	{
		if (check_overlap(DropBlock_X + 1, DropBlock_Y) == TRUE)
		{
			DropBlock_X++;
		}
	}

	//����͎��i�n�[�h�h���b�v�����j
	if (GetButtonDown(XINPUT_BUTTON_DPAD_UP))
	{
		while (check_overlap(DropBlock_X, DropBlock_Y + 1) == TRUE)
		{
			DropBlock_Y++;
		}
	}

	//�����͎��i�\�t�g�h���b�v�����j
	if (GetButton(XINPUT_BUTTON_DPAD_DOWN))
	{
		if (check_overlap(DropBlock_X, DropBlock_Y + 1) == TRUE)
		{
			DropBlock_Y++;
		}
	}
}

/*******************************
*�u���b�N�@�\�F�X�g�b�N��������
*�����F�Ȃ�
*�߂�l�F�Ȃ�
********************************/

void change_block(void)
{
	BLOCK_STATE temp[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE] = { E_BLOCK_EMPTY }; //�ޔ�̈�
	int i, j;  ///���[�v�J�E���^

	//�X�g�b�N�悪�󂩂ǂ����m�F
	if (Stock_Flg == TRUE)
	{
		for (i = 0; i < BLOCK_TROUT_SIZE; i++)
		{
			for (j = 0; j < BLOCK_TROUT_SIZE; j++)
			{
				temp[i][j] = DropBlock[i][j];
				DropBlock[i][j] = Stock[i][j];
				Stock[i][j] = temp[i][j];

			}

		}
	}
	else
	{
		Stock_Flg = TRUE;
		for (i = 0; i < BLOCK_TROUT_SIZE; i++)
		{
			for (j = 0; j < BLOCK_TROUT_SIZE; j++)
			{
				Stock[i][j] = DropBlock[i][j];
			}
		}
		//�V�����u���b�N�̐ݒ�Ǝ��̃u���b�N�̐��Y
		create_block();
	}
}

/*******************************
*�u���b�N�@�\�F�u���b�N�̌�������
*�����F��]����������i�O�F�����v���1�Fj�����v����j
*�߂�l�F�Ȃ�
********************************/

void turn_block(int clockwise)
{
	BLOCK_STATE temp[BLOCK_TROUT_SIZE][BLOCK_TROUT_SIZE] = {E_BLOCK_EMPTY}; //�ޔ�̈�
	int i, j;//���[�v�J�E���^

	do
	{
		if (clockwise == TURN_CROCKWICE)
		{
			//�u���b�N���ꎞ�ێ�����
			for (i = 0;i < BLOCK_TROUT_SIZE; i++)
			{
				for (j = 0; j < BLOCK_TROUT_SIZE; j++)
				{
					temp[j][3-i] = DropBlock[i][j];
				}
			}
		}
		else
		{
			//�u���b�N���ꎞ�ێ�����
			for (i = 0; i < BLOCK_TROUT_SIZE; i++)
			{
				for (j = 0; j < BLOCK_TROUT_SIZE; j++)
				{
					temp[3 - j][i] = DropBlock[i][j];
				}
			}
		}

		//�u���b�N��]
		for (i = 0; i < BLOCK_TROUT_SIZE; i++)
		{
			for (j = 0; j<BLOCK_TROUT_SIZE; j++)
			{
				DropBlock[i][j] = temp[i][j];
			}
		}


		//�Ǒ��̕␳����
		if (check_overlap(DropBlock_X, DropBlock_Y) && DropBlock_X >= E_BLOCK_WALL)
		{
			DropBlock_X--;
		}
		if (check_overlap(DropBlock_X, DropBlock_Y) && DropBlock_X<=E_BLOCK_EMPTY)
		{
			DropBlock_X++;
		}

	}while(check_overlap(DropBlock_X, DropBlock_Y) == FALSE);

	PlaySoundMem(SoundEffect[2], DX_PLAYTYPE_BACK, TRUE);
}

/************************
*�u���b�N�@�\�F�͈͊O�`�F�b�N����
*�����F�����u���b�N�̍��W�i���A���j
*�߂�l�FTRUE(�͈͓��j,FALSE�i�͈͊O�j
*************************/

int check_overlap(int x, int y)
{
	int i, j;   //���[�v�J�E���^

    for (i = 0; i < BLOCK_TROUT_SIZE; i++)
	{
		for (j = 0; j < BLOCK_TROUT_SIZE; j++)
		{
			if (DropBlock[i][j] != E_BLOCK_EMPTY)
			{
				if (FieId[i + y][j + x] != E_BLOCK_EMPTY)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/*************************
*�u���b�N�@�\�F���n�����u���b�N���Œ�ς݂ɂ��鏈��
*�����F�����u���b�N�̍��W�i���A���j
*�߂�l�F�Ȃ�
**************************/

void lock_block(int x,int y)
{
	int i, j;  //���[�v�J�E���^

	for (i = 0; i < BLOCK_TROUT_SIZE; i++)
	{
		for (j = 0; j < BLOCK_TROUT_SIZE; j++)
		{
			if (DropBlock[i][j] != E_BLOCK_EMPTY)
			{
				FieId[y + i][x + j] = DropBlock[i][j];
			}
		}
	}
	PlaySoundMem(SoundEffect[1], DX_PLAYTYPE_BACK,TRUE);
}

/*****************************
*�u���b�N�@�\�F�u���b�N�̉����m�F����
*�����F�Ȃ�
*�߂�l�F�Ȃ�
*****************************/

void check_line(void)
{
	int i, j, k; //���[�v�J�E���^

	for (i = 0; i < FIELD_HEIGHT - 1; i++)
	{
		for (j = 1; j < FIELD_WIDTH; j++)
		{
			//�s�̓r�����󂢂Ă邩�H
			if (FieId[i][j] == E_BLOCK_EMPTY)
			{
				break;
			}
		}


		//��񑵂��Ă�����A�J�E���g�𑝂₵�A��i������
		if (j >= FIELD_WIDTH)
		{
			//�J�E���^�𑝉�
			DeleteLine++;

			//��i������
			for (k = i; k > 0; k--)
			{
				for (j = 1; j < FIELD_WIDTH; j++)
				{
					FieId[k][j] = FieId[k - 1][j];
				}
			}
			PlaySoundMem(SoundEffect[0], DX_PLAYTYPE_BACK, TRUE);
		}
	}
}




