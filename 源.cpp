#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

// ���� Windows Multimedia API application programming interface
#pragma comment(lib,"Winmm.lib")/* #pragma ��һ��������ָ��,�Ǹ����߱�������Ҫ����һЩ����,Ȼ���ں����comment����ָ����ʲô���� 
                                          winmm.lib��������windows¼���豸�ɼ�¼������*/
#define High 890 // ��Ϸ����ߴ�
#define Width 500
#define BULLET_NUM 15
IMAGE img_bk; // ����ͼƬ
float position_x, position_y; // �ɻ�
float enemy_x, enemy_y; // �л�λ��
IMAGE img_planeNormal1, img_planeNormal2; // �����ɻ�ͼƬ
IMAGE img_planeExplode1, img_planeExplode2; // ��ը�ɻ�ͼƬ
IMAGE img_bullet1, img_bullet2; // �ӵ�ͼƬ
IMAGE img_enemyPlane1, img_enemyPlane2; // �л�ͼƬ
int isExpolde = 0; // �ɻ��Ƿ�ը
int score = 0; // �÷�
int gameStatus = 0;
class Plane
{
   public:
	void startup();
	void show();
	void updateWithoutInput();
	void updateWithInput();
	void gameover();
	void startMenu(); // ��ʼ�˵�����
	void pauseMenu(); // ��Ϸ��ͣ��˵����棬һ�㰴ESC���������ý���
	void readRecordFile();  //��ȡ��Ϸ�����ļ��浵
	void writeRecordFile();  //�洢��Ϸ�����ļ��浵
}Plane1;
struct Bullet
{
	int x;
	int y;
	bool flag;
}bullet[BULLET_NUM];
DWORD t1, t2;
void Plane::startup()
{
	t1 = t2 = GetTickCount();//��ȡϵͳ��������ǰ����ʱ��
	mciSendString("open D:\\gamemusic1.mp3 alias bkmusic", 0, 0, 0);//�򿪱�������
	mciSendString("play bkmusic repeat", 0, 0, 0);  // ѭ������
	initgraph(Width, High);
	// ��ȡ���ھ��
	HWND hwnd = GetHWnd();
	// ���ô��ڱ�������
	SetWindowText(hwnd, "�ɻ���ս ");
	loadimage(&img_bk, "D:\\one.jpg");
	loadimage(&img_planeNormal1, "D:\\peo1.png");
	loadimage(&img_planeNormal2, "D:\\peo11.png");
	loadimage(&img_bullet1, "D:\\hua1.png");
	loadimage(&img_bullet2, "D:\\hua2.png");
	loadimage(&img_enemyPlane1, "D:\\ememy1.png");
	loadimage(&img_enemyPlane2, "D:\\ememy11.png");
	loadimage(&img_planeExplode1, "D:\\p1l.jpg");
	position_x = Width * 0.5;
	position_y = High * 0.7;
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].flag = false;
	}
	enemy_x = Width * 0.5;
	enemy_y = 10;
	BeginBatchDraw();//��������Ҫ���������Ķ���ʱ��һֱˢ����Ļ���������.��һ����װ��˫����
	while (gameStatus == 0)
		startMenu(); // ��ʼ�˵�����
}
void Plane::startMenu() // ��ʼ�˵�����
{
	putimage(0, 0, &img_bk);	// ��ʾ����
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("����"));
	outtextxy(Width * 0.3, High * 0.2, "1 ����Ϸ");
	outtextxy(Width * 0.3, High * 0.3, "2 ��ȡ��Ϸ�浵");
	outtextxy(Width * 0.3, High * 0.4, "3 �˳�");

	settextcolor(WHITE);
	settextstyle(30, 0, _T("����"));
	outtextxy(Width * 0.25, High * 0.6, "�����ƶ����Ʒɻ��ƶ�");
	outtextxy(Width * 0.25, High * 0.65, "�ո����ӵ�");
	outtextxy(Width * 0.25, High * 0.7, "ESC����ͣ��Ϸ");
	outtextxy(Width * 0.25, High * 0.75, "ײ������������¿�ʼ");
	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // �ж��Ƿ�������
	{
		input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�
		if (input == '1')
			gameStatus = 1;
		else if (input == '2')
		{
			readRecordFile();
			gameStatus = 1;
		}
		else if (input == '3')
		{
			gameStatus = 2;
			exit(0);
		}
	}
}

void Plane::pauseMenu() // ��Ϸ��ͣ��˵����棬һ�㰴ESC���������ý���
{
	putimage(0, 0, &img_bk);	// ��ʾ����
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("����"));
	outtextxy(Width * 0.3, High * 0.2, "1 ������Ϸ");
	outtextxy(Width * 0.3, High * 0.3, "2 ���浵��");
	outtextxy(Width * 0.3, High * 0.4, "3 �˳�");

	settextcolor(WHITE);
	settextstyle(30, 0, _T("����"));
	outtextxy(Width * 0.25, High * 0.6, "�����ƶ����Ʒɻ��ƶ�");
	outtextxy(Width * 0.25, High * 0.65, "�ո����ӵ�");
	outtextxy(Width * 0.25, High * 0.7, "ESC����ͣ��Ϸ");
	outtextxy(Width * 0.25, High * 0.75, "ײ������������¿�ʼ");
	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // �ж��Ƿ�������
	{
		input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�
		if (input == '1')
			gameStatus = 1;
		else if (input == '2')
		{
			writeRecordFile();
			gameStatus = 1;
		}
		else if (input == '3')
		{
			gameStatus = 2;
			exit(0);
		}
	}
}

