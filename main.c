/*
 * �y����̒ǉ��������@�\�z
 * �u����ӏ����}�[�L���O����@�\(ver0.2�Ŏ�����)
 * �P����AI�@�\(ver0.3�Ŏ�����)
 * ���A���^�C���ł̃R�}�̌��\��(ver0.3�Ŏ�����)
 * CPU���u�����R�}��\��(ver0.4�Ŏ�����)
 * 1P,2P�ΐ����(ver0.5������)
 * �r�[�v������ 1P -> �����̃^�[���Ŗ炷�B 2P -> �^�[�����݂ɖ炷�B(ver0.6������)
 * sleep()�̎���(ver0.7�����\��)
 * �R���p�C�����̌x������(ver0.8������)
 *
 * �yothello ver0.8�z
 *
 * */

#include <stdio.h>

// �Ֆʂ̑傫��
#define BOARDSIZE	8

// ��Ԃ���
#define NONE		0
#define BLACK		1
#define WHITE		2
#define MARK		3

// �O���[�o���ϐ�
char board[BOARDSIZE][BOARDSIZE];	// �Ֆ�
int turn = 0, ai_x = -1, ai_y = -1;
FILE *file;
int play_num = 0;

// �������̈ړ���
int vec_y[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int vec_x[] = {0, 1, 1, 1, 0, -1, -1, -1};

// �v���g�^�C�v�錾
void setBoard(void);				// �������֐�
void disp(void);					// �Ֆʕ\���֐�
void check_cnt(void);				// ���̃R�}�Ɣ��̃R�}���J�E���g����֐�
int check_flip(int y, int x, int turn, int vec);	// vec�Ŏw�肳�ꂽ�����ɂ��ĂЂ�����Ԃ�R�}�����邩�m�F����
int check(int y, int x, int turn);	// ���̏ꏊ�ɒu�����Ƃ��o���邩���m�F����֐�
void input(int turn);				// ���͊֐�
void flip(int y, int x, int turn, int vec);	// ���ۂɗ��Ԃ��֐�
int put(int y, int x, int turn);	// ���͂��󂯂ė��Ԃ��邩�m���߂�֐�
int check_end(int turn);			// �I������
void check_winner(void);				// ���Ҕ���
void marking(void);					// �u����}�X���݂邩�`�F�b�N���鏈��
int marking_put(int i, int j, int turn); // �Ђ�����Ԃ���܂����݂邩�`�F�b�N����֐�
void death_marking(void);			// �}�[�L���O�|�C���^������
void ai_rand(int turn);				// �l�H���\
int init_view(void);				// �����`��֐�
void two_player_circle_marking(int y, int x, int turn);	// 2P�ΐ�̎��݂����u�����R�}���}�[�L���O
void death_player_circle_marking(void);					// �}�[�L���O���폜

int main(void)
{
	// ������ʕ`��
	while(1)
	{
		init_view();
		if (play_num == 1 || play_num == 2)
		{
			play_num = 0;
			break;
		}
	}

	// �Ֆʂ̏�����
	setBoard();
	
	// �����̏�����
	srand(time(NULL));

	// �Q�[���̃��C�����[�v
	while(turn < 2)
	{
		if (play_num == 1)		// 1P Player �I�������[�v����
		{
			
			// �ǂ���̎�Ԃ��\��
			if (turn == 0)
			{
				printf("turn:��\n");
			}
			else
			{
				printf("turn:��\n");
			}
			
			// ���݂̗��҂̃R�}����\��
			check_cnt();

			// �u����}�X���݂邩�`�F�b�N���鏈��
			marking();

			// �Ֆʕ\��
			disp();

			// ���͏���
			switch(turn)
			{
				case 0: printf("\a"); input(turn); break;
				case 1: ai_rand(turn); sleep(3); break;
				default:printf("error\n");
				return -1;
			}
		}
		// 2P Player �I�������[�v����
		else						
		{
			// �ǂ���̎�Ԃ��\��
			if (turn == 0)
			{
				printf("turn:��\n");
			}
			else
			{
				printf("turn:��\n");
			}
			printf("\a");
			
			// ���݂̗��҂̃R�}����\��
			check_cnt();

			// �u����}�X���݂邩�`�F�b�N���鏈��
			marking();

			// �Ֆʕ\��
			disp();

			// ���͏���
			input(turn);
		}

		// �}�[�L���O�|�C���^���폜
		death_marking();

		// ���肪�u�����R�}�̃}�[�L���O���폜
		// death_player_circle_marking();

		// ��Ԍ��
		turn = (turn + 1) % 2;

		// �I������
		switch(check_end(turn))
		{
			case 1: printf("pass\n"); turn = (turn + 1) % 2; break;
			case 2: printf("game end\n"); turn = 2; break;
			default:break;
		}
		system("cls");
	}


	check_winner();
	
	return 0;
}

// ������ʕ`��֐�
int init_view(void)
{
	printf("��������������������������������\n");
	printf("      �I�Z�� Version : 0.7\n");
	printf("��������������������������������\n\n\n");
	printf("1 : 1P Play\n");
	printf("2 : 2P Play\n\n\n");
	printf("��������͂��Ă��������B\n");
	printf(">");

	if (scanf("%d", &play_num) == 0)
	{
		// ���l�����Ȃ�������o�b�t�@���N���A����
		scanf("%*[^\n]%+c");
		printf("input error\n");
		system("cls");
		return 0;
	}

	if (play_num == 1)
	{
		system("cls");
		return 1;
	}
	else if (play_num == 2)
	{
		system("cls");
		return 2;
	}
	else
	{
		printf("input error\n");
		system("cls");
		return 0;
	}
}

// �������֐�
void setBoard(void)
{
	
	int i;

	for (i = 0; i < BOARDSIZE * BOARDSIZE; ++i)
	{
		board[i / BOARDSIZE][i % BOARDSIZE] = NONE;
	}

	board[BOARDSIZE / 2 - 1][BOARDSIZE / 2] = BLACK;	// [3][4]
	board[BOARDSIZE / 2][BOARDSIZE / 2 - 1] = BLACK;	// [4][3]
	board[BOARDSIZE / 2][BOARDSIZE / 2] = WHITE;		// [4][4]
	board[BOARDSIZE / 2 - 1][BOARDSIZE / 2 - 1] = WHITE;	// [3][3]
}

// �Ֆʕ\���֐�
void disp(void)
{
	
	int i, j;

	printf(" ");
	for (i = 0; i < BOARDSIZE; ++i)
	{
		printf("%2d", i + 1);
	}
	printf("\n");

	for (i = 0; i < BOARDSIZE; ++i)
	{
		printf("%2d", (i + 1) * 10);
		for (j = 0; j < BOARDSIZE; ++j)
		{
			switch(board[i][j])
			{
				case NONE:  printf("�E"); break;
				case BLACK: if (play_num != 1)
							{
								if (i == ai_y && j == ai_x)
								{
									printf("��");
								}
								else
								{
									printf("��");
								}
							}
							else
							{
								printf("��");
							}
							break;
				case WHITE:	if (i == ai_y && j == ai_x) printf("��"); else printf("��"); break;
				case MARK:  printf("��"); break;
				default:	printf("er"); break;
			}
		}
		printf("\n");
	}
}

int check(int y, int x, int turn)
{
	
	int vec;

	// �ǂꂩ������ł��Ђ�����Ԃ邩�m�F
	for (vec = 0; vec < 8; ++vec)
	{
		if (check_flip(y, x, turn, vec) == 1)
		{
			return 1;
		}
	}
	return 0;
}

int check_flip(int y, int x, int turn, int vec)
{
	// 
	int flag = 0;
	while(1)
	{
		y += vec_y[vec];
		x += vec_x[vec];

		// �Ֆʂ̊O�ɏo�Ă�����I��
		// �󂫃}�X��������I��
		// �}�[�L���O�|�C���^�Ȃ�I��
		if (x < 0 || y < 0 || x > BOARDSIZE - 1|| y > BOARDSIZE - 1 || board[y][x] == NONE || board[y][x] == MARK)
		{
			return 0;
		}

		// ����̃R�}����������t���O�𗧂Ă�
		if (board[y][x] == (turn ? BLACK : WHITE))
		{
			flag = 1;
			continue;
		}

		// �����t���O�������Ă���΃��[�v�E�o�B���Ȃ���ΏI���B
		if (flag == 1)
		{
			break;
		}
		return 0;
	}
	return 1;
}

void input(int turn)
{
	
	int place = 0, y, x;
	while(1)
	{
		// ���͂���
		printf(">");

		if (scanf("%d", &place) == 0)
		{
			// ���l�����Ȃ�������o�b�t�@���N���A����
			scanf("%*[^\n]%+c");
			printf("input error\n");
			continue;
		}

		// ���l���͈͓�������
		if (place < 11 || place > 88)
		{
			printf("input[%d]:error\n", place);
			place = 0;
			continue;
		}

		y = place / 10;
		x = place % 10;

		// ���������ڂ����m�F
		if (x < 1 || y < 1 || x > 8 || y > 8)
		{
			printf("input[%d]:error/n", place);
			place = 0;
			continue;
		}

		if (put(y - 1, x - 1, turn) == 1)
		{
			two_player_circle_marking(y, x, turn);
			break;
		}
		else
		{
			printf("input[%d]:can't flip\n", place);
		}

		place = 0;
	}
}

void flip(int y, int x, int turn, int vec)
{
	while(1)
	{
		y += vec_y[vec];
		x += vec_x[vec];
		
		// �����̃R�}�������̓}�[�L���O�|�C���^����������I��
		if (board[y][x] == (turn ? WHITE : BLACK) || board[y][x] == MARK)
		{
			break;
		}

		// ����ȊO�Ȃ玩���̃R�}��h��Ԃ�
		board[y][x] = (turn ? WHITE : BLACK);
	}
}

int put(int y, int x, int turn)
{
	
	int vec, flag = 0;

	// �󔒂��}�[�L���O�|�C���^�łȂ���ΏI��
	if (board[y][x] != NONE && board[y][x] != MARK)
	{
		return 0;
	}

	// �S�����ɂ��Ċm�F
	for (vec = 0; vec < 8; ++vec)
	{
		if (check_flip(y, x, turn, vec) == 1)
		{
			// ���Ԃ�
			flip(y, x, turn, vec);
			flag = 1;
		}
	}

	if (flag == 1)
	{
		// ���̏ꏊ�ɃR�}��u��
		board[y][x] = (turn ? WHITE : BLACK);
		return 1;
	}


	return 0;
}

int marking_put(int y, int x, int turn)
{
	// 
	int vec, flag = 0;

	// �󔒂łȂ���ΏI��
	if (board[y][x] != NONE)
	{
		return 0;
	}

	// �S�����ɂ��Ċm�F
	
	for (vec = 0; vec < 8; ++vec)
	{
		if (check_flip(y, x, turn, vec) == 1)
		{
			flag = 1;
		}
	}
	
	if (flag == 1)
	{
		return 1;
	}

	return 0;
}

int check_end(int turn)
{
	
	int i, j;

	// �u����ꏊ���݂邩�m�F
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			// ����Ε��ʂɎ��s
			if (board[i][j] == NONE && check(i ,j, turn) == 1)
			{
				return 0;
			}
		}
	}

	// �ꏊ���Ȃ������̂Ō�サ�ĒT��
	turn = (turn + 1) % 2;
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			// �����pass���đ��s
			if (board[i][j] == NONE && check(i, j, turn) == 1)
			{
				return 1;
			}
		}
	}
	// �Ȃ������̂ŏI��
	return 2;
}

