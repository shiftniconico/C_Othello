/*
 * 【今後の追加したい機能】
 * 置ける箇所をマーキングする機能(ver0.2で実装済)
 * 単純なAI機能(ver0.3で実装済)
 * リアルタイムでのコマの個数表示(ver0.3で実装済)
 * CPUが置いたコマを表示(ver0.4で実装済)
 * 1P,2P対戦実装(ver0.5実装済)
 * ビープ音実装 1P -> 自分のターンで鳴らす。 2P -> ターン交互に鳴らす。(ver0.6実装済)
 * sleep()の実装(ver0.7実装予定)
 * コンパイル時の警告解決(ver0.8実装済)
 *
 * 【othello ver0.8】
 *
 * */

#include <stdio.h>

// 盤面の大きさ
#define BOARDSIZE	8

// 状態を定期
#define NONE		0
#define BLACK		1
#define WHITE		2
#define MARK		3

// グローバル変数
char board[BOARDSIZE][BOARDSIZE];	// 盤面
int turn = 0, ai_x = -1, ai_y = -1;
FILE *file;
int play_num = 0;

// 向き毎の移動量
int vec_y[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int vec_x[] = {0, 1, 1, 1, 0, -1, -1, -1};

// プロトタイプ宣言
void setBoard(void);				// 初期化関数
void disp(void);					// 盤面表示関数
void check_cnt(void);				// 黒のコマと白のコマをカウントする関数
int check_flip(int y, int x, int turn, int vec);	// vecで指定された向きについてひっくり返るコマがあるか確認する
int check(int y, int x, int turn);	// その場所に置くことが出来るかを確認する関数
void input(int turn);				// 入力関数
void flip(int y, int x, int turn, int vec);	// 実際に裏返す関数
int put(int y, int x, int turn);	// 入力を受けて裏返せるか確かめる関数
int check_end(int turn);			// 終了判定
void check_winner(void);				// 勝者判定
void marking(void);					// 置けるマスが在るかチェックする処理
int marking_put(int i, int j, int turn); // ひっくり返せるますが在るかチェックする関数
void death_marking(void);			// マーキングポインタ初期化
void ai_rand(int turn);				// 人工無能
int init_view(void);				// 初期描画関数
void two_player_circle_marking(int y, int x, int turn);	// 2P対戦の時互いが置いたコマをマーキング
void death_player_circle_marking(void);					// マーキングを削除

int main(void)
{
	// 初期画面描画
	while(1)
	{
		init_view();
		if (play_num == 1 || play_num == 2)
		{
			play_num = 0;
			break;
		}
	}

	// 盤面の初期化
	setBoard();
	
	// 乱数の初期化
	srand(time(NULL));

	// ゲームのメインループ
	while(turn < 2)
	{
		if (play_num == 1)		// 1P Player 選択時ループ処理
		{
			
			// どちらの手番か表示
			if (turn == 0)
			{
				printf("turn:●\n");
			}
			else
			{
				printf("turn:○\n");
			}
			
			// 現在の両者のコマ数を表示
			check_cnt();

			// 置けるマスが在るかチェックする処理
			marking();

			// 盤面表示
			disp();

			// 入力処理
			switch(turn)
			{
				case 0: printf("\a"); input(turn); break;
				case 1: ai_rand(turn); sleep(3); break;
				default:printf("error\n");
				return -1;
			}
		}
		// 2P Player 選択時ループ処理
		else						
		{
			// どちらの手番か表示
			if (turn == 0)
			{
				printf("turn:●\n");
			}
			else
			{
				printf("turn:○\n");
			}
			printf("\a");
			
			// 現在の両者のコマ数を表示
			check_cnt();

			// 置けるマスが在るかチェックする処理
			marking();

			// 盤面表示
			disp();

			// 入力処理
			input(turn);
		}

		// マーキングポインタを削除
		death_marking();

		// 相手が置いたコマのマーキングを削除
		// death_player_circle_marking();

		// 手番交代
		turn = (turn + 1) % 2;

		// 終了判定
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

// 初期画面描画関数
int init_view(void)
{
	printf("■■■■■■■■■■■■■■■■\n");
	printf("      オセロ Version : 0.7\n");
	printf("■■■■■■■■■■■■■■■■\n\n\n");
	printf("1 : 1P Play\n");
	printf("2 : 2P Play\n\n\n");
	printf("数字を入力してください。\n");
	printf(">");

	if (scanf("%d", &play_num) == 0)
	{
		// 数値が取れなかったらバッファをクリアする
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

// 初期化関数
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

// 盤面表示関数
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
				case NONE:  printf("・"); break;
				case BLACK: if (play_num != 1)
							{
								if (i == ai_y && j == ai_x)
								{
									printf("◎");
								}
								else
								{
									printf("●");
								}
							}
							else
							{
								printf("●");
							}
							break;
				case WHITE:	if (i == ai_y && j == ai_x) printf("◎"); else printf("○"); break;
				case MARK:  printf("＊"); break;
				default:	printf("er"); break;
			}
		}
		printf("\n");
	}
}

int check(int y, int x, int turn)
{
	
	int vec;

	// どれか一方向でもひっくり返るか確認
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

		// 盤面の外に出ていたら終了
		// 空きマスだったら終了
		// マーキングポインタなら終了
		if (x < 0 || y < 0 || x > BOARDSIZE - 1|| y > BOARDSIZE - 1 || board[y][x] == NONE || board[y][x] == MARK)
		{
			return 0;
		}

		// 相手のコマがあったらフラグを立てる
		if (board[y][x] == (turn ? BLACK : WHITE))
		{
			flag = 1;
			continue;
		}

		// もしフラグがたっていればループ脱出。いなければ終了。
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
		// 入力する
		printf(">");

		if (scanf("%d", &place) == 0)
		{
			// 数値が取れなかったらバッファをクリアする
			scanf("%*[^\n]%+c");
			printf("input error\n");
			continue;
		}

		// 数値が範囲内か判定
		if (place < 11 || place > 88)
		{
			printf("input[%d]:error\n", place);
			place = 0;
			continue;
		}

		y = place / 10;
		x = place % 10;

		// もう少し詳しく確認
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
		
		// 自分のコマもしくはマーキングポインタがあったら終了
		if (board[y][x] == (turn ? WHITE : BLACK) || board[y][x] == MARK)
		{
			break;
		}

		// それ以外なら自分のコマを塗りつぶす
		board[y][x] = (turn ? WHITE : BLACK);
	}
}

