 #include "..\..\include\hge.h"
#include "..\..\include\hgecolor.h"
#include "..\..\include\hgesprite.h"
#include "..\..\include\hgedistort.h"
#include "..\..\include\hgefont.h"
#include "..\..\include\hgegui.h"
#include "CrackingTheCodeClient.h"
#include "menuitem.h"
#include "windows.h"
#include <process.h>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

#define RUN 1
#define REQUEST 2
#define NEW 3

HTEXTURE boxTexture, chatboxTexture, cursorTexture, matrixTexture1, matrixTexture2, matrixTexture3, matrixTexture4, matrixTexture5, matrixTexture6, 
matrixTexture7, matrixTexture8, matrixTexture9, matrixTexture10, matrixTexture11, matrixTexture12;
HEFFECT scrollSound, startSound, stopSound;
HMUSIC matrixTheme;
hgeSprite *box, *textBox, *cursor, *matrixSprite;
hgeGUI	*gui; hgeGUI *guicp;
hgeFont	*text;

char dir[25];
char email[25];
char title[25];
char buffer[26];
int textboxcounter = 0;

HGE *hge = 0;
CrackingTheCodeClient clientObj;

int CURRENT_PAGE = 0;
enum curpage
{
	FRONT_PAGE = 0,
	RUNNING = 1,
	CREATING_PROCESS = 2
};

int CURRENT_ENTRY = 0;
enum createprocs
{
	ENTERING_DIRECTORY = 1,
	ENTERING_EMAIL = 2,
	ENTERING_TITLE = 3
};

int yOffset = 0, charsOnLastLine = 0, buttonTrigger = 0, lastPressed, matrixTimer = 1200;
char messageBuffer[4000] = "\nWelcome to the Code Cracker Client!\n\nSelect 'Run' to connect to the server and assist in the current task\n\nSelect 'New' to create a process if one is not already running on the server.";
bool CheckRun();
bool CheckNew();
void GuiInputHandler();
void InitializeGUI();
void MainMenuInputHandler();
void CreatingProcessInputHandler();
void TextBoxInputHandler();
void PushCharOntoBuffer(char c);
void PushStringOntoBuffer(char *str);
void MatrixAnimation();
void SubmitRun();
void SubmitRequest();
void SubmitProcess();















void SubmitRun()
{
	matrixTheme = hge->Stream_Load("matrix\\theme.mp3");
	hge->Stream_Play(matrixTheme, true);
	PushStringOntoBuffer("****CLIENT IS RUNNING****");
	clientObj.crackCode();
}

void SubmitRequest()
{
	PushStringOntoBuffer("--code for request here derp--");
}