void check_winner()
{
	
	int i, j, b = 0, w = 0;

	// �R�}�𐔂��グ��
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			switch(board[i][j])
			{
				case BLACK: ++b; break;
				case WHITE: ++w; break;
				default:	break;
			}
		}
	}

	// �Ō�ɔՖʕ\��
	disp();

	// ���҂�\��
	if (b > w)
	{
		printf("�� : Winner BLACK!\n");
	}
	else if (b < w)
	{
		printf("�� : Winner WHITE!\n");
	}
	else
	{
		printf("Drawn Game.");
	}
}

// �u����}�X���݂邩�`�F�b�N���鏈��
void marking(void)
{
	
	int i, j;
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			if (marking_put(i, j, turn) == 1)
			{
				board[i][j] = MARK;
			}
		}
	}
}

// �}�[�L���O�|�C���^������
void death_marking(void)
{
	
	int i, j;
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			if (board[i][j] == MARK)
			{
				board[i][j] = NONE;
			}
		}
	}
}

void ai_rand(int turn)
{
	int place = 0, y, x;
	while (1)
	{
		// �K���Ɍ��߂�
		place = rand() % 89;

		// ���l���͈͓����m�F
		if (place < 11 || place > 88)
		{
			place = 0;
			continue;
		}

		y = place / 10;
		x = place % 10;

		// ���������ڂ����m�F
		if (x < 1 || y < 1 || x > 8 || y > 8)
		{
			place = 0;
			continue;
		}

		if (put(y - 1, x - 1, turn) == 1)
		{
			printf(">%d\n", place);
			ai_y = y - 1;
			ai_x = x - 1;
			break;
		}

		place = 0;
	}
}

void two_player_circle_marking(int y, int x, int turn)
{
	ai_y = y - 1;
	ai_x = x - 1;
}

void death_player_circle_marking(void)
{
	ai_y = 0;
	ai_x = 0;
}

void check_cnt(void)
{
	int i, j, b = 0, w = 0;

	// �R�}�𐔂��グ��
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			switch(board[i][j])
			{
				case BLACK: ++b; break;
				case WHITE: ++w; break;
				default:	break;
			}
		}
	}

	printf("1P : %2d", b);
	if (play_num == 1)
	{
		printf("  CPU : %2d\n", w);
	}
	else if (play_num == 2)
	{
		printf("  2P : %2d\n", w);
	}
}
