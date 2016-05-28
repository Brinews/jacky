// FREAKOUT.CPP - break game demo

// INCLUDES ///////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN // include all macros
#define INITGUID            // include all GUIDs 

#include <windows.h>        // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>

#include <iostream>       // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>          // directX includes
#include <d3dx9.h> 

#include "mybricks.h"       // game library includes

#include <tchar.h>

using namespace std;

// DEFINES ////////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME _T("WIN3DCLASS") // class name

#define WINDOW_WIDTH            640     // size of window
#define WINDOW_HEIGHT           480

// states for game loop
#define GAME_STATE_INIT         0
#define GAME_STATE_START_LEVEL  1
#define GAME_STATE_RUN          2
#define GAME_STATE_SHUTDOWN     3
#define GAME_STATE_EXIT         4 
#define GAME_STATE_LOSEIT		5
#define GAME_STATE_WIN			6

// block defines
#define NUM_BLOCK_ROWS          6
#define NUM_BLOCK_COLUMNS       8

#define BLOCK_WIDTH             64
#define BLOCK_HEIGHT            16
#define BLOCK_ORIGIN_X          8
#define BLOCK_ORIGIN_Y          8
#define BLOCK_X_GAP             80
#define BLOCK_Y_GAP             32

// paddle defines
#define PADDLE_START_X          (SCREEN_WIDTH/2 - 16)
#define PADDLE_START_Y          (SCREEN_HEIGHT - 32);
#define PADDLE_WIDTH            32
#define PADDLE_HEIGHT           8
#define PADDLE_COLOR            191

// ball defines
#define BALL_START_Y            (SCREEN_HEIGHT/2)
#define BALL_SIZE                4

// PROTOTYPES /////////////////////////////////////////////////

// game console
int Game_Init(void *parms=NULL);
int Game_Shutdown(void *parms=NULL);
int Game_Main(void *parms=NULL);

// GLOBALS ////////////////////////////////////////////////////

HWND main_window_handle  = NULL; // save the window handle
HINSTANCE main_instance  = NULL; // save the instance
int game_state           = GAME_STATE_INIT; // starting state

int paddle_x = 0, paddle_y = 0; // tracks position of paddle
int ball_x   = 0, ball_y   = 0; // tracks position of ball
int ball_dx  = 0, ball_dy  = 0; // velocity of ball
int score    = 0;               // the score
int level    = 1;               // the current level
int blocks_hit = 0;             // tracks number of blocks hit

// this contains the game grid data   

UCHAR blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLUMNS];     

// FUNCTIONS //////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd, 
		UINT msg, 
		WPARAM wparam, 
		LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   // handle to a device context

	// what is the message 
	switch(msg)
	{	
		case WM_CREATE: 
			{
				// do initialization stuff here
				return(0);
			} break;

		case WM_PAINT:
			{
				// start painting
				hdc = BeginPaint(hwnd,&ps);

				// the window is now validated 

				// end painting
				EndPaint(hwnd,&ps);
				return(0);
			} break;

		case WM_DESTROY: 
			{
				// kill the application			
				PostQuitMessage(0);
				return(0);
			} break;

		default:break;

	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

LPDIRECT3D9 g_pD3D = NULL; 
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; 
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL; // Buffer to hold vertices 

//定义顶点信息的结构体 
struct CUSTOMVERTEX 
{ 
    FLOAT x, y, z, rhw;     // The transformed position for the vertex. 
    DWORD colour;           // The vertex colour. 
}; 
  
//定义自由顶点格式 
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE) 
  
//定义释放COM对象的宏 
#define SafeRelease(pObject) if(pObject != NULL) {pObject->Release(); pObject=NULL;} 


