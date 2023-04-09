#define _CRT_SECURE_NO_WARNINGS

/*定义头文件*/
#include <stdio.h>									// 标准输入输出函数库
#include <conio.h>									// 为了读取方向键
#include <Windows.h>								// 控制dos界面（获取控制台上坐标位置，设置字体颜色）
#include <math.h>									// pow函数
#include <time.h>

/*定义全局变量*/
int step = 0;										// 已执行步数
int score = 0;										// 存储游戏分数
long int Time;										// 游戏运行时间
int BOX[4][4] = { 0,0,0,0,							// 游戏中的16个格子
				0,0,0,0,
				0,0,0,0,
				0,0,0,0 };
HANDLE hOut;										// 控制台句柄

/*函数声明*/
void gotoxy(int x, int y);							// 获取屏幕光标位置
int color(int c);									// 设置文字颜色
int TextColors(int i);								// 根据数字修改颜色
void drawTheGameBox();								// 绘制游戏界面
int* add(int item[]);								// 合并数字
int ifMove(int item[]);								// 判断是否能够移动数字，1-能动，0-不能动
void Gameplay();									// 开始游戏
void Replay();										// 重新游戏
int if2n(int x);									// 判断x是否是2的n次方
// 判断是否能够上移，可以上移（格子中的两个数相加是2的n次方），则返回1；不能移动，则返回0
int ifup();
// 判断是否能够下移，可以下移（格子中的两个数相加是2的n次方），则返回1；不能移动，则返回0
int ifdown();
// 判断是否能够左移，可以左移（格子中的两个数相加是2的n次方），则返回1；不能移动，则返回0
int ifleft();
// 判断是否能够右移，可以右移（格子中的两个数相加是2的n次方），则返回1；不能移动，则返回0
int ifright();
int BOXmax();										// 返回棋盘最大数
int Gamefaile();									// 判断是否失败
int Gamewin();										// 判断是否胜利
int keyboardControl(int key);						// 键盘控制移动
void close();										// 关闭游戏
void title();										// 绘制标题
void choice();										// 选择框
void Rabbit();										// 绘制字符画
void regulation();									// 游戏规则介绍
void explation();									// 按键说明

/*
	获取屏幕光标位置
*/
void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);			// 定位光标位置
}

/*
	文字颜色函数
*/
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);			// 更改文字颜色
	return 0;
}

/*
	设置标题
*/
void title()
{
	color(11);
	gotoxy(19, 2);
	printf("■■■      ■■■■      ■ ■      ■■■■");
	gotoxy(19, 3);
	printf("  ■      ■  ■      ■ ■      ■  ■");
	gotoxy(19, 4);
	printf("■■■      ■  ■      ■ ■      ■■■■");
	gotoxy(19, 5);
	printf("■        ■  ■      ■■■■     ■  ■");
	gotoxy(19, 6);
	printf("■■■      ■■■■        ■      ■■■■");
}

/*
	菜单选项
*/
void choice()
{
	int n;
	int i, j = 1;
	gotoxy(29, 8);
	color(13);
	printf("挑  战  2  0  4  8");
	color(14);														// 黄色边框
	for (i = 9; i <= 20; i++)										// 输出上下边框===
	{
		for (j = 15; j <= 60; j++)									// 输出左右边框||
		{
			gotoxy(j, i);
			if (i == 9 || i == 20) printf("=");
			else if (j == 15 || j == 59) printf("||");
		}
	}

	color(12);														// 红色字体
	gotoxy(25, 12);													// 设置显示位置
	printf("1.开始游戏");
	gotoxy(40, 12);
	printf("2.游戏规则");
	gotoxy(25, 16);
	printf("3.按键说明");
	gotoxy(40, 16);
	printf("4.退出");
	gotoxy(24, 22);
	color(5);														// 深紫色字体
	printf("请选择【1 2 3 4】：【 】\b\b\b");
	scanf("%d", &n);												// 输入选项
	switch (n)
	{
	case 1:
		Gameplay();
		break;
	case 2:
		regulation();
		break;
	case 3:
		explation();
		break;
	case 4:
		exit(0);
		break;
	}
}

/*
	绘制游戏界面4*4网格
*/
void drawTheGameBox()
{
	int i, j, k;
	color(14);															// 黄色
	for (j = 2; j <= 22; j += 5)										// 打印棋盘边框
	{
		gotoxy(15, j);
		for (k = 1; k < 42; k++)
		{
			printf("-");												// 打印棋盘的横向边框
		}
		printf("\n");
	}
	for (i = 3; i < 7; i++)
	{
		gotoxy(15, i);
		printf("|         |         |         |         | ");				// 打印棋盘的纵向边框
	}
	for (i = 8; i < 12; i++)
	{
		gotoxy(15, i);
		printf("|         |         |         |         | ");
	}
	for (i = 13; i < 17; i++)
	{
		gotoxy(15, i);
		printf("|         |         |         |         | ");
	}
	for (i = 18; i < 22; i++)
	{
		gotoxy(15, i);
		printf("|         |         |         |         | ");
	}

	gotoxy(16, 1);
	color(11);																	// 浅绿色
	printf("游戏分数：%ld", score);

	color(13);																	// 粉色
	gotoxy(42, 1);																// 屏幕坐标位置
	printf("执行步数：%d\n", step);	

	gotoxy(44, 23);
	color(10);																	// 绿色
	printf("已用时：%d s", time(NULL) - Time);									// 输出游戏运行时间
}

/*
	根据数字修改颜色
*/
int TextColors(int number)
{
	switch (number)																// 格子中出现2^n的数字显示为不同颜色
	{
	case 2:
		return color(12);
		break;
	case 4:
		return color(11);
		break;
	case 8:
		return color(10);
		break;
	case 16:
		return color(14);
		break;
	case 32:
		return color(6);
		break;
	case 64:
		return color(5);
		break;
	case 128:
		return color(4);
		break;
	case 256:
		return color(3);
		break;
	case 512:
		return color(2);
		break;
	case 1024:
		return color(9);
		break;
	case 2048:
		return color(7);
		break;
	}
	return 0;
}

/*
	合并数字
*/
int* add(int item[])
{
	int i = 0, j = 0;
	int tep[4] = { 0, 0, 0, 0 };
	int *tmp = (int*)calloc(sizeof(int), 4);								// 返回的指针必须分配到堆上
	int* ret = tmp;
	for (i = 0; i < 4; i++)
	{
		if (item[i] != 0)													// 如果这个格子里有数字
		{
			tep[j++] = item[i];
		}
	}
	// 把两个相邻的相同的数加起来
	for (i = 0; i < 3; i++)
	{
		if (tep[i] == tep[i + 1])											// 如果两个数字相同，则合并
		{
			tep[i] *= 2;													// 一个格子中的数字翻倍，另一个为空
			tep[i + 1] = 0;
			score = score + tep[i];											// 加分，加的分数为消除数字的2倍
		}
	}
	j = 0;
	for (i = 0; i < 4; i++)
	{
		if (tep[i] != 0)
		{
			tmp[j++] = tep[i];
		}
	}
	return tmp;
}

/*
	判断能否移动数字，1-能移动，0-不能移动
*/
int ifMove(int item[])
{
	int i = 0;
	for (i = 0; i < 3; i++)
	{
		// 如果两个相邻位置上数字相同，能移动，返回1
		if (item[i] != 0 && item[i] == item[i + 1])
		{
			return 1;
		}
		// 如果两个相邻位置上，一个是空格子，一个上有数字，也能移动，返回1
		if (item[i] == 0 && item[i + 1] != 0)
		{
			return 1;
		}
	}
	return 0;
}

/*
	判断x是否是2的n次方，是返回1，不是返回0
*/
int if2n(int x)
{
	int flag = 0;
	int n;
	int N = 1;
	for (n = 1; n <= 11; n++)
	{
		if (x == pow(2, n))
		{
			flag = 1;
			if (n > N)
				N = n;
			return flag;
		}
	}
	return flag;
}

/*
	判断是否能够上移，1-能；0-不能
*/
int ifup()
{
	int i, j;
	int flag = 0;														// 定义标志变量，只有0或1
	for (j = 0; j < 4; j++)
		for (i = 0; i < 3; i++)
		{
			// 如果上下两个格子中的数字相加是2的n次方，并且下面的格子中有数字
			if ((if2n(BOX[i][j] + BOX[i + 1][j]) == 1) && BOX[i + 1][j])
			{
				flag = 1;												// 可以上移
			}
		}
		return flag;
}

/*
	判断是否能够下移，1-能；0-不能
*/
int ifdown()
{
	int i, j;
	int flag = 0;														// 定义标志变量，只有0或1
	for (j = 0; j < 4; j++)
		for (i = 3; i > 0; i--)
		{
			// 如果上下两个格子中的数字相加是2的n次方，并且上面的格子中有数字
			if ((if2n(BOX[i][j] + BOX[i - 1][j]) == 1) && BOX[i - 1][j])
			{
				flag = 1;												// 可以下移
			}
		}
	return flag;
}

/*
	判断是否能够左移，1-能；0-不能
*/
int ifleft()
{
	int i, j;
	int flag = 0;														// 定义标志变量，只有0或1
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
		{
			// 如果左右两个格子中的数字相加是2的n次方，并且右面的格子中有数字
			if ((if2n(BOX[i][j] + BOX[i][j + 1]) == 1) && BOX[i][j + 1])
			{
				flag = 1;												// 可以左移
			}
		}
	return flag;
}

/*
	判断是否能够右移，1-能；0-不能
*/
int ifright()
{
	int i, j;
	int flag = 0;														// 定义标志变量，只有0或1
	for (i = 0; i < 4; i++)
		for (j = 3; j > 0; j--)
		{
			// 如果左右两个格子中的数字相加是2的n次方，并且左面的格子中有数字
			if ((if2n(BOX[i][j] + BOX[i][j - 1]) == 1) && BOX[i][j - 1])
			{
				flag = 1;												// 可以右移
			}
		}
	return flag;
}

/*
	返回棋盘最大数
*/
int BOXmax()
{
	int max = BOX[0][0];											
	int i, j;
	for (i = 0; i < 4; i++)											// 遍历整个数组
	{
		for (j = 0; j < 4; j++)
		{
			if (BOX[i][j] > max)									// 如果数组中有数值大于max的值
			{
				max = BOX[i][j];
			}
		}
	}
	return max;
}

/*
	判断是否胜利
*/
int Gamewin()
{
	int flag = 0;
	if (BOXmax() == 2048)											// 如果棋盘中的最大值为2048，则游戏胜利
	{
		system("cls");
		gotoxy(1, 6);
		color(2);													// 暗绿色
		// 输出win的字符画


		gotoxy(35, 17);
		color(13);
		printf("游戏胜利");
		flag = 1;
	}
	return flag;
}

/*
	判断是否失败，并输出棋盘最大数
*/
int Gamefaile()
{
	int flag = 0;
	int max;
	// 当上下左右都不能移动时，游戏失败
	if (ifup() + ifdown() + ifleft() + ifright() == 0)
	{
		system("cls");
		gotoxy(34, 3);
		color(14);
		printf("合并出的最大数是：");
		gotoxy(52, 3);
		color(11);
		max = BOXmax();
		printf("%d", max);

		gotoxy(34, 17);
		color(13);
		printf("无法移动，游戏失败！");
		flag = 1;
	}
	return flag;
}

/*
	键盘控制移动
*/
int keyboardControl(int key)
{
	int i = 0, j = 0;
	int* p;
	int tp[4] = { 0, 0, 0, 0 };
	switch (key)											// left=75, up = 72, right = 77, down = 80
	{
	case 72:												// 向上
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[0][i];								// 把一列数移到中间变量
			tp[1] = BOX[1][i];
			tp[2] = BOX[2][i];
			tp[3] = BOX[3][i];
			p = add(tp);									// 获得合并之后的数值
			// 判断是否可以移动，可以移动，则出现一个新数字；不能移动但按键了，也不会出现新数字
			if (!ifMove(tp))
			{
				j++;										// 向上移动
			}
			BOX[0][i] = p[0];								// 把处理好的中间变量移回来
			BOX[1][i] = p[1];
			BOX[2][i] = p[2];
			BOX[3][i] = p[3];
		}
		return j != 4;										// 当j不超过4时，可以执行up操作
	case 80:												// 向下
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[3][i];
			tp[1] = BOX[2][i];
			tp[2] = BOX[1][i];
			tp[3] = BOX[0][i];
			p = add(tp);
			if (!ifMove(tp))
			{
				j++;
			}
			BOX[3][i] = p[0];
			BOX[2][i] = p[1];
			BOX[1][i] = p[2];
			BOX[0][i] = p[3];
		}
		return j != 4;
	case 75:												// 向左
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[i][0];
			tp[1] = BOX[i][1];
			tp[2] = BOX[i][2];
			tp[3] = BOX[i][3];
			p = add(tp);
			if (!ifMove(tp))
			{
				j++;
			}
			BOX[i][0] = p[0];
			BOX[i][1] = p[1];
			BOX[i][2] = p[2];
			BOX[i][3] = p[3];
		}
		return j != 4;
	case 77:												// 向右
		j = 0;
		for (i = 0; i < 4; i++)
		{
			tp[0] = BOX[i][3];
			tp[1] = BOX[i][2];
			tp[2] = BOX[i][1];
			tp[3] = BOX[i][0];
			p = add(tp);
			if (!ifMove(tp))
			{
				j++;
			}
			BOX[i][3] = p[0];
			BOX[i][2] = p[1];
			BOX[i][1] = p[2];
			BOX[i][0] = p[3];
		}
		return j != 4;
	case 27:												//ESC
		gotoxy(20, 23);
		color(12);
		printf("确定退出游戏吗：（y/n）");
		{
			char c = _getch();
			if (c == 'y' || c == 'Y')
			{
				exit(0);										// 退出游戏
			}
			if (c == 'n' || c == 'N')
			{
				gotoxy(20, 23);
				printf("                             ");		// 继续游戏
			}
			break;
		}
	default:
		return 0;
	}
}

/*
	开始游戏
*/
void Gameplay()
{
	system("cls");
	int i = 0, j = 0;
	Time = time(NULL);										// 获取当前时间为游戏开始时间
	drawTheGameBox();										// 绘制游戏界面
	/*在随机位置显示初始数字2或者4*/
	int a, b;												// BOX[][]数组的两个元素
	srand(time(NULL));										// 设置随机数种子
	do
	{
		a = rand() % 4;
		b = rand() % 4;
	} while (BOX[a][b] != 0);								// 一直到棋盘中没有空格

	if (rand() % 4 == 0)
	{
		BOX[a][b] = 4;
	}
	else
	{
		BOX[a][b] = 2;
	}
	for (i = 0; i < 4; i++)									// 遍历整个网格
	{
		for (j = 0; j < 4; j++)
		{
			if (BOX[i][j] == 0)								// 如果网格中有空位，就继续下去
			{
				continue;
			}
			gotoxy(15 + j * 10 + 5, 2 + i * 5 + 3);			// 设置数字显示位置
			int c = BOX[i][j];
			TextColors(c);									// 设置数字颜色
			printf("%d", c);
		}
	}

	/*设置按键操作*/
	while (1)
	{
		while (_kbhit())
		{
			// 如果按下的键不是在keyboardControl()中定义的，会没有反应，一直到按下定义的按键为止
			if (!keyboardControl(_getch()))
			{
				continue;
			}
			drawTheGameBox();								// 绘制棋盘
			for (i = 0; i < 4; i++)							// 循环整个棋盘
			{
				for (j = 0; j < 4; j++)
				{
					if (BOX[i][j] == 0)						// 如果棋盘有空位，则可以一直按键
					{
						continue;
					}
					gotoxy(15 + j * 10 + 5, 2 + i * 5 + 3);	// 合并后的数出现的位置
					int c = BOX[i][j];
					TextColors(c);
					printf("%d", c);
				}
			}
			do {
				a = rand() % 4;
				b = rand() % 4;								// 获得随机位置
			} while (BOX[a][b] != 0);
			if (rand() % 4 == 0)							// 2或4随机出现在空格处（进行方向操作合并后，在空白处出现）
			{
				BOX[a][b] = 4;
			}
			else {
				BOX[a][b] = 2;
			}
			step++;
			gotoxy(15 + b * 10 + 5, 2 + a * 5 + 3);
			int c = BOX[a][b];
			TextColors(c);
			printf("%d", c);
		}

		// 只要游戏成功或失败，都会出现以下内容
		if (Gamefaile() + Gamewin() != 0)
		{
			int n;
			gotoxy(20, 20);
			color(12);
			printf("我要重新玩一局-------1");
			gotoxy(45, 20);
			printf("不玩了，退出吧-------2\n");
			gotoxy(43, 21);
			color(11);
			scanf("%d", &n);
			switch (n)
			{
			case 1:
				Replay();
				break;
			case 2:
				exit(0);
				break;
			}
		}
	}
}

/*
	重新开始游戏
*/
void Replay()
{
	system("cls");
	score = 0, step = 0;
	memset(BOX, 0, 16 * sizeof(int));
	Gameplay();
}



/*
	主函数
*/
int main()
{
	title();
	choice();
}

/*
	游戏规则介绍
*/
void regulation()
{
	int i, j = 1;
	system("cls");
	color(13);
	gotoxy(34, 3);
	printf("游戏规则");
	color(2);
	for (i = 6; i <= 18; i++)									// 输出上下边框
	{
		for (j = 15; j <= 70; j++)								// 输出左右边框
		{
			gotoxy(j, i);
			if (i == 6 || i == 18) printf("=");
			else if (j == 15 || j == 69) printf("||");
		}
	}
	color(3);
	gotoxy(18, 7);
	printf("tip1：玩家可以通过↑、↓、←、→方向键来移动方块");
	color(10);
	gotoxy(18, 9);
	printf("tip2：按ESC退出游戏");
	color(14);
	gotoxy(18, 11);
	printf("tip3：玩家选择的方向上，若有相同的数字则合并");
	color(11);
	gotoxy(18, 13);
	printf("tip4：每移动一步，空位随机出现一个2或4");
	color(4);
	gotoxy(18, 15);
	printf("tip5：棋盘被数字填满，无法进行有效移动，游戏失败");
	color(5);
	gotoxy(18, 17);
	printf("tip6：棋盘上出现2048，游戏胜利");
	_getch();													// 按任意键返回欢迎界面
	system("cls");
	main();
}

/*
	按键说明
*/
void explation()
{
	gotoxy(20, 22);
	color(13);
	printf("①、↑、↓、←、→方向键进行游戏操作");
	gotoxy(20, 24);
	printf("②、ESC键退出游戏");
	_getch();													// 按任意键返回欢迎界面
	system("cls");
	main();
}



