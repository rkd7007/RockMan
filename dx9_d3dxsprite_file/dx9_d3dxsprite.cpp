//-----------------------------------------------------------------------------
//           Name: dx9_d3dxsprite.cpp
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: This sample demonstrates how to create a animated 2D sprite 
//                 using D3DXSprite which is hardware accelerated and fully 
//                 compatible with 3D generated content.
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "resource.h"
#include "MyAnimationSprite.h"
#include "MyMapSprite.h"
#include "MyMenuSprite.h"
#include "MyPlayerHpSprite.h"
#include "MyReadyStartSprite.h"
#include "MyAttackSprite.h"
#include "BossAnimationSprite.h"
#include "BossAttackSprite.h"
#include "BossHpSprite.h"
#include "MyGameEndSprite.h"

#pragma comment(lib, "winmm.lib")
#include <MMSystem.h>
#include "mmsystem.h"
#include "Digitalv.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;

float  g_fElpasedTime;
double g_dCurTime;
double g_dLastTime;
float  g_fMoveSpeed = 50.0f;
float  g_fSpinX = 0.0f;
float  g_fSpinY = 0.0f;

int g_stage = 0; //0 = 메뉴화면, 1 = 인게임, 2 = 게임종료
bool isStay = true;
bool isHit = true;
bool Die = false;
int g_HP = 100;
int b_HP = 100;

CMyAnimationSprite g_aniSprite; //player
CMyMapSprite g_InGameMap; //map
CMyMenuSprite g_Menu; //main menu
CMyPlayerHpSprite g_playerHp; //player hp
CMyReadyStartSprite g_readyStart; //ready start ui
CMyAttackSprite g_playerAttack; //player attack
CMyGameEndSprite g_gameEnd; //game over ui (win, lose)

CBossAnimationSprite b_bossSprite; //boss
CBossAttackSprite b_bossAttack; //boss attack
CBossHpSprite b_bossHp; //boss hp