//初始化D3D设备 
HRESULT InitialiseD3D(HWND hWnd) 
{ 
    //First of all, create the main D3D object. If it is created successfully we  
    //should get a pointer to an IDirect3D8 interface. 
    //取得D3D9的对象 
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION); 
    if(g_pD3D == NULL) 
    { 
        return E_FAIL; 
    } 
  
    //得到当前的显示模式 
    //Get the current display mode 
    D3DDISPLAYMODE d3ddm; 
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) 
    { 
        return E_FAIL; 
    } 
  
    //创建一个D3D设备 
    //Create a structure to hold the settings for our device 
    D3DPRESENT_PARAMETERS d3dpp;  
    ZeroMemory(&d3dpp, sizeof(d3dpp)); 
  
    //Fill the structure.  
    //We want our program to be windowed, and set the back buffer to a format 
    //that matches our current display mode 
    d3dpp.Windowed = TRUE;//全屏模式还是窗口模式 
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//后台缓冲区复制到前台时,清除后台缓冲区内容 
    d3dpp.BackBufferFormat = d3ddm.Format;//屏幕的显示模式 
  
    //创建一个Direct3D设备 
    //Create a Direct3D device. 
    if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,  
                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) 
    { 
        return E_FAIL; 
    } 
      
    return S_OK; 
} 
  
HRESULT InitialiseVertexBuffer() 
{ 
    VOID* pVertices; 
      
    //顶点信息数组 
    //Store each point of the triangle together with it's colour 
    CUSTOMVERTEX cvVertices[] = 
    { 
        {250.0f, 100.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0),},    //Vertex 1 - Red    (250, 100) 
        {400.0f, 350.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0),},    //Vertex 2 - Green  (400, 350) 
        {100.0f, 350.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255),},    //Vertex 3 - Blue   (100, 350) 
    }; 
  
    //通过设备创建顶点缓冲 
    //Create the vertex buffer from our device 
    if(FAILED(g_pD3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 
                                               0, D3DFVF_CUSTOMVERTEX, 
                                               D3DPOOL_DEFAULT, &g_pVertexBuffer,NULL))) 
    { 
        return E_FAIL; 
    } 
  
    //锁定顶点缓冲，并得到一个存放顶点信息的缓冲区的指针 
    //Get a pointer to the vertex buffer vertices and lock the vertex buffer 
    if(FAILED(g_pVertexBuffer->Lock(0, sizeof(cvVertices), (void**)&pVertices, 0))) 
    { 
        return E_FAIL; 
    } 
  
    //复制顶点信息 
    //Copy our stored vertices values into the vertex buffer 
    memcpy(pVertices, cvVertices, sizeof(cvVertices)); 
  
    //解锁顶点缓冲区 
    //Unlock the vertex buffer 
    g_pVertexBuffer->Unlock(); 
  
    return S_OK; 
} 


void CleanUp() 
{ 
    SafeRelease(g_pVertexBuffer); 
    SafeRelease(g_pD3DDevice); 
    SafeRelease(g_pD3D); 
}

// WINMAIN ////////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
		HINSTANCE hprevinstance,
		LPSTR lpcmdline,
		int ncmdshow)
{
	// this is the winmain function

	WNDCLASS winclass;	// this will hold the class we create
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	PAINTSTRUCT ps;     // generic paintstruct

	// first fill in the window class stucture
	winclass.style			= CS_CLASSDC; //CS_DBLCLKS | CS_OWNDC |  CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= GetModuleHandle(NULL);//hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= NULL;//(HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// create the window, note the use of WS_POPUP
	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME,    // class
					_T("WIN3D Game"),	// title
					WS_OVERLAPPEDWINDOW, //WS_POPUP | WS_VISIBLE,
					0,0,	                // initial x,y
					WINDOW_WIDTH, //GetSystemMetrics(SM_CXSCREEN)-100,  // intial width
					WINDOW_HEIGHT, //GetSystemMetrics(SM_CYSCREEN)-100,  // initial height
					GetDesktopWindow(), //NULL,	    // handle to parent 
					NULL,	    // handle to menu
					hinstance,
					NULL)))	// creation parms
		return(0);

	// hide mouse
	ShowCursor(FALSE);

	// save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance      = hinstance;

	// perform all game console specific initialization
	Game_Init();

	//Initialize Direct3D 
    //初始化Direct3D 
    if(SUCCEEDED(InitialiseD3D(hwnd))) 
    {  
        //Show our window 
        //显示窗口 
        ShowWindow(hwnd, SW_SHOWDEFAULT); 
        UpdateWindow(hwnd); 
  
        //Initialize Vertex Buffer 
        //初始化顶点缓冲 
        //if(SUCCEEDED(InitialiseVertexBuffer())) 
        { 
            //Start game running: Enter the game loop 
            //开始游戏: 进入游戏循环 

			// enter main event loop
			while(1)
			{
				if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				{ 
					// test if this is a quit
					if (msg.message == WM_QUIT)
						break;

					// translate any accelerator keys
					TranslateMessage(&msg);

					// send the message to the window proc
					DispatchMessage(&msg);
				} // end if

				// main game processing goes here
				Game_Main();

			} // end while

		} 
	} 
	// shutdown game and release all resources
	Game_Shutdown();

	// show mouse
	ShowCursor(TRUE);

	CleanUp();
	UnregisterClass(_T("Win3D Game"), winclass.hInstance); 

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