void SubmitProcess()
{
	clientObj.startProcess(title,email,dir);
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

bool CheckRun()
{	
	if (buttonTrigger == RUN)
	{
		buttonTrigger = 0;
		return true;
	}
	return false;
}	
	
bool CheckNew()
{
	if (buttonTrigger == NEW)
	{
		buttonTrigger = 0;
		return true;
	}
	return false;
}

bool CheckRequest()
{
	if (buttonTrigger == REQUEST)
	{
		buttonTrigger = 0;
		return true;
	}
	return false;
}
int curserPulse = 0;
bool FrameFunc() // Recursive function, called 100 times a second
{
	if (CheckRun())
	{
		SubmitRun();
	}
	if (CheckRequest())
	{
		SubmitRequest();
	}

	if (CURRENT_PAGE != CREATING_PROCESS)
		MainMenuInputHandler();
	if (CURRENT_PAGE == CREATING_PROCESS)
	{
		if (curserPulse > 30)
		{
			buffer[textboxcounter] = '|';
			if (curserPulse > 60)
				curserPulse = 0;
		}
		else
			buffer[textboxcounter] = 0;

		curserPulse+=1;

		CreatingProcessInputHandler();
	}
	return false;
}

// Renders
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	// Background
	if (CURRENT_PAGE == RUNNING)
		MatrixAnimation();
	else
		box->Render(0,0);

	if (CURRENT_PAGE != CREATING_PROCESS)
	{
		// Buttons
		box->RenderEx(100, 400, 0, 0.20f, 0.15f);
		box->RenderEx(317.5f, 400, 0, 0.20f, 0.15f);
		box->RenderEx(535, 400, 0, 0.20f, 0.15f);

		// Text
		if (CURRENT_PAGE != RUNNING)
		{
			text->SetScale(1.0f);
			text->printf(400, 50, HGETEXT_CENTER, "Code Cracker - CLIENT");
			text->SetScale(0.75f);
			text->printf(400, 125, HGETEXT_CENTER, "Message Box");
			text->printf(400, 525, HGETEXT_CENTER, "NOTE: Use arrow keys to navigate Message Box");
		}

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
			text->printf(225,(float)165+(y*13), HGETEXT_LEFT, "%s", toPrint);
		}

		// GUI
		text->SetScale(1.0f);
		gui->Render();
	}
	else
	{
		if (CURRENT_ENTRY == ENTERING_DIRECTORY)
		{
			strncpy(dir, buffer, textboxcounter);
			text->SetScale(0.65f);
			text->printf(400, 450, HGETEXT_CENTER, "Press ENTER to finish\nPress ESC to cancel\nPress TAB to cycle through entries.\n\nDIRECTORY: Leave blank to load from working directory");
		}
		else if (CURRENT_ENTRY == ENTERING_EMAIL)
		{
			strncpy(email, buffer, textboxcounter);
			text->SetScale(0.65f);
			text->printf(400, 450, HGETEXT_CENTER, "Press ENTER to finish\nPress ESC to cancel\nPress TAB to cycle through entries.\n\nE-MAIL: Enter your e-mail if you want updates from the server sent to you.");
		}
		else if (CURRENT_ENTRY == ENTERING_TITLE)
		{
			strncpy(title, buffer, textboxcounter);
			text->SetScale(0.65f);
			text->printf(400, 450, HGETEXT_CENTER, "Press ENTER to finish\nPress ESC to cancel\nPress TAB to cycle through entries.\n\nTITLE: This is what your process will be named as.");
		}
		else 
		{
			CURRENT_ENTRY = 1;
		}

		text->SetScale(1.0f);
		text->printf(400, 50, HGETEXT_CENTER, "CREATING NEW PROCESS");

		//dir
		textBox->RenderEx(320, 150, 0, 0.2f, 0.075f);
		if (CURRENT_ENTRY == ENTERING_DIRECTORY)
			{text->SetScale(0.5f); text->printf(325, 160, HGETEXT_LEFT, buffer);}
		else
			{text->SetScale(0.5f); text->printf(325, 160, HGETEXT_LEFT, dir);}

		//email
		textBox->RenderEx(320, 250, 0, 0.2f, 0.075f);
		if (CURRENT_ENTRY == ENTERING_EMAIL)
			{text->SetScale(0.5f); text->printf(325, 260, HGETEXT_LEFT, buffer);}
		else
			{text->SetScale(0.5f); text->printf(325, 260, HGETEXT_LEFT, email);}

		//title
		textBox->RenderEx(320, 350, 0, 0.2f, 0.075f);
		if (CURRENT_ENTRY == ENTERING_TITLE)
			{text->SetScale(0.5f); text->printf(325, 360, HGETEXT_LEFT, buffer);}
		else
			{text->SetScale(0.5f); text->printf(325, 360, HGETEXT_LEFT, title);}

		text->SetScale(0.75f); guicp->Render();

	}

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

void CreatingProcessInputHandler()
{
	switch(hge->Input_GetKey())
	{
		case HGEK_TAB:
			CURRENT_ENTRY++;

			for (int z = 0; z < 25; z++)
				{buffer[z] = 0;}

			if (CURRENT_ENTRY > 3) 
				CURRENT_ENTRY = 1;

			if (CURRENT_ENTRY == ENTERING_DIRECTORY)
			{
				for (int z = 0; z < 25; z++)
				{dir[z] = 0;}
			}
			else if (CURRENT_ENTRY == ENTERING_EMAIL)
			{
				for (int z = 0; z < 25; z++)
				{email[z] = 0;}
			}
			else if (CURRENT_ENTRY == ENTERING_TITLE)
			{
				for (int z = 0; z < 25; z++)
				{title[z] = 0;}
			}
			textboxcounter = 0;
			break;
		case HGEK_ENTER:
			if (dir[1] > 0 && email[1] > 0 && title[1] > 0)
			{
				SubmitProcess();
				PushStringOntoBuffer("Finished creating new process");
				CURRENT_PAGE = 0; // finished
				for (int z = 0; z < 25; z++)
				{dir[z] = 0; email[z] = 0; title[z] = 0;buffer[z] = 0;}
				textboxcounter = 0;
			}
			break;
		case HGEK_ESCAPE:
			CURRENT_PAGE = 0;
			textboxcounter = 0;
			for (int z = 0; z < 25; z++)
			{dir[z] = 0; email[z] = 0; title[z] = 0;buffer[z] = 0;}
			break;
	}

	float dt=hge->Timer_GetDelta();static float t=0.0f;int id;static int lastid2=0;
	id=guicp->Update(dt);
	if(id == -1)
	{
		if (lastid2 == 1 || lastid2 == 2 || lastid2 == 3)
		{
			guicp->Enter();
			CURRENT_ENTRY = lastid2;
			textboxcounter = 0;
			for (int z = 0; z < 25; z++)
				{buffer[z] = 0;}
			lastid2 = 0;
			if (CURRENT_ENTRY == ENTERING_DIRECTORY)
			{
				for (int z = 0; z < 25; z++)
				{dir[z] = 0;}
			}
			else if (CURRENT_ENTRY == ENTERING_EMAIL)
			{
				for (int z = 0; z < 25; z++)
				{email[z] = 0;}
			}
			else if (CURRENT_ENTRY == ENTERING_TITLE)
			{
				for (int z = 0; z < 25; z++)
				{title[z] = 0;}
			}
		}
	}
	else if(id) { lastid2=id; guicp->Leave(); }

	TextBoxInputHandler();
}

