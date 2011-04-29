#include "..\..\include\hge.h"
#include "..\..\include\hgecolor.h"
#include "..\..\include\hgesprite.h"
#include "..\..\include\hgedistort.h"
#include "..\..\include\hgefont.h"
#include "..\..\include\hgegui.h"
#include "CrackingTheCodeServer.h"
#include "menuitem.h"
#include "windows.h"
#include <process.h>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

#define START 1
#define STOP 2

HTEXTURE boxTexture, chatboxTexture, cursorTexture, matrixTexture1, matrixTexture2, matrixTexture3, matrixTexture4, matrixTexture5, matrixTexture6, 
matrixTexture7, matrixTexture8, matrixTexture9, matrixTexture10, matrixTexture11, matrixTexture12;
HEFFECT scrollSound, startSound, stopSound;
HMUSIC matrixTheme;
hgeSprite *box, *textBox, *cursor, *matrixSprite;
hgeGUI	*gui;
hgeFont	*text;

HGE *hge = 0;
CrackingTheCodeServer serverObj;

int yOffset = 0, charsOnLastLine = 0, buttonTrigger = 0, lastPressed, matrixTimer = 1200;
char messageBuffer[4000] = "Hinckle Dinkle fuckle derp\nWhat the fuck are you read\nI am whom I am because I can define reality\nBut on thursdays, it should be known that you are a cunt\nYeah well, so are you.";
bool CheckStart();
bool CheckStop();
void GuiInputHandler();
void InitializeGUI();
void MainMenuInputHandler();
void PushCharOntoBuffer(char c);
void PushStringOntoBuffer(char *str);
void MatrixAnimation();

bool FrameFunc() // Recursive function, called 100 times a second
{
	if (CheckStart())
	{
		serverObj.start();

		matrixTheme = hge->Stream_Load("matrix\\theme.mp3");
		hge->Stream_Play(matrixTheme, true);
		PushStringOntoBuffer("****SERVER STARTING****");
	}
	if (CheckStop())
	{
		serverObj.stop(); 

		hge->Stream_Free(matrixTheme);
		PushStringOntoBuffer("****SERVER STOPPING****");
	}

	MainMenuInputHandler();
	return false;
}

void PushStringOntoBuffer(char *str)
{
	int size = strlen(str);
	if (size > 4000) return;

	for (int x = size-1; x >= 0; x--)
	{
		PushCharOntoBuffer(str[x]);
	}
	PushCharOntoBuffer('\n');
}

bool CheckStart()
{	
	if (buttonTrigger == START)
	{
		buttonTrigger = 0;
		return true;
	}
	return false;
}	
	
bool CheckStop()
{
	if (buttonTrigger == STOP)
	{
		buttonTrigger = 0;
		return true;
	}
	return false;
}

// Renders
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	if (lastPressed == START)
	{
		text->SetColor(0xFFFFFFFF);
		text->printf(400, 425, HGETEXT_CENTER, "Server is running.");
	}
	else
	{
		text->SetColor(0xFF000000);
		text->printf(400, 425, HGETEXT_CENTER, "Server is not running.");
	}

	// Background
	if (lastPressed == START)
		MatrixAnimation();
	else
		box->Render(0,0);

	// Buttons
	box->RenderEx(100, 400, 0, 0.20f, 0.15f);
	box->RenderEx(535, 400, 0, 0.20f, 0.15f);

	// Text
	text->SetScale(1.0f);
	text->printf(400, 50, HGETEXT_CENTER, "Code Cracker - SERVER");
	text->SetScale(0.75f);
	text->printf(400, 125, HGETEXT_CENTER, "Message Box");
	text->printf(400, 525, HGETEXT_CENTER, "NOTE: Use arrow keys to navigate Message Box");

	text->SetColor(0xFF000000);

	// Message Box
	textBox->RenderEx(200, 150, 0, 0.5f, 0.4f);
	text->SetScale(0.625f);
	char toPrint[41];
	int readChars = yOffset;

	for (int y = 0; y < 10; y++)
	{
		int xsPassed = 0;
		for (int x = 0; x < 40; x++)
		{
			toPrint[x] = messageBuffer[readChars];
			if (messageBuffer[readChars+1] == '\n')
			{
				x = 40;
			}
			readChars++;
			xsPassed++;
		}

		for (int z = 0; z < 40; z++)
		{
			if (toPrint[z] == '\n')
				{toPrint[z] = '>';}
		}
		toPrint[xsPassed] = '\0';
		text->printf(225,(float)165+(y*13), HGETEXT_LEFT, "%s",toPrint);
	}

	// GUI
	text->SetScale(1.0f);
	gui->Render();
	
	hge->Gfx_EndScene();
	return false;
}

void PushCharOntoBuffer(char c)
{
	char temp[4000]; temp[0] = c;
	for (int x = 1; x < 4000; x++)
		{temp[x] = messageBuffer[x-1];}
	for (int x = 0; x < 4000; x++)
		{messageBuffer[x] = temp[x];}
}