// T3DX GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////////

int Game_Init(void *parms)
{
	// this function is where you do all the initialization 
	// for your game


	// return success
	return(1);

} // end Game_Init

///////////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated


	// return success
	return(1);

} // end Game_Shutdown

///////////////////////////////////////////////////////////////

void Init_Blocks(void)
{
	// initialize the block field
	for (int row=0; row < NUM_BLOCK_ROWS; row++)
		for (int col=0; col < NUM_BLOCK_COLUMNS; col++)
			blocks[row][col] = row*16+col*3+16;

} // end Init_Blocks

///////////////////////////////////////////////////////////////

void Draw_Blocks(void)
{
	// this function draws all the blocks in row major form
	int x1 = BLOCK_ORIGIN_X, // used to track current position
		y1 = BLOCK_ORIGIN_Y; 

	// draw all the blocks
	for (int row=0; row < NUM_BLOCK_ROWS; row++)
	{    
		// reset column position
		x1 = BLOCK_ORIGIN_X;

		// draw this row of blocks
		for (int col=0; col < NUM_BLOCK_COLUMNS; col++)
		{
			// draw next block (if there is one)
			if (blocks[row][col]!=0)
			{
				// draw block     
				Draw_Rectangle(x1-4,y1+4,
						x1+BLOCK_WIDTH-4,y1+BLOCK_HEIGHT+4,0);

				Draw_Rectangle(x1,y1,x1+BLOCK_WIDTH,
						y1+BLOCK_HEIGHT,blocks[row][col]);
			} // end if

			// advance column position
			x1+=BLOCK_X_GAP;
		} // end for col

		// advance to next row position
		y1+=BLOCK_Y_GAP;

	} // end for row

} // end Draw_Blocks

///////////////////////////////////////////////////////////////

