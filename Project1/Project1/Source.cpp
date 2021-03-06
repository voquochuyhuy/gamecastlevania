﻿

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "GameTime.h"
#include "Simon.h"
#include "Ground.h"
#include "ViewPort.h"
#include"define.h"
#include "Zombie.h"
#include"BigFire.h"
#include"Candle.h"



CGame *game;

Simon * simon;
ViewPort *vp = new ViewPort(0, 0);
static float h = 0, t = 0;
vector<LPGAMEOBJECT> objects;

class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetLevel(SIMON_LEVEL_BIG);
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else
		simon->SetState(SIMON_STATE_IDLE);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_SIMON, L"Castlevania\\Simon_right.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SIMON_2, L"Castlevania\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_LV1, L"Castlevania\\lv1.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_LV1_2, L"Castlevania\\lv2.png", D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_BRICK, L"Castlevania\\Brick.png", D3DCOLOR_XRGB(3, 26, 110));
	textures->Add(ID_TEX_ZOMBIE, L"Castlevania\\ZOMBIE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_PANTHER, L"Castlevania\\PANTHER.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_FIRE, L"Castlevania\\123.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"Castlevania\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WHIP, L"Castlevania\\WHIP.png", D3DCOLOR_XRGB(255, 0, 255));
	
	
	
	


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	
	sprites->Add(10001, 434, 0, 469, 64, texSimon);		// đứng im phải

	sprites->Add(10002, 375, 0, 401, 64, texSimon);		// đi phải
	sprites->Add(10003, 314, 0, 344, 64, texSimon);
	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);
	sprites->Add(10011, 11, 0, 47, 64, texSimon2);		// đứng im trái

	sprites->Add(10012, 74, 0, 107, 64, texSimon2);		// đi trái
	sprites->Add(10013, 132, 0, 171, 64, texSimon2);

	
	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 
	
	sprites->Add(10021, 247, 0, 259, 15, texSimon);			// idle small right
	sprites->Add(10022, 275, 0, 291, 15, texSimon);			// walk 
	sprites->Add(10023, 306, 0, 320, 15, texSimon);			// 

	sprites->Add(10031, 187, 0, 198, 15, texSimon);			// idle small left

	sprites->Add(10032, 155, 0, 170, 15, texSimon);			// walk
	sprites->Add(10033, 125, 0, 139, 15, texSimon);
	

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 32, 32, texMisc);



	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	sprites->Add(30002, 32, 0, 68, 64, texEnemy); 

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 50, 32, texEnemy1); // nằm chờ
	sprites->Add(30012, 64, 0, 120, 32, texEnemy1);
	sprites->Add(30013, 126, 0, 188, 32, texEnemy1);
	sprites->Add(30014, 188, 0, 256, 32, texEnemy1);
	
	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2); 
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);
	

	

	LPANIMATION ani;

	ani = new CAnimation(100);	
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(100);	// idle small right
	ani->Add(10021);
	animations->Add(402, ani);

	ani = new CAnimation(100);	// idle small left
	ani->Add(10031);
	animations->Add(403, ani);

	

	ani = new CAnimation(100);	
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	animations->Add(500, ani);

	ani = new CAnimation(100);	
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);
	ani = new CAnimation(100);	// walk right small
	ani->Add(10021);
	ani->Add(10022);
	ani->Add(10023);
	animations->Add(502, ani);

	ani = new CAnimation(100);	// walk left small
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(503, ani);

	


	ani = new CAnimation(100);		
	ani->Add(10099);
	animations->Add(599, ani);



	ani = new CAnimation(100);		
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100);
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(602, ani);

	ani = new CAnimation(100);
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);
	

	simon = new Simon();
	simon->AddAnimation(400);		
	simon->AddAnimation(401);
	simon->AddAnimation(402);		// idle right small
	simon->AddAnimation(403);
	

	simon->AddAnimation(500);		
	simon->AddAnimation(501);	
	simon->AddAnimation(502);		// walk right small
	simon->AddAnimation(503);
	

	simon->AddAnimation(599);		
	
	simon->SetPosition(150, 327);
	objects.push_back(simon);
	for (int i = 0; i < 50; i++)
	{
		Ground *ground = new Ground();
		ground->AddAnimation(601);
		ground->SetPosition(0 + i * 16.0f, 407);
		objects.push_back(ground);
	}
	/*for (int i = 0; i < 2; i++)
	{
		Zombie *zombie = new Zombie();
		zombie->AddAnimation(602);
		zombie->SetPosition(0 + i *32, 350);
		zombie->SetState(ZOMBIE_STATE_WALKING);
		objects.push_back(zombie);
	}
	/*
		BigFire *bigfire = new BigFire();
		bigfire->AddAnimation(700);
		bigfire->SetPosition(350, 350);
		objects.push_back(bigfire);
		BigFire *bigfire1 = new BigFire();
		bigfire1->AddAnimation(700);
		bigfire1->SetPosition(481, 350);
		objects.push_back(bigfire1);
		BigFire *bigfire2 = new BigFire();
		bigfire2->AddAnimation(700);
		bigfire2->SetPosition(672, 350);
		objects.push_back(bigfire2);
		BigFire *bigfire3 = new BigFire();
		bigfire3->AddAnimation(700);
		bigfire3->SetPosition(866, 350);
		objects.push_back(bigfire3);
		BigFire *bigfire4 = new BigFire();
		bigfire4->AddAnimation(700);
		bigfire4->SetPosition(1104, 350);
		objects.push_back(bigfire4);
		BigFire *bigfire5 = new BigFire();
		bigfire5->AddAnimation(700);
		bigfire5->SetPosition(1282, 350);
		objects.push_back(bigfire5);*/
	

	


	

	

}


void Update(DWORD dt)
{
	
	
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
}


void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		
		CTextures *textures = CTextures::GetInstance();
		float x, y;
		simon->GetPosition(x, y);// object world
		

		
		
		if (x >= SCREEN_WIDTH / 2 && x < 1536-SCREEN_WIDTH/2 - SIMON_BIG_BBOX_WIDTH)

		{
			simon->ViewPortInWorld();
			
			
			D3DXVECTOR3 p(0, 120, 0);
			RECT r;
			r.left = x- SCREEN_WIDTH/2;
			r.top = 36;
			r.right = 640 + x - SCREEN_WIDTH/2;
			r.bottom = 480;			
			spriteHandler->Draw(textures->Get(ID_TEX_LV1), &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
			
			simon->SetPositionInViewPort();
			//simon->Render();
			//simon->SetPosition(simon->z, simon->t);
			//simon->SetPositioninViewPort(x - simon->viewportInWorld(x-SCREENWIDTH/2),407);
			
		}
		
		else if (x >= 1408 - SIMON_BIG_BBOX_WIDTH)
		{
			D3DXVECTOR3 u(0, 120, 0);
			RECT y;
			y.left = 0;
			y.top = 32;
			y.right = 640;
			y.bottom = 480;
			spriteHandler->Draw(textures->Get(ID_TEX_LV1_2), &y, NULL, &u, D3DCOLOR_XRGB(255, 255, 255));
			
		}
		
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
		
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	//GameTime *time = new GameTime;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH , SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}