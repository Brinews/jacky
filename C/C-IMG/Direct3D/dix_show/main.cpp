
//-----------------------------------������˵����----------------------------------------------
//  ��������:��MoodWords
//	 2013��4�� Create by ����˼
//  �����������ʼ�󣬶�ȡtxt�еĴ��ﲢ����500msһ����Ƶ����ʾ����Ļ��
//------------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //����PlaySound����������ļ�
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"��MoodWords��"	        //Ϊ���ڱ��ⶨ��ĺ�
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }   //����һ����ȫ�ͷź꣬���ں���COM�ӿ�ָ����ͷ�

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; //Direct3D�豸����
ID3DXFont*					 g_pFont=NULL;    //����COM�ӿ�
float								 g_FPS = 0.0f;       //һ�������͵ı���������֡����
wchar_t							 g_strFPS[50];    //����֡���ʵ��ַ�����

vector<string> words;
long long startTime;
int wordIndex = 0;

wchar_t dispChars[64];


//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );//���ڹ��̺���
HRESULT					Direct3D_Init(HWND hwnd);		 //����������н���Direct3D�ĳ�ʼ��
HRESULT					Objects_Init(HWND hwnd); 		//����������н���Ҫ���Ƶ��������Դ��ʼ��
VOID							Direct3D_Render(HWND hwnd); 	//����������н���Direct3D��Ⱦ�������д
VOID							Direct3D_CleanUp( );				//���������������COM��Դ�Լ�������Դ
float							Get_FPS();

//---------------------��ȡ�ļ�--------------------------------------
//-----------------------------------------------------------------
void readWords(char* filename)
{
	ifstream fin(filename);
	string word;

	while (getline(fin, word, '@')) {
		words.push_back(word);
	}

	wordIndex = 0;

	fin.close();
}

//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon=(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;		

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow( L"ForTheDreamOfGameDevelop",WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );

	//Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK==Direct3D_Init (hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("��Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd,250,80,WINDOW_WIDTH,WINDOW_HEIGHT,true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow( hwnd, nShowCmd );    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	PlaySound(L"Final Fantasy XIII.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP); //ѭ�����ű������� 

	readWords("test.txt");
	startTime = timeGetTime();

	//��5����Ϣѭ������
	MSG msg = { 0 };  //��ʼ��msg
	while( msg.message != WM_QUIT )			//ʹ��whileѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			Direct3D_Render(hwnd);   //������Ⱦ
		}
	}
	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;  
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )      
{
	switch( message )						//switch��俪ʼ
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		Direct3D_Render(hwnd);                 //����Direct3D��Ⱦ����
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Direct3D_CleanUp();			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage( 0 );			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc( hwnd, message, wParam, lParam );		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��Direct3D_Init( )������--------------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

		if(!(S_OK==Objects_Init(hwnd))) return E_FAIL;     //����һ��Objects_Init��������Ⱦ��Դ�ĳ�ʼ��
	return S_OK;
}



//-----------------------------------��Object_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init(HWND hwnd)
{
	//��������
	if(FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("΢���ź�"), &g_pFont)))
		return E_FAIL;
	srand(timeGetTime());      //��ϵͳʱ���ʼ��������� 
	return S_OK;
}


//-----------------------------------��Direct3D_Render( )������-------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮һ������������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʼ����
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // ��ʼ����
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮��������ʽ���ƣ�����������д���Ķ�����
	//--------------------------------------------------------------------------------------
	//�ڴ������ϽǴ�����ʾÿ��֡��
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pFont->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_XRGB(255,39,136));

	long long int nowTime = timeGetTime();
	
	if (startTime != 0) {
		if ((nowTime - startTime)%500 == 0) {
			wordIndex = (wordIndex+1) % words.size();
		}
		else if ((nowTime - startTime)%500 < 200) {
			//��������250����д�ڶ�������
			formatRect.top = 250;
			//swprintf_s(dispChars, 64, _T("%s"), words[wordIndex].c_str());
			WCHAR wsz[256];
			memset(wsz, 0, sizeof(wsz));
			const char *w = words[wordIndex].c_str();
			MultiByteToWideChar(CP_ACP, 0, w, strlen(w)+1, wsz, sizeof(wsz)/sizeof(wsz[0]));

			g_pFont->DrawText(0, wsz, -1, &formatRect, 
				DT_CENTER, D3DCOLOR_XRGB(255,255,255));
		} else if ((nowTime - startTime)%500 < 500) {
			//��������250����д�ڶ�������
			formatRect.top = 250;
			g_pFont->DrawText(0, _T(""), -1, &formatRect, 
				DT_CENTER, D3DCOLOR_XRGB(255,255,255));
		}
	}

	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�ġ�����������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // ��������
	//--------------------------------------------------------------------------------------
	// ��Direct3D��Ⱦ�岽��֮�塿����ʾ��ת
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ
}

//-----------------------------------��Get_FPS( )������------------------------------------------
//	���������ڼ���ÿ��֡���ʵ�һ������
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{
	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int     frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

	//�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount    = 0;//������֡��frameCountֵ����
	}

	return fps;
}

//-----------------------------------��Direct3D_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)
}