void Process_Ball(void)
{
	// this function tests if the ball has hit a block or the paddle
	// if so, the ball is bounced and the block is removed from 
	// the playfield note: very cheesy collision algorithm :)

	// first test for ball block collisions

	// the algorithm basically tests the ball against each 
	// block's bounding box this is inefficient, but easy to 
	// implement, later we'll see a better way

	int x1 = BLOCK_ORIGIN_X, // current rendering position
		y1 = BLOCK_ORIGIN_Y; 

	int ball_cx = ball_x+(BALL_SIZE/2),  // computer center of ball
		ball_cy = ball_y+(BALL_SIZE/2);

	// test of the ball has hit the paddle
	if (ball_y > (SCREEN_HEIGHT/2) && ball_dy > 0)
	{
		// extract leading edge of ball
		int x = ball_x+(BALL_SIZE/2);
		int y = ball_y+(BALL_SIZE/2);

		// test for collision with paddle
		if ((x >= paddle_x && x <= paddle_x+PADDLE_WIDTH) &&
				(y >= paddle_y && y <= paddle_y+PADDLE_HEIGHT))
		{
			// reflect ball
			ball_dy=-ball_dy;

			// push ball out of paddle since it made contact
			ball_y+=ball_dy;

			// add a little english to ball based on motion of paddle
			if (KEY_DOWN(VK_RIGHT))
				ball_dx-=(rand()%3);
			else
				if (KEY_DOWN(VK_LEFT))
					ball_dx+=(rand()%3);
				else
					ball_dx+=(-1+rand()%3);

			// test if there are no blocks, if so send a message
			// to game loop to start another level
			if (blocks_hit >= (NUM_BLOCK_ROWS*NUM_BLOCK_COLUMNS))
			{
				game_state = GAME_STATE_START_LEVEL;
				level++;
			} // end if

			// make a little noise
			MessageBeep(MB_OK);

			// return
			return; 

		} // end if

	} // end if

	// now scan thru all the blocks and see of ball hit blocks
	for (int row=0; row < NUM_BLOCK_ROWS; row++)
	{    
		// reset column position
		x1 = BLOCK_ORIGIN_X;

		// scan this row of blocks
		for (int col=0; col < NUM_BLOCK_COLUMNS; col++)
		{
			// if there is a block here then test it against ball
			if (blocks[row][col]!=0)
			{
				// test ball against bounding box of block
				if ((ball_cx > x1) && (ball_cx < x1+BLOCK_WIDTH) &&     
						(ball_cy > y1) && (ball_cy < y1+BLOCK_HEIGHT))
				{
					// remove the block
					blocks[row][col] = 0; 

					// increment global block counter, so we know 
					// when to start another level up
					blocks_hit++;

					// bounce the ball
					ball_dy=-ball_dy;

					// add a little english
					ball_dx+=(-1+rand()%3);

					// make a little noise
					MessageBeep(MB_OK);

					// add some points
					score+=5*(level+(abs(ball_dx)));

					// that's it -- no more block
					return;

				} // end if  

			} // end if

			// advance column position
			x1+=BLOCK_X_GAP;
		} // end for col

		// advance to next row position
		y1+=BLOCK_Y_GAP;

	} // end for row

} // end Process_Ball