//사운드
MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;
int dwID;

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void shutDown(void);
void render(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

    memset(&uMsg,0,sizeof(uMsg));
    
	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
		                     "Direct3D 기말과제 - 20182311 강아름 - ROCKMAN",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					         0, 0, 1026, 600, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		{
			g_dCurTime     = timeGetTime();
			g_fElpasedTime = (float)((g_dCurTime - g_dLastTime) * 0.001);
			g_dLastTime    = g_dCurTime;

			render();
		}
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;

    switch( msg )
	{	
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_UP:
				if(!isStay && !Die) g_aniSprite.KeyDownUp(true, 'U');
				break;
			case VK_DOWN:
				if (!isStay && !Die) g_aniSprite.KeyDownUp(true, 'D');
				break;
			case VK_LEFT:
				if (!isStay && !Die) g_aniSprite.KeyDownUp(true, 'L');
				break;
			case VK_RIGHT:
				if (!isStay && !Die) g_aniSprite.KeyDownUp(true, 'R');
				break;
			case VK_SPACE: //점프
				if (!isStay && !Die && g_aniSprite.JumpCheck() == FALSE)
				{
					g_aniSprite.KeyDownUp(true, 'S');
					g_aniSprite.Jumping();
				}
				break;
			case VK_SHIFT: //공격 (점프 중 일때는 공격할 수 없다, 총알이 나가는 중에는 재 공격이 불가하다. (총알은 한 발만 나간다.))
				if (!isStay && !Die && g_aniSprite.JumpCheck() == FALSE && g_playerAttack.Attacking() == FALSE)
				{
					//sndPlaySoundA("attack.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_aniSprite.KeyDownUp(true, 'A');
					g_playerAttack.AttackPostion(g_aniSprite.xPos, g_aniSprite.yPos);
					g_playerAttack.PlayAttack();
				}
				break;
			}
		}
        break;

		case WM_KEYUP:
		{
			switch (wParam)
			{
			case VK_UP:
				g_aniSprite.KeyDownUp(false, 'U');
				g_aniSprite.ChangeAnitype(1);
				break;
			case VK_DOWN:
				g_aniSprite.KeyDownUp(false, 'D');
				g_aniSprite.ChangeAnitype(1);
				break;
			case VK_LEFT:
				g_aniSprite.KeyDownUp(false, 'L');
				g_aniSprite.ChangeAnitype(1);
				break;
			case VK_RIGHT:
				g_aniSprite.KeyDownUp(false, 'R');
				g_aniSprite.ChangeAnitype(1);
				break;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			//메뉴화면에서 클릭시 게임 중으로 변경
			if (g_stage == 0)
			{
				MessageBox(hWnd, "▶ 게임 방법 ◀\n※ ↑↓←→ : 이동\n※ SPACE : 점프\n※ SHIFT : 공격 ※\n※ 보스를 죽여라! ※", "DirectX 과제", MB_ICONWARNING | MB_OK);
				g_stage = 1;
			}

			ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD(lParam);
			ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD(lParam);
			bMousing = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			bMousing = false;
		}
		break;

		case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit.x = LOWORD (lParam);
			ptCurrentMousePosit.y = HIWORD (lParam);

			if( bMousing )
			{
				g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
				g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
			}
			
			ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
		}
		break;
		
		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	D3DCAPS9 d3dCaps;
	g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );

    D3DDISPLAYMODE d3ddm;
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
	
    d3dpp.Windowed               = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );

	   
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
                                1026.0f / 600.0f, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_aniSprite.Create(g_pd3dDevice, "Rockman.txt");
	g_InGameMap.Create(g_pd3dDevice, "Map.png");
	g_Menu.Create(g_pd3dDevice, "Menu.png");
	g_playerHp.Create(g_pd3dDevice, "PlayerHp.png");
	g_readyStart.Create(g_pd3dDevice, "ReadyStart.png");
	g_playerAttack.Create(g_pd3dDevice, "Rockman.png");
	b_bossSprite.Create(g_pd3dDevice, "Boss.txt");
	b_bossAttack.Create(g_pd3dDevice, "BossAttack.txt");
	b_bossHp.Create(g_pd3dDevice, "BossHp.png");
	g_gameEnd.Create(g_pd3dDevice, "gameEnd.png");

	//사운드
	mciOpen.lpstrElementName = "mainsound.mp3";
	mciOpen.lpstrDeviceType = "mpegvideo";
	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);
	dwID = mciOpen.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
 
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0 );

	g_pd3dDevice->BeginScene();

	//메뉴화면
	if (g_stage == 0)
		g_Menu.OnUpdate(g_fElpasedTime);
	//게임 중
	if (g_stage == 1)
	{
		g_InGameMap.OnUpdate(g_fElpasedTime); //map
		g_aniSprite.OnUpdate(g_fElpasedTime); //player
		g_playerHp.OnUpdate(g_fElpasedTime); //player hp
		g_playerAttack.OnUpdate(g_fElpasedTime); //player attack
		b_bossSprite.OnUpdate(g_fElpasedTime); //boss
		b_bossHp.OnUpdate(g_fElpasedTime); //boss hp

		//win 또는 lose ui 출력
		if (Die)
			g_gameEnd.OnUpdate(g_fElpasedTime);
	}

	//인게임에 돌입하면 카운트 다운 시작
	if (g_stage == 1 && isStay)
	{
		g_readyStart.OnUpdate(g_fElpasedTime); //ready start
		g_aniSprite.Stay();
		b_bossSprite.Stay();
		if (g_aniSprite.ReadyGo() == TRUE) isStay = false; //player가 움직일 수 있도록 한다
	}

	//boss attack. 보스가 공격 모션을 취하면, 스킬 생성 => 스킬 생성이 되고 있어도 보스는 Idle로 변환 됨 => 스킬 생성이 끝나면 (애니메이션이 끝나면) false로 바꿈
	if (b_bossSprite.isAttack1) { b_bossAttack.isAttack1 = true; b_bossAttack.OnUpdate(g_fElpasedTime); }
	if (b_bossSprite.isAttack2) { b_bossAttack.isAttack2 = true; b_bossAttack.OnUpdate(g_fElpasedTime); }

	if (!b_bossAttack.isAttack1) { b_bossSprite.isAttack1 = false; }
	if (!b_bossAttack.isAttack2) { b_bossSprite.isAttack2 = false; }

	//player가 boss의 공격에 맞음
	if (((g_aniSprite.isTrigger(b_bossAttack.m_rcCollision[0]) == TRUE) ||
		(g_aniSprite.isTrigger(b_bossAttack.m_rcCollision[1]) == TRUE) ||
		(g_aniSprite.isTrigger(b_bossAttack.m_rcCollision[2]) == TRUE)) && g_stage == 1 && isHit)
	{
		//sndPlaySoundA("hit1.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		g_HP -= 20;
		g_aniSprite.ChangePlayerHp(g_HP);
		g_aniSprite.HitStay();
		isHit = false;
	}
	if (g_aniSprite.HitTimer() == FALSE) isHit = true;

	//boss가 player의 공격에 맞음
	if ((b_bossSprite.isTrigger(g_playerAttack.m_rcCollision) == TRUE) && g_stage == 1)
	{
		//sndPlaySoundA("hit2.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		b_HP -= 20;
		b_bossSprite.ChangeBossHp(b_HP);
		b_bossSprite.Hit();
		g_playerAttack.PlayerHit(); //player가 총을 안 쏜 상태로 바꿈
	}

	//player hp sprite change, Game over
	if (g_aniSprite.PlayerHp(80) == TRUE) g_playerHp.ChangeHpSprite(80);
	if (g_aniSprite.PlayerHp(60) == TRUE) g_playerHp.ChangeHpSprite(60);
	if (g_aniSprite.PlayerHp(40) == TRUE) g_playerHp.ChangeHpSprite(40);
	if (g_aniSprite.PlayerHp(20) == TRUE) g_playerHp.ChangeHpSprite(20);
	if (g_aniSprite.PlayerHp(0) == TRUE)
	{ 
		g_playerHp.ChangeHpSprite(0);
		Die = true;
		g_aniSprite.End(true);
		b_bossSprite.End(false);
	}

	//boss hp sprite change, Game over
	if (b_bossSprite.BossHp(80) == TRUE) b_bossHp.ChangeHpSprite(80);
	if (b_bossSprite.BossHp(60) == TRUE) b_bossHp.ChangeHpSprite(60);
	if (b_bossSprite.BossHp(40) == TRUE) b_bossHp.ChangeHpSprite(40);
	if (b_bossSprite.BossHp(20) == TRUE) b_bossHp.ChangeHpSprite(20);
	if (b_bossSprite.BossHp(0) == TRUE) 
	{
		b_bossHp.ChangeHpSprite(0);
		Die = true;
		g_gameEnd.bWin(); //win ui
		g_aniSprite.End(false);
		b_bossSprite.End(true);
	}

    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