void Plane::readRecordFile()  //��ȡ��Ϸ�����ļ��浵
{
	FILE* fp;
	fopen_s(&fp, "c:\\gameRecord.dat", "r");
	for (int i = 0; i < BULLET_NUM; i++)
	{
		fscanf_s(fp, "%f %f %f %f %f %f %d %d", &position_x, &position_y, &bullet[i].x, &bullet[i].y, &enemy_x, &enemy_y, &isExpolde, &score);
	}
	fclose(fp);
}

void Plane::writeRecordFile()  //�洢��Ϸ�����ļ��浵
{
	FILE* fp;
	fopen_s(&fp, "c:\\gameRecord.dat", "w");
	for (int i = 0; i < BULLET_NUM; i++)
	{
		fprintf_s(fp, "%f %f %f %f %f %f %d %d", position_x, position_y, bullet[i].x, bullet[i].y, enemy_x, enemy_y, isExpolde, score);
	}
	fclose(fp);
}


void Plane::show()
{
	putimage(0, 0, &img_bk);	// ��ʾ����	
	if (isExpolde == 0)
	{
		putimage(position_x - 50, position_y - 60, &img_planeNormal1, NOTSRCERASE); // ��ʾ�����ɻ�	
		putimage(position_x - 50, position_y - 60, &img_planeNormal2, SRCINVERT);
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].flag == true)
			{
				putimage(bullet[i].x , bullet[i].y, &img_bullet1, NOTSRCERASE); // ��ʾ�ӵ�	
				putimage(bullet[i].x, bullet[i].y, &img_bullet2, SRCINVERT);
			}
		} 
		putimage(enemy_x, enemy_y, &img_enemyPlane1, NOTSRCERASE); // ��ʾ�л�	
		putimage(enemy_x, enemy_y, &img_enemyPlane2, SRCINVERT);
	}
	else
	{
		putimage(0,0, &img_planeExplode1, NOTSRCERASE); // ��ʾ��ը�ɻ�	
		putimage(0,0, &img_planeExplode1, SRCINVERT);
	}
	outtextxy(Width * 0.48, High * 0.90, "�÷֣�");//�ú�������Ļ�ϵ�ָ����(x��y)����ʾ�ı����ַ���
	char s[5];
	sprintf_s(s, "%d", score);
	outtextxy(Width * 0.55, High * 0.90, s);
	FlushBatchDraw();
	Sleep(2);
}
void Plane::updateWithoutInput()
{
	if (isExpolde == 0)
	{

		if (enemy_y < High - 25)
			enemy_y = enemy_y + 1;
		else
			enemy_y = 10;
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (fabs(bullet[i].x - enemy_x) + fabs(bullet[i].y - enemy_y) < 80)  // �ӵ����ел�
			{
				enemy_x = rand() % Width; //��0~Width-1֮����������
				enemy_y = -40;
				bullet[i].y = -85;
			}
		}
	}

		if (fabs(position_x - enemy_x) + fabs(position_y - enemy_y) < 150)  // �л���������
		{
			isExpolde = 1;
			mciSendString("close exmusic", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString("open D:\\lose1.mp3 alias exmusic", NULL, 0, NULL); // ������
			mciSendString("play exmusic", NULL, 0, NULL); // ������һ��		
		}
}
void CreateBullet()
{
	int i = 0;
	for (i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == false)

		{
			bullet[i].flag = true;
			bullet[i].x = position_x;
			bullet[i].y = position_y-50;
			break;
		}
	}
}
void BulletMove(int speed = 2)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].flag == true)
		{
			bullet[i].y -= speed;
			if (bullet[i].y <= 0)
				bullet[i].flag = false;
		}
	}
}
void Move(int speed =2)
{
	if (GetAsyncKeyState(VK_UP)&&position_y>=0)
		position_y = position_y -speed;
	if (GetAsyncKeyState(VK_DOWN)&&position_y<=High)
		position_y = position_y + speed;
	if (GetAsyncKeyState(VK_LEFT)&&position_x>=0)
		position_x = position_x - speed;
	if (GetAsyncKeyState(VK_RIGHT)&&position_x+20<=Width)
		position_x = position_x + speed;
	if (GetAsyncKeyState(VK_SPACE)&&t2-t1>200)
	{
		CreateBullet();
		t1 = t2;
	}
	t2 = GetTickCount();
	BulletMove();
}
void Plane::updateWithInput()
{
	if (isExpolde == 0)
	{
			Move();
			mciSendString("close fgmusic", NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString("open D:\\f_gun.mp3 alias fgmusic", NULL, 0, NULL); // ������
			mciSendString("play fgmusic", NULL, 0, NULL); // ������һ��
			char input;
			if (_kbhit())  // �ж��Ƿ�������
			{
				input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�
				if (input== 27) // ESC����ACSII��Ϊ27
				{
					gameStatus = 3;
				}
			}
	}
}

void Plane::gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();
}

int main()
{
	Plane1.startup();  // ���ݳ�ʼ��	
	while (1)  //  ��Ϸѭ��ִ��
	{

		Plane1.show();  // ��ʾ����
		Plane1.updateWithoutInput();  // ���û������޹صĸ���
		Plane1.updateWithInput();     // ���û������йصĸ���
	}
	Plane1.gameover();     // ��Ϸ��������������
	return 0;
}