///////////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	char buffer[128]; // used to print text

	// what state is the game in? 
	if (game_state == GAME_STATE_INIT)
	{
		// initialize everything here graphics
		DD_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

		// seed the random number generator
		// so game is different each play
		srand(Start_Clock());

		// set the paddle position here to the middle bottom
		paddle_x = PADDLE_START_X;
		paddle_y = PADDLE_START_Y;

		// set ball position and velocity
		ball_x = 8+rand()%(SCREEN_WIDTH-16);
		ball_y = BALL_START_Y;
		ball_dx = -4 + rand()%(8+1);
		ball_dy = 6 + rand()%2;

		// transition to start level state
		game_state = GAME_STATE_START_LEVEL;

	} // end if 
	////////////////////////////////////////////////////////////////
	else if (game_state == GAME_STATE_START_LEVEL)
	{
		// get a new level ready to run

		// initialize the blocks
		Init_Blocks();

		// reset block counter
		blocks_hit = 0;

		// transition to run state
		game_state = GAME_STATE_RUN;

	} // end if
	///////////////////////////////////////////////////////////////
	else if (game_state == GAME_STATE_RUN)
	{
		// start the timing clock
		Start_Clock();

		// clear drawing surface for the next frame of animation
		Draw_Rectangle(0,0,SCREEN_WIDTH-1, SCREEN_HEIGHT-1,200);

		// move the paddle
		if (KEY_DOWN(VK_RIGHT))
		{
			// move paddle to right
			paddle_x+=8;

			// make sure paddle doesn't go off screen
			if (paddle_x > (SCREEN_WIDTH-PADDLE_WIDTH))
				paddle_x = SCREEN_WIDTH-PADDLE_WIDTH;

		} // end if
		else if (KEY_DOWN(VK_LEFT))
		{
			// move paddle to right
			paddle_x-=8;

			// make sure paddle doesn't go off screen
			if (paddle_x < 0)
				paddle_x = 0;

		} // end if

		// draw blocks
		Draw_Blocks();

		// move the ball
		ball_x+=ball_dx;
		ball_y+=ball_dy;

		// keep ball on screen, if the ball hits the edge of 
		// screen then bounce it by reflecting its velocity
		if (ball_x > (SCREEN_WIDTH - BALL_SIZE) || ball_x < 0) 
		{
			// reflect x-axis velocity
			ball_dx=-ball_dx;

			// update position 
			ball_x+=ball_dx;
		} // end if

		// now y-axis
		if (ball_y < 0) 
		{
			// reflect y-axis velocity
			ball_dy=-ball_dy;

			// update position 
			ball_y+=ball_dy;
		} // end if
		else   if (ball_y > (SCREEN_HEIGHT - BALL_SIZE))
		{// penalize player for missing the ball
			// reflect y-axis velocity
			ball_dy=-ball_dy;

			// update position 
			ball_y+=ball_dy;

			// minus the score
			score-=100;

			// lost game
			game_state = GAME_STATE_LOSEIT;

		} // end if

		// next watch out for ball velocity getting out of hand
		if (ball_dx > 8) ball_dx = 8;
		else if (ball_dx < -8) ball_dx = -8;    

		// test if ball hit any blocks or the paddle
		Process_Ball();

		// draw the paddle and shadow
		Draw_Rectangle(paddle_x-8, paddle_y+8, 
			paddle_x+PADDLE_WIDTH-8, 
			paddle_y+PADDLE_HEIGHT+8,0);

		Draw_Rectangle(paddle_x, paddle_y, 
			paddle_x+PADDLE_WIDTH, 
			paddle_y+PADDLE_HEIGHT,PADDLE_COLOR);

		// draw the ball
		Draw_Rectangle(ball_x-4, ball_y+4, ball_x+BALL_SIZE-4, 
			ball_y+BALL_SIZE+4, 0);
		Draw_Rectangle(ball_x, ball_y, ball_x+BALL_SIZE, 
			ball_y+BALL_SIZE, 255);

		memset(buffer, 0, sizeof(buffer));

		if (level > 1) {
			game_state = GAME_STATE_WIN;
		}

		// draw the info
		if (game_state == GAME_STATE_LOSEIT) {
			sprintf(buffer,"K I C K B A L L  得分 %d  等级 %d  结果: LOST, 回车继续",
				score, level);
		} else if (game_state == GAME_STATE_WIN) {
			sprintf(buffer,"K I C K B A L L  得分 %d  等级 %d  结果: WIN,  回车继续",
				score, level);
		} else {
			sprintf(buffer,"K I C K B A L L  得分 %d             等级 %d",
				score, level);
		}

		Draw_Text_GDI(buffer, 8,SCREEN_HEIGHT-16, 127);

		// flip the surfaces
		DD_Flip();

		// sync to 33ish fps
		Wait_Clock(30);

		// check of user is trying to exit
		if (KEY_DOWN(VK_ESCAPE))
		{
			// send message to windows to exit
			PostMessage(main_window_handle, WM_DESTROY,0,0);

			// set exit state
			game_state = GAME_STATE_SHUTDOWN;

		} // end if
	} // end if
	else if (game_state == GAME_STATE_SHUTDOWN)
	{
		// in this state shut everything down and release resources
		DD_Shutdown();

		// switch to exit state
		game_state = GAME_STATE_EXIT;

	} // end if
	else if (game_state == GAME_STATE_LOSEIT)
	{
		// sync to 33ish fps
		Wait_Clock(30);

		// check of user is trying to exit
		if (KEY_DOWN(VK_ESCAPE))
		{
			// send message to windows to exit
			PostMessage(main_window_handle, WM_DESTROY,0,0);

			// set exit state
			game_state = GAME_STATE_SHUTDOWN;

		} // end if
		
		if (KEY_DOWN(VK_RETURN))
		{
			game_state = GAME_STATE_START_LEVEL;
			score = 0;
			level = 1;
		}
	}
	else if (game_state == GAME_STATE_WIN)
	{
		// sync to 33ish fps
		Wait_Clock(30);

		// check of user is trying to exit
		if (KEY_DOWN(VK_ESCAPE))
		{
			// send message to windows to exit
			PostMessage(main_window_handle, WM_DESTROY,0,0);

			// set exit state
			game_state = GAME_STATE_SHUTDOWN;

		} // end if

		if (KEY_DOWN(VK_RETURN))
		{
			game_state = GAME_STATE_START_LEVEL;
			score = 0;
			level = 1;
		}
	}

	// return success
	return(1);

} // end Game_Main

