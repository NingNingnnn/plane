#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

// 引用 Windows Multimedia API application programming interface
#pragma comment(lib,"Winmm.lib")/* #pragma 是一条编译器指令,是给告诉编译器你要链接一些东西,然后在后面的comment里面指明是什么东西 
                                          winmm.lib库用来对windows录音设备采集录音数据*/
#define High 890 // 游戏画面尺寸
#define Width 500
#define BULLET_NUM 15
IMAGE img_bk; // 背景图片
float position_x, position_y; // 飞机
float enemy_x, enemy_y; // 敌机位置
IMAGE img_planeNormal1, img_planeNormal2; // 正常飞机图片
IMAGE img_planeExplode1, img_planeExplode2; // 爆炸飞机图片
IMAGE img_bullet1, img_bullet2; // 子弹图片
IMAGE img_enemyPlane1, img_enemyPlane2; // 敌机图片
int isExpolde = 0; // 飞机是否爆炸
int score = 0; // 得分
int gameStatus = 0;
class Plane
{
   public:
	void startup();
	void show();
	void updateWithoutInput();
	void updateWithInput();
	void gameover();
	void startMenu(); // 初始菜单界面
	void pauseMenu(); // 游戏暂停后菜单界面，一般按ESC键后启动该界面
	void readRecordFile();  //读取游戏数据文件存档
	void writeRecordFile();  //存储游戏数据文件存档
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
	t1 = t2 = GetTickCount();//获取系统开机到当前经过时间
	mciSendString("open D:\\gamemusic1.mp3 alias bkmusic", 0, 0, 0);//打开背景音乐
	mciSendString("play bkmusic repeat", 0, 0, 0);  // 循环播放
	initgraph(Width, High);
	// 获取窗口句柄
	HWND hwnd = GetHWnd();
	// 设置窗口标题文字
	SetWindowText(hwnd, "飞机大战 ");
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
	BeginBatchDraw();//当我们需要绘制连续的动画时，一直刷新屏幕会产生闪屏.是一个封装的双缓存
	while (gameStatus == 0)
		startMenu(); // 初始菜单界面
}
void Plane::startMenu() // 初始菜单界面
{
	putimage(0, 0, &img_bk);	// 显示背景
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("宋体"));
	outtextxy(Width * 0.3, High * 0.2, "1 新游戏");
	outtextxy(Width * 0.3, High * 0.3, "2 读取游戏存档");
	outtextxy(Width * 0.3, High * 0.4, "3 退出");

	settextcolor(WHITE);
	settextstyle(30, 0, _T("宋体"));
	outtextxy(Width * 0.25, High * 0.6, "键盘移动控制飞机移动");
	outtextxy(Width * 0.25, High * 0.65, "空格发射子弹");
	outtextxy(Width * 0.25, High * 0.7, "ESC键暂停游戏");
	outtextxy(Width * 0.25, High * 0.75, "撞击后按任意键重新开始");
	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();  // 根据用户的不同输入来移动，不必输入回车
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

void Plane::pauseMenu() // 游戏暂停后菜单界面，一般按ESC键后启动该界面
{
	putimage(0, 0, &img_bk);	// 显示背景
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(50, 0, _T("宋体"));
	outtextxy(Width * 0.3, High * 0.2, "1 继续游戏");
	outtextxy(Width * 0.3, High * 0.3, "2 保存档案");
	outtextxy(Width * 0.3, High * 0.4, "3 退出");

	settextcolor(WHITE);
	settextstyle(30, 0, _T("宋体"));
	outtextxy(Width * 0.25, High * 0.6, "键盘移动控制飞机移动");
	outtextxy(Width * 0.25, High * 0.65, "空格发射子弹");
	outtextxy(Width * 0.25, High * 0.7, "ESC键暂停游戏");
	outtextxy(Width * 0.25, High * 0.75, "撞击后按任意键重新开始");
	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();  // 根据用户的不同输入来移动，不必输入回车
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

void Plane::readRecordFile()  //读取游戏数据文件存档
{
	FILE* fp;
	fopen_s(&fp, "c:\\gameRecord.dat", "r");
	for (int i = 0; i < BULLET_NUM; i++)
	{
		fscanf_s(fp, "%f %f %f %f %f %f %d %d", &position_x, &position_y, &bullet[i].x, &bullet[i].y, &enemy_x, &enemy_y, &isExpolde, &score);
	}
	fclose(fp);
}

void Plane::writeRecordFile()  //存储游戏数据文件存档
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
	putimage(0, 0, &img_bk);	// 显示背景	
	if (isExpolde == 0)
	{
		putimage(position_x - 50, position_y - 60, &img_planeNormal1, NOTSRCERASE); // 显示正常飞机	
		putimage(position_x - 50, position_y - 60, &img_planeNormal2, SRCINVERT);
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].flag == true)
			{
				putimage(bullet[i].x , bullet[i].y, &img_bullet1, NOTSRCERASE); // 显示子弹	
				putimage(bullet[i].x, bullet[i].y, &img_bullet2, SRCINVERT);
			}
		} 
		putimage(enemy_x, enemy_y, &img_enemyPlane1, NOTSRCERASE); // 显示敌机	
		putimage(enemy_x, enemy_y, &img_enemyPlane2, SRCINVERT);
	}
	else
	{
		putimage(0,0, &img_planeExplode1, NOTSRCERASE); // 显示爆炸飞机	
		putimage(0,0, &img_planeExplode1, SRCINVERT);
	}
	outtextxy(Width * 0.48, High * 0.90, "得分：");//该函数在屏幕上的指定点(x，y)上显示文本或字符串
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
			if (fabs(bullet[i].x - enemy_x) + fabs(bullet[i].y - enemy_y) < 80)  // 子弹击中敌机
			{
				enemy_x = rand() % Width; //在0~Width-1之间产生随机数
				enemy_y = -40;
				bullet[i].y = -85;
			}
		}
	}

		if (fabs(position_x - enemy_x) + fabs(position_y - enemy_y) < 150)  // 敌机击中我们
		{
			isExpolde = 1;
			mciSendString("close exmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString("open D:\\lose1.mp3 alias exmusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play exmusic", NULL, 0, NULL); // 仅播放一次		
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
			mciSendString("close fgmusic", NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString("open D:\\f_gun.mp3 alias fgmusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play fgmusic", NULL, 0, NULL); // 仅播放一次
			char input;
			if (_kbhit())  // 判断是否有输入
			{
				input = _getch();  // 根据用户的不同输入来移动，不必输入回车
				if (input== 27) // ESC键的ACSII码为27
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
	Plane1.startup();  // 数据初始化	
	while (1)  //  游戏循环执行
	{

		Plane1.show();  // 显示画面
		Plane1.updateWithoutInput();  // 与用户输入无关的更新
		Plane1.updateWithInput();     // 与用户输入有关的更新
	}
	Plane1.gameover();     // 游戏结束、后续处理
	return 0;
}
