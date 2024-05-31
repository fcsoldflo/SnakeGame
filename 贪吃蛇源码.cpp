#define _CRT_SECURE_NO_WARNINGS
// ��Ҫ��ͷ�ļ� 
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include<fstream>

// �������������ҵ���ʾ���� 
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define ESC 5
#define FOOD 10
#define MAX_SCORES 100//���������������
#define SCORE_FILE "scores.txt"//����������ļ�

// �����ʾλ�õĽṹ������
typedef struct snake {
	int x;
	int y;
	struct snake* next;
}snake;
// ����ȫ�ֱ��� 
int score = 0; // ��ǰ�÷�
int speed = 200; // �洢��ǰ�ٶ�
int status;
snake* tail, * head; // �洢��ͷ��β 
snake* food, * q;// q���ڱ������� 
HANDLE hOUT;
void gotoxy(int x, int y); // ���ù��λ�� A
int choice(void); // ������Ϸ���� B
int check(int a, int b);//���ʳ����û��ˢ�������� C
int color(int c); // ����������ɫ  D
void printGame(void); // ��ӡ��Ϸ����  E
void printSnake(void); // ��ӡ���� F
void printFood(void); // ��ӡʳ�� G
void printTips(void); // ��ӡ��ʾ H
void snakeMove(void); // ���������� I
int biteSelf(void); // �ж��Ƿ�ҧ�����Լ� J
int encounterWall(void); // �ж��Ƿ�ײǽ K
void keyboardControl(void); // ��ȡ���� L
void speedUp(void); // ���� M
void speedDown(void); // ���� N
char* s_gets(char* st, int n); // ��ȡ�ַ� O
void frees(snake*); // �ͷ��ڴ� P
void saveScore(int score);//��ȡ���� Q
void loadAndSortScores();//������������� R
void swap(int* xp, int* yp);//������������ S
void sortScores(int scores[], int n);//���������� T

int main(int argc, char* argv[])
{
	while (1)
	{
		if (choice() == 1)
			keyboardControl();//L
		else
		{
			gotoxy(5, 15);
			printf("�����������");
			getchar(); // ȥ��ǰһ��ǰ������ 
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
	int yourchoice;//�˶�ֱ������111�У��м��Ǻ�ӵĳ�ʼ������ʾ
	// �������� 
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
	printf("\t\t\t\t\t\t1.������Ϸ\t2.�鿴˵��\t3.�˳���Ϸ\n");
	color(11);
	printf("\t\t\t\t\t\t��ѡ��:");
	scanf_s("%d", &yourchoice);
	system("cls");//�����Ļ���ݣ�ֻ������Windowsϵͳ
	switch (yourchoice)
	{
	case 1:
		// ��ʼ�� 
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
		printf("û�д����,������1,2��3\n");
		Sleep(2000);//��ͣ��ǧ����
		system("cls");
	}
	return yourchoice;
}
int color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);        //����������ɫ
	return 0;
}
void printGame()
{
	int i, j;
	gotoxy(30, 2);//�� Borland C �����亯���� conio.h ������
	printf("��Ϸ���ڼ��ء���\n");
	gotoxy(30, 6);
	printf("�á� �� �� ���ֱ�����ߵ��ƶ�\n");
	gotoxy(30, 8);
	printf("ÿ�λ�ȡʳ���10��  F1����,F2����,�ո���ͣ���ٴΰ��¿ո�ʼ\n");
	gotoxy(30, 10);
	printf("����ײǽ��ҧ���Լ�!");
	gotoxy(30, 12);
	printf("�ٶȲ�����100,������300");
	Sleep(3000);
	system("cls");

	// ��ӡ���½���
	for (i = 0; i <= 50; i += 2)
	{
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, 25);
		printf("��");
	}
	// ��ӡ���ҽ���
	for (i = 0; i <= 25; i += 1)
	{
		gotoxy(0, i);
		printf("��");
		gotoxy(50, i);
		printf("��");
	}
	// ��ӡ�м�����
	for (i = 1; i <= 24; i += 1)
	{
		for (j = 2; j <= 48; j += 2)
		{
			gotoxy(j, i);
			color(11);
			printf("��");
		}
	}
	// ��ӡ�Ҳ�Ĺ���ͼƷ���
	gotoxy(60, 13);
	printf("��ǰ����:%d��,��ǰ�ٶ�%d", score, speed);
	gotoxy(60, 15);
	printf("�á� �� �� ���ֱ�����ߵ��ƶ�\n");
	gotoxy(60, 18);
	printf("ÿ�λ�ȡʳ���10��  ����F1����,F2����,�ո���ͣ\n");
	gotoxy(60, 20);
	printf("����ײǽ��ҧ���Լ�!");
	gotoxy(60, 22);
	printf("�ٶȲ�����100,������300");
}
void printSnake(void)//195-250�д���ο�����һ��
{
	int i;
	// �趨��β(16,13),ͷ����,��ʼ���� 
	tail = (snake*)malloc(sizeof(snake));
	tail->x = 16;
	tail->y = 13;
	tail->next = NULL;
	// �趨��ʼ�߳���4
	for (i = 1; i <= 4; i++)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 16 + 2 * i;
		head->y = 13;
		tail = head; // ͷ��Ϊβ
	}
	// �������
	while (tail->next)
	{
		gotoxy(tail->x, tail->y);
		color(14);
		printf("��");
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
	// ����ʱ���޸����� 
	food = (snake*)malloc(sizeof(snake));
	food->x = 1; // ��ʼ��x����
	do {
		while (food->x % 2 && food->x)
		{
			food->x = rand() % 46 + 2;// 2-48 
		}
		food->y = rand() % 23 + 1; // 1-24
	} while (check(food->x, food->y));
	gotoxy(food->x, food->y);
	color(12);
	printf("��");
}
void printTips(void)
{
	color(11);
	printf("***********Tips************\n"
		"1.���ú�����ٶȿ��Ի�ø��ߵķ���Ŷ!\n"
		"2.һ����Ҫײ���Լ��������ߵ�ǽ!\n");
}
void snakeMove(void)
{
	snake* snakenext;
	snakenext = (snake*)malloc(sizeof(snake));
	if (biteSelf() || encounterWall())
	{
		gotoxy(60, 11);
		if (biteSelf) {
			printf("ҧ���Լ���!");
		}
		else {
			printf("ײ��ǽ����");
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
	{   // ǰ���������ж���ɲſ�ʼ�ƶ� 
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
					printf("��");
					q = q->next;
				}
				score += FOOD;
				gotoxy(60, 13);
				printf("��ǰ����:%d��,��ǰ�ٶ�%d", score, speed);
				printFood();
			}
			else
			{
				while (q->next->next)
				{
					gotoxy(q->x, q->y);
					color(14);
					printf("��");
					q = q->next;
				}
				gotoxy(q->next->x, q->next->y);
				color(11);
				printf("��");
				free(q->next);
				q->next = NULL;
			}
		}
	}
}