int put(int y, int x, int turn)
{
	
	int vec, flag = 0;

	// 空白かつマーキングポインタでなければ終了
	if (board[y][x] != NONE && board[y][x] != MARK)
	{
		return 0;
	}

	// 全方向について確認
	for (vec = 0; vec < 8; ++vec)
	{
		if (check_flip(y, x, turn, vec) == 1)
		{
			// 裏返す
			flip(y, x, turn, vec);
			flag = 1;
		}
	}

	if (flag == 1)
	{
		// この場所にコマを置く
		board[y][x] = (turn ? WHITE : BLACK);
		return 1;
	}


	return 0;
}

int marking_put(int y, int x, int turn)
{
	// 
	int vec, flag = 0;

	// 空白でなければ終了
	if (board[y][x] != NONE)
	{
		return 0;
	}

	// 全方向について確認
	
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

	// 置ける場所が在るか確認
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			// あれば普通に実行
			if (board[i][j] == NONE && check(i ,j, turn) == 1)
			{
				return 0;
			}
		}
	}

	// 場所がなかったので交代して探す
	turn = (turn + 1) % 2;
	for (i = 0; i < BOARDSIZE; ++i)
	{
		for (j = 0; j < BOARDSIZE; ++j)
		{
			// あればpassして続行
			if (board[i][j] == NONE && check(i, j, turn) == 1)
			{
				return 1;
			}
		}
	}
	// なかったので終了
	return 2;
}

void check_winner()
{
	
	int i, j, b = 0, w = 0;

	// コマを数え上げる
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

	// 最後に盤面表示
	disp();

	// 勝者を表示
	if (b > w)
	{
		printf("● : Winner BLACK!\n");
	}
	else if (b < w)
	{
		printf("○ : Winner WHITE!\n");
	}
	else
	{
		printf("Drawn Game.");
	}
}

// 置けるマスが在るかチェックする処理
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

// マーキングポインタ初期化
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
		// 適当に決める
		place = rand() % 89;

		// 数値が範囲内か確認
		if (place < 11 || place > 88)
		{
			place = 0;
			continue;
		}

		y = place / 10;
		x = place % 10;

		// もう少し詳しく確認
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

	// コマを数え上げる
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