void MainMenuInputHandler()
{
	float dt=hge->Timer_GetDelta();static float t=0.0f;int id;static int lastid=0;
	if(hge->Input_GetKeyState(HGEK_ESCAPE)) { lastid=5; gui->Leave(); }
	id=gui->Update(dt);
	if(id == -1)
	{
		switch(lastid)
		{
			case 1:
				gui->Enter();
				if (lastPressed != lastid)
				{
					lastPressed = lastid;
					buttonTrigger = lastid;
					hge->Effect_Play(startSound);
				}
				break;
			case 2:
				gui->Enter();
				if (lastPressed != lastid)
				{
					lastPressed = lastid;
					buttonTrigger = lastid;
					hge->Effect_Play(stopSound);
				}
				break;
			case 3:
				for (int x = yOffset-1; x > 0; x--)
				{
					if (x-1 == 0)
					{
						yOffset = 0;
						return;
					}
					if (messageBuffer[x] == '\n')
					{
						yOffset = x;
						x = 0;
					}
				}
				hge->Effect_Play(scrollSound);
				gui->Enter();
				break;
			case 4:
				for (int x = yOffset+1; x < 4000; x++)
				{
					if (messageBuffer[x] == '\n')
					{
						yOffset = x;
						x = 4000;
					}
				}
				hge->Effect_Play(scrollSound);
				gui->Enter();
				break;
		}
	}
	else if(id) { lastid=id; gui->Leave(); }

	switch(hge->Input_GetKey())
	{
		case HGEK_DOWN:
			for (int x = yOffset+1; x < 4000; x++)
			{
				if (messageBuffer[x] == '\n')
				{
					yOffset = x;
					x = 4000;
				}
				
			}
			hge->Effect_Play(scrollSound);
			break;
		case HGEK_UP:
			for (int x = yOffset-1; x > 0; x--)
			{
				if (x-1 == 0)
				{
					yOffset = 0;
					return;
				}
				if (messageBuffer[x] == '\n')
				{
					yOffset = x;
					x = 0;
				}
			}
			hge->Effect_Play(scrollSound);
			break;
	}

	return;
}

void InitializeGUI() 
{
	gui = new hgeGUI();
	gui->AddCtrl(new hgeGUIMenuItem(1, text, 9, 177, 430, 0.0f,"Start"));
	gui->AddCtrl(new hgeGUIMenuItem(2, text, 9, 615, 430, 0.0f,"Stop"));
	gui->AddCtrl(new hgeGUIMenuItem(3, text, 9, 125, 165, 0.0f,"UP"));
	gui->AddCtrl(new hgeGUIMenuItem(4, text, 9, 125, 265, 0.0f,"DOWN"));
	gui->SetNavMode(HGEGUI_CYCLED);
	gui->SetCursor(cursor);
	gui->SetFocus(1);
	gui->Enter();
}

void MatrixAnimation()
{
	matrixTimer-=50;

	if (matrixTimer == 100)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture1, 0, 0, 1024, 756);}
	else if (matrixTimer == 200)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture2, 0, 0, 1024, 756);}
	else if (matrixTimer == 300)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture3, 0, 0, 1024, 756);}
	else if (matrixTimer == 400)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture4, 0, 0, 1024, 756);}
	else if (matrixTimer == 500)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture5, 0, 0, 1024, 756);}
	else if (matrixTimer == 600)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture6, 0, 0, 1024, 756);}
	else if (matrixTimer == 700)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture7, 0, 0, 1024, 756);}
	else if (matrixTimer == 800)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture8, 0, 0, 1024, 756);}
	else if (matrixTimer == 900)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture9, 0, 0, 1024, 756);}
	else if (matrixTimer == 1000)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture10, 0, 0, 1024, 756);}
	else if (matrixTimer == 1100)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture11, 0, 0, 1024, 756);}
	else if (matrixTimer == 1200)
	{delete matrixSprite; matrixSprite = new hgeSprite(matrixTexture12, 0, 0, 1024, 756);}
	
	if (matrixTimer == 0)
		matrixTimer = 1200;

	matrixSprite->RenderStretch(0,0,800,600);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Code Cracker - Server");
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_USESOUND, true);
	hge->System_SetState(HGE_FXVOLUME, 5);
	hge->System_SetState(HGE_MUSVOLUME, 15);
	hge->System_SetState(HGE_STREAMVOLUME, 15);
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_SHOWSPLASH, false);
	hge->System_SetState(HGE_DONTSUSPEND, true);

	if(hge->System_Initiate())
	{
		text = new hgeFont("texture\\font1.fnt"); text->SetColor(0xFF000000);
		boxTexture = hge->Texture_Load("texture\\box.png");
		chatboxTexture = hge->Texture_Load("texture\\text_box.jpg");
		cursorTexture = hge->Texture_Load("texture\\cursor.png");
		startSound = hge->Effect_Load("sounds\\startSound.wav");
		scrollSound = hge->Effect_Load("sounds\\scrollSound.wav");
		stopSound = hge->Effect_Load("sounds\\stopSound.wav");
		matrixTexture1 = hge->Texture_Load("matrix\\1.png");
		matrixTexture2 = hge->Texture_Load("matrix\\2.png");
		matrixTexture3 = hge->Texture_Load("matrix\\3.png");
		matrixTexture4 = hge->Texture_Load("matrix\\4.png");
		matrixTexture5 = hge->Texture_Load("matrix\\5.png");
		matrixTexture6 = hge->Texture_Load("matrix\\6.png");
		matrixTexture7 = hge->Texture_Load("matrix\\7.png");
		matrixTexture8 = hge->Texture_Load("matrix\\8.png");
		matrixTexture9 = hge->Texture_Load("matrix\\9.png");
		matrixTexture10 = hge->Texture_Load("matrix\\10.png");
		matrixTexture11 = hge->Texture_Load("matrix\\11.png");
		matrixTexture12 = hge->Texture_Load("matrix\\12.png");
		matrixTheme = hge->Stream_Load("matrix\\theme.mp3");

		matrixSprite = new hgeSprite(matrixTexture1, 0, 0, 1024, 756);
		cursor = new hgeSprite(cursorTexture, 0, 0, 32, 32);
		box = new hgeSprite(boxTexture, 0, 0, 800, 600);
		textBox = new hgeSprite(chatboxTexture, 0, 0, 800, 397);

		InitializeGUI();
		hge->System_Start();
	}

	hge->System_Shutdown();
	hge->Release();
	return 0;
}