void saveScore(int score) {
	FILE* file = fopen(SCORE_FILE, "a"); // ��׷��ģʽ���ļ�
	if (file) {
		fprintf(file, "%d\n", score); // ������д���ļ�
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
		fclose(file);// ��ȡ�ļ��е����з���
		sortScores(scores, n);// �Է���������������ʹ��ð��������Ϊʾ����
		printf("����������:\n");
		for (int i = 0; i < n; i++) {
			printf("���� %d: %d\n", i + 1, scores[i]);
		}
	}
	else {
		printf("Error opening file.\n");
	}// ��������ķ���������
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
				swap(&scores[j], &scores[j + 1]);//��������ʹ�����ɴ�С����
			}
		}
	}
}

int biteSelf(void)//����������ͷ������������λ��������ͬ��������
{
	int x = 0; // Ĭ��δҧ���Լ�
	q = head->next;
	// �������� 
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
	int x = 0; // Ĭ��δײ��ǽ����ֹ��������

	if (head->x == 0 || head->x == 50 || head->y == 0 || head->y == 25)
		x = 1;

	return x;
}
void keyboardControl(void)
{
	status = RIGHT; // ��ʼ�������ƶ�
	while (1)
	{
		if (GetAsyncKeyState(VK_UP) && status != DOWN) // GetAsyncKeyState���������жϺ�������ʱָ���������״̬
		{
			status = UP;           //����߲�������ǰ����ʱ�򣬰��ϼ���ִ������ǰ������
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != UP) // ����߲�������ǰ����ʱ�򣬰��¼���ִ������ǰ������
		{
			status = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != RIGHT) // ����߲�������ǰ����ʱ�򣬰������ִ������ǰ��
		{
			status = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != LEFT) // ����߲�������ǰ����ʱ�򣬰��Ҽ���ִ������ǰ��
		{
			status = RIGHT;
		}
		if (GetAsyncKeyState(VK_SPACE))// �ո���ͣ 
		{
			while (1)
			{
				Sleep(300);
				if (GetAsyncKeyState(VK_SPACE)) // �ٴΰ��ո�ı�״̬ 
				{
					break;
				}

			}
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
		{
			status = ESC; // ��esc��,ֱ�ӵ���������
			system("cls");
			color(11);
			gotoxy(30, 20);
			printf("��Ϸ����");
			exit(0);
		}
		else if (GetAsyncKeyState(VK_F1)) // ��F1�������٣�ȷ��������Ƿ��£��ж�F1��״̬
		{
			speedUp();
			gotoxy(60, 13);
			printf("��ǰ����:%d��,��ǰ�ٶ�%d", score, speed);
		}
		else if (GetAsyncKeyState(VK_F2)) // ��F2��������
		{
			speedDown();
			gotoxy(60, 13);
			printf("��ǰ����:%d��,��ǰ�ٶ�%d", score, speed);
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
		next = current->next; // ������һ���ڵ��ָ��  
		free(current);        // �ͷŵ�ǰ�ڵ�  
		current = next;        // �ƶ�����һ���ڵ�  
	}
}