void TextBoxInputHandler()
{
	if (hge->Input_GetChar() > 0 && textboxcounter < 25 && hge->Input_GetChar() != HGEK_BACKSPACE 
		&& hge->Input_GetChar() != HGEK_ENTER && hge->Input_GetChar() != HGEK_ESCAPE && hge->Input_GetChar() != HGEK_UP && hge->Input_GetChar() != HGEK_DOWN
		&& hge->Input_GetChar() != HGEK_LEFT && hge->Input_GetChar() != HGEK_RIGHT && hge->Input_GetChar() != HGEK_TAB)
	{
		buffer[textboxcounter] = hge->Input_GetChar();
		textboxcounter++;
	}
	if (textboxcounter > 0 && hge->Input_GetChar() == HGEK_BACKSPACE)
	{
		buffer[textboxcounter] = 0;
		textboxcounter--;
	}
}

void MainMenuInputHandler()
{
	float dt=hge->Timer_GetDelta();static float t=0.0f;int id;static int lastid=0;

	id=gui->Update(dt);
	if(id == -1)
	{
		switch(lastid)
		{
			case RUN:
				gui->Enter();
				if (CURRENT_PAGE != RUNNING)
				{
					buttonTrigger = lastid;
					hge->Effect_Play(startSound);
					CURRENT_PAGE = RUNNING;
				}
				break;
			case REQUEST:
				gui->Enter();
				buttonTrigger = lastid;
				hge->Effect_Play(startSound);
				break;
			case NEW:
				gui->Enter();
				hge->Stream_Free(matrixTheme);
				CURRENT_PAGE = CREATING_PROCESS;
				CURRENT_ENTRY = ENTERING_DIRECTORY;
				break;
			case 4:
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
			case 5:
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
	gui->AddCtrl(new hgeGUIMenuItem(1, text, 9, 177, 430, 0.0f,"Run"));
	gui->AddCtrl(new hgeGUIMenuItem(2, text, 9, 396, 430, 0.0f,"Request"));
	gui->AddCtrl(new hgeGUIMenuItem(3, text, 9, 615, 430, 0.0f,"New"));
	gui->AddCtrl(new hgeGUIMenuItem(4, text, 9, 125, 165, 0.0f,"UP"));
	gui->AddCtrl(new hgeGUIMenuItem(5, text, 9, 125, 265, 0.0f,"DOWN"));
	gui->SetNavMode(HGEGUI_CYCLED);
	gui->SetCursor(cursor);
	gui->SetFocus(1);
	gui->Enter();

	guicp = new hgeGUI();
	guicp->AddCtrl(new hgeGUIMenuItem(1, text, 9, 410, 125, 0.0f,"DIRECTORY"));
	guicp->AddCtrl(new hgeGUIMenuItem(2, text, 9, 410, 225, 0.0f,"E-MAIL"));
	guicp->AddCtrl(new hgeGUIMenuItem(3, text, 9, 415, 325, 0.0f,"PROCESS TITLE"));
	guicp->SetNavMode(HGEGUI_CYCLED);
	guicp->SetCursor(cursor);
	guicp->SetFocus(1);
	guicp->Enter();
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