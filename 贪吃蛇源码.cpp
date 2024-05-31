#define _CRT_SECURE_NO_WARNINGS
// 必要的头文件 
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include<fstream>

// 定义标记上下左右的明示常量 
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ESC 5
#define FOOD 10
#define MAX_SCORES 100//储存的最大分数个数
#define SCORE_FILE "scores.txt"//分数储存的文件

// 定义表示位置的结构体类型
typedef struct snake {
	int x;
	int y;
	struct snake* next;
}snake;
// 定义全局变量 
int score = 0; // 当前得分
int speed = 200; // 存储当前速度
int status;
snake* tail, * head; // 存储蛇头蛇尾 
snake* food, * q;// q用于遍历链表 
HANDLE hOUT;
void gotoxy(int x, int y); // 设置光标位置 A
int choice(void); // 载入游戏界面 B
int check(int a, int b);//检查食物有没有刷在身体里 C
int color(int c); // 设置字体颜色  D
void printGame(void); // 打印游戏界面  E
void printSnake(void); // 打印蛇身 F
void printFood(void); // 打印食物 G
void printTips(void); // 打印提示 H
void snakeMove(void); // 主操作函数 I
int biteSelf(void); // 判断是否咬到了自己 J
int encounterWall(void); // 判断是否撞墙 K
void keyboardControl(void); // 获取击键 L
void speedUp(void); // 加速 M
void speedDown(void); // 减速 N
char* s_gets(char* st, int n); // 读取字符 O
void frees(snake*); // 释放内存 P
void saveScore(int score);//读取分数 Q
void loadAndSortScores();//输出分数和排名 R
void swap(int* xp, int* yp);//基础交换函数 S
void sortScores(int scores[], int n);//将分数排名 T

int main(int argc, char* argv[])
{
	while (1)
	{
		if (choice() == 1)
			keyboardControl();//L
		else
		{
			gotoxy(5, 15);
			printf("按任意键返回");
			getchar(); // 去除前一个前导换行 
			while (1)
			{
				if (getchar())
				{
					system("cls");
					break;
				}
			}
		}
	}
	frees(head);
	return 0;
}

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int choice(void)
{
	int yourchoice;//此段直接跳到111行，中间是后加的初始界面显示
	// 画出界面 
	gotoxy(0, 0);
	color(2);
	printf("          00                       00\n"
		"        0000000                   000                         000                 000                 00 000                00    000  00\n"
		"     000000000000                00000000000       000        000                 000               00000000               000     0000000\n"
		"   00000  000 000000     00000000000000000000      000   0000000000000           000               0000 0000000000000      0000    000 000\n"
		" 0000 0000000000 0000    000000000              000000000000   00  000          0000              0000 000 0000    0       0000000 000000000\n"
		"   0   0   0000          00  00000000000000     00 000  00000     000    000000000000000000    0000000000  000            000  000000000000\n"
		"      00000000000        000 000 0000000000     0000000000 000   000      000000000 00000000    000000000  000000000     0 000  00  0000  00\n"
		"      00000000000        00  000      0000      000000000  000000000           0000                000     0000         000000000    0000000\n"
		"      000 000 000        0000000    0000           000     00000              00000000             000     000000000    000   000    00000\n"
		"      000000  000        0000000   000    000      0000000 000    000        000   000000          000     000000000    000   000  000000\n"
		"      0000000000                 00000    000   0000000000 0000   000        000      00000        000     000          000000000 00000000000\n"
		"        000000000               00000000000000         000 0000000000       000         00000      000     0000                      000000\n"
		"      000    000                                                           000                            000\n\n\n");
	color(12);
	printf("\t\t\t\t\t\t1.进入游戏\t2.查看说明\t3.退出游戏\n");
	color(11);
	printf("\t\t\t\t\t\t请选择:");
	scanf_s("%d", &yourchoice);
	system("cls");//清除屏幕内容，只适用于Windows系统
	switch (yourchoice)
	{
	case 1:
		// 初始化 
		printGame();
		printSnake();
		printFood();
		break;
	case 2:
		printTips();
		break;
	case 3:
		gotoxy(30, 10);
		color(11);
		printf("Bye!");
		exit(0);
	default:
		printf("没有此序号,请输入1,2或3\n");
		Sleep(2000);//暂停两千毫秒
		system("cls");
	}
	return yourchoice;
}
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //更改文字颜色
	return 0;
}
void printGame()
{
	int i, j;
	gotoxy(30, 2);//在 Borland C 的扩充函数库 conio.h 中声明
	printf("游戏正在加载……\n");
	gotoxy(30, 6);
	printf("用↑ ↓ ← →分别控制蛇的移动\n");
	gotoxy(30, 8);
	printf("每次获取食物加10分  F1加速,F2减速,空格暂停，再次按下空格开始\n");
	gotoxy(30, 10);
	printf("不能撞墙和咬到自己!");
	gotoxy(30, 12);
	printf("速度不低于100,不高于300");
	Sleep(3000);
	system("cls");

	// 打印上下界面
	for (i = 0; i <= 50; i += 2)
	{
		gotoxy(i, 0);
		printf("□");
		gotoxy(i, 25);
		printf("□");
	}
	// 打印左右界面
	for (i = 0; i <= 25; i += 1)
	{
		gotoxy(0, i);
		printf("□");
		gotoxy(50, i);
		printf("□");
	}
	// 打印中间网格
	for (i = 1; i <= 24; i += 1)
	{
		for (j = 2; j <= 48; j += 2)
		{
			gotoxy(j, i);
			color(11);
			printf("■");
		}
	}
	// 打印右侧的规则和计分栏
	gotoxy(60, 13);
	printf("当前分数:%d分,当前速度%d", score, speed);
	gotoxy(60, 15);
	printf("用↑ ↓ ← →分别控制蛇的移动\n");
	gotoxy(60, 18);
	printf("每次获取食物加10分  按下F1加速,F2减速,空格暂停\n");
	gotoxy(60, 20);
	printf("不能撞墙和咬到自己!");
	gotoxy(60, 22);
	printf("速度不低于100,不高于300");
}
void printSnake(void)//195-250行代码参考文心一言
{
	int i;
	// 设定蛇尾(16,13),头插入,初始向右 
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 16;
	tail->y = 13;
	tail->next = NULL;
	// 设定初始蛇长是4
	for (i = 1; i <= 4; i++)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 16 + 2 * i;
		head->y = 13;
		tail = head; // 头成为尾
	}
	// 输出蛇身
	while (tail->next)
	{
		gotoxy(tail->x, tail->y);
		color(14);
		printf("★");
		tail = tail->next;
	}
}
int check(int a, int b) {
	q = head;
	while (q->next != NULL) {
		if (a != q->x || b != q->y) {
			q = q->next;
		}
		else
			return 1;
	}
	if (q->next == NULL) {
		return 0;
	}
}
void printFood(void)
{
	srand((unsigned)time(NULL));
	// 利用时钟修改种子 
	food = (snake*)malloc(sizeof(snake));
	food->x = 1; // 初始化x坐标
	do {
		while (food->x % 2 && food->x)
		{
			food->x = rand() % 46 + 2;// 2-48 
		}
		food->y = rand() % 23 + 1; // 1-24
	} while (check(food->x, food->y));
	gotoxy(food->x, food->y);
	color(12);
	printf("●");
}
void printTips(void)
{
	color(11);
	printf("***********Tips************\n"
		"1.采用合理的速度可以获得更高的分数哦!\n"
		"2.一定不要撞到自己或者两边的墙!\n");
}
void snakeMove(void)
{
	snake* snakenext;
	snakenext = (snake*)malloc(sizeof(snake));
	if (biteSelf() || encounterWall())
	{
		gotoxy(60, 11);
		if (biteSelf) {
			printf("咬到自己啦!");
		}
		else {
			printf("撞到墙啦！");
		}
		free(snakenext);
		Sleep(3000);
		system("cls");
		printf("GAMEOVER\n");
		saveScore(score);
		loadAndSortScores();
		Sleep(10000);
		system("cls");
		exit(0);
	}
	else
	{   // 前两个条件判断完成才开始移动 
		Sleep(350 - speed);
		if (status == UP || status == DOWN || status == LEFT || status == RIGHT)
		{
			if (status == LEFT) {
				snakenext->y = head->y;
				snakenext->x = head->x - 2;
			}
			else if (status == RIGHT) {
				snakenext->y = head->y;
				snakenext->x = head->x + 2;
			}
			if (status == UP) {
				snakenext->x = head->x;
				snakenext->y = head->y - 1;
			}
			else if (status == DOWN) {
				snakenext->x = head->x;
				snakenext->y = head->y + 1;
			}
			snakenext->next = head;
			head = snakenext;
			q = head;
			if (snakenext->x == food->x && snakenext->y == food->y)
			{
				while (q)
				{
					gotoxy(q->x, q->y);
					color(14);
					printf("★");
					q = q->next;
				}
				score += FOOD;
				gotoxy(60, 13);
				printf("当前分数:%d分,当前速度%d", score, speed);
				printFood();
			}
			else
			{
				while (q->next->next)
				{
					gotoxy(q->x, q->y);
					color(14);
					printf("★");
					q = q->next;
				}
				gotoxy(q->next->x, q->next->y);
				color(11);
				printf("■");
				free(q->next);
				q->next = NULL;
			}
		}
	}
}


void saveScore(int score) {
	FILE* file = fopen(SCORE_FILE, "a"); // 以追加模式打开文件
	if (file) {
		fprintf(file, "%d\n", score); // 将分数写入文件
		fclose(file);
	}
}

void loadAndSortScores() {
	int scores[MAX_SCORES];
	int n = 0;
	FILE* file = fopen(SCORE_FILE, "r");
	if (file) {
		while (fscanf(file, "%d", &scores[n]) != EOF && n < MAX_SCORES) {
			n++;
		}
		fclose(file);// 读取文件中的所有分数
		sortScores(scores, n);// 对分数进行排序（这里使用冒泡排序作为示例）
		printf("分数与排名:\n");
		for (int i = 0; i < n; i++) {
			printf("排名 %d: %d\n", i + 1, scores[i]);
		}
	}
	else {
		printf("Error opening file.\n");
	}// 输出排序后的分数和排名
}

void swap(int* xp, int* yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void sortScores(int scores[], int n) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (scores[j] < scores[j + 1]) {
				swap(&scores[j], &scores[j + 1]);//分数交换使分数由大到小排名
			}
		}
	}
}

int biteSelf(void)//遍历链表若头与蛇身体任意位置坐标相同则死亡。
{
	int x = 0; // 默认未咬到自己
	q = head->next;
	// 遍历蛇身 
	while (q->next)
	{
		if (q->x == head->x && q->y == head->y)
		{
			x = 1;
		}
		q = q->next;
	}

	return x;
}
int encounterWall(void)
{
	int x = 0; // 默认未撞到墙，防止开局死亡

	if (head->x == 0 || head->x == 50 || head->y == 0 || head->y == 25)
		x = 1;

	return x;
}
void keyboardControl(void)
{
	status = RIGHT; // 初始蛇向右移动
	while (1)
	{
		if (GetAsyncKeyState(VK_UP) && status != DOWN) // GetAsyncKeyState函数用来判断函数调用时指定虚拟键的状态
		{
			status = UP;           //如果蛇不是向下前进的时候，按上键，执行向上前进操作
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != UP) // 如果蛇不是向上前进的时候，按下键，执行向下前进操作
		{
			status = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != RIGHT) // 如果蛇不是向右前进的时候，按左键，执行向左前进
		{
			status = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != LEFT) // 如果蛇不是向左前进的时候，按右键，执行向右前进
		{
			status = RIGHT;
		}
		if (GetAsyncKeyState(VK_SPACE))// 空格暂停 
		{
			while (1)
			{
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE)) // 再次按空格改变状态 
				{
					break;
				}

			}
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			status = ESC; // 按esc键,直接到结束界面
			system("cls");
			color(11);
			gotoxy(30, 20);
			printf("游戏结束");
			exit(0);
		}
		else if (GetAsyncKeyState(VK_F1)) // 按F1键，加速；确认虚拟键是否按下，判定F1键状态
		{
			speedUp();
			gotoxy(60, 13);
			printf("当前分数:%d分,当前速度%d", score, speed);
		}
		else if (GetAsyncKeyState(VK_F2)) // 按F2键，减速
		{
			speedDown();
			gotoxy(60, 13);
			printf("当前分数:%d分,当前速度%d", score, speed);
		}
		snakeMove();
	}
}
void speedUp(void)
{
	if (speed <= 280)
		speed += 20;
}
void speedDown(void)
{
	if (speed >= 120)
		speed -= 20;
}
char* s_gets(char* st, int n)
{
	char* ret_val;
	char* find;

	gotoxy(60, 11);
	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		find = strchr(st, '\n');
		if (find)
			*find = '\0';
		else
			while (getchar() != '\n')
				continue;
	}

	return ret_val;
}
void frees(snake* s)
{
	snake* current = s;
	snake* next;

	while (current)
	{
		next = current->next; // 保存下一个节点的指针  
		free(current);        // 释放当前节点  
		current = next;        // 移动到下一个节点  
	}
}
