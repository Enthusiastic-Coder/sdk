#pragma once

#include <windows.h>
#include <Windowsx.h>
#include <include_gl.h>
#include <HRTimer.h>

#define TEMPLATE_DECL \
template<class T,int t_X,int t_Y,int t_DX,int t_DY,int t_fov, int t_nearZ, int t_farZ,int t_bitPerPixel,int t_depthBuffer,int t_stencilBuffer>

#define TEMPLATE_IMPL \
T, t_X,t_Y,t_DX,t_DY,t_fov, t_nearZ, t_farZ, t_bitPerPixel,t_depthBuffer, t_stencilBuffer

//template<typename bool t_dWM=0, int t_X=0, int t_Y=0, int t_DX=0, int t_DY=0,
//int t_fov = 65, int t_farZ=100000, int t_bitPerPixel=32, int t_depthBuffer=24, int t_stencilBuffer=8>
//struct OpenGLWndTraits
//{
//	static const bool _t_dWM=t_dWM;
//	static const int _t_X = t_X;
//	static const int _t_Y = t_Y;
//	static const int _t_DX = t_DX;
//	static const int _t_DY = t_DY;
//	static const int _t_fov = t_fov;
//	static const int _t_farZ = t_farZ;
//	static const int _t_bitPerPixel = t_bitPerPixel; 
//	static const int _t_depthBuffer = t_depthBuffer;
//	static const int _t_stencilBuffer= t_stencilBuffer;
//};
//
//typedef OpenGLWndTraits<> STANDARD_COMPATIABLE_TRAITS;
//
//template<typename T, STANDARD_COMPATIABLE_TRAITS>
//class OpenGLWnd2
//{
//};


template<class T, int t_X=0, int t_Y=0, int t_DX=0, int t_DY=0,
int t_fov = 65, int t_nearZ = 1,int t_farZ=100000, int t_bitPerPixel=32, int t_depthBuffer=24, int t_stencilBuffer=8>
class OpenGLHWND
{
public:

	OpenGLHWND(bool bCoreProfile=false, int iMajor=2, int iMinor=1)	: 
		_dt(0.0),
		_hWnd(nullptr),
		_fInverseFPS(1/30.0),
		_hglrc(nullptr),
		_bCoreProfile(bCoreProfile),
		_iMajor(iMajor),
		_iMinor(iMinor)
	{
	}

	HWND _hWnd;
	HGLRC _hglrc;
	double _fInverseFPS;
	HRTimer _timer;
	
	void setFramePerSecond( double fps)
	{
		_fInverseFPS = 1.0 / std::max(1.0,fps);
	}

	void setRunning( bool bRunning )
	{
		_running = bRunning;
	}

	bool isRunning()
	{
		return _running;
	}

	double frameTime()
	{
		return _dt;
	}

	bool isCoreProfile()
	{
		return _bCoreProfile;
	}

	int Run(HINSTANCE hInstance, int nCmdShow)
	{
		_this = (T*)this;
		_this->setFramePerSecond(30.0);
		_running = true;
		
		char szWindowClass[256]="";
		sprintf_s( szWindowClass,"%d%d%d%d%d%d%dFullScreenOpenGLWnd", t_X, t_Y, t_DX, t_DY, t_bitPerPixel, t_depthBuffer, t_stencilBuffer );

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hInstance		= hInstance;
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= OpenGLHWND<TEMPLATE_IMPL>::WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hIcon			= NULL;
		wcex.hbrBackground	= NULL;
		wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_AIRBUSGROUNDPHYSICS );
		wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName	= szWindowClass;

		if( RegisterClassEx(&wcex) )
		{
			int cx = GetSystemMetrics( SM_CXSCREEN );
			int cy = GetSystemMetrics( SM_CYSCREEN );

			RECT rc = {t_X, t_Y, t_X+t_DX, t_Y+t_DY};

			if( t_DX == 0) rc.right = cx + rc.left;
			if( t_DY == 0) rc.bottom = cy + rc.top;

			DWORD styles = 0;

			if( t_DX && t_DY )
			{
				styles = WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME|WS_CAPTION|WS_SYSMENU;
			}
			
			//AdjustWindowRect( &rc, styles | WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, true );
			
			if( _hWnd ) DestroyWindow(_hWnd);

			CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, NULL, styles | WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
			   rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, NULL, NULL, hInstance, LPVOID(this));

			if( _hWnd )
			{
				ShowWindow(_hWnd, nCmdShow);
				UpdateWindow(_hWnd);

				HWND foreGroundHWND = GetForegroundWindow();

				AttachThreadInput( GetCurrentThreadId(), GetWindowThreadProcessId(foreGroundHWND, NULL ), true );
				SetFocus(_hWnd );
				ShowWindow(_hWnd,nCmdShow);
				AttachThreadInput( GetCurrentThreadId(), GetWindowThreadProcessId(foreGroundHWND, NULL ), false );

				MSG msg;

				_timer.Start();

				bool bQuit = false;

				while( !bQuit ) 
				{
					while (!_running && GetMessage(&msg, NULL, 0, 0))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					if (!_running)
						break;

					while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					{
						if (msg.message == WM_QUIT)
							bQuit = true;

						if (msg.message == WM_TIMER)
						{
							WndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
						}
						else
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}

					__if_exists(T::OnTimeOut)
					{
						_dt = _timer.Elapsed();

						if (_dt >= _fInverseFPS)
						{
							_timer.Restart();
							_this->OnTimeOut(_dt);
						}
					}

				} 
			}
		}

		return 0;
	}

	operator HWND()
	{
		return _hWnd;
	}

protected:
					
	virtual bool OnSize(int width, int height, int showstate)
	{
		glViewport( 0, 0, width, height );
		return true;
	}

	int getFOV()
	{
		return t_fov;
	}

	float getNEAR()
	{
		return t_nearZ;
	}

	int getFAR()
	{
		return t_farZ;
	}

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		T *pT = (T *)GetProp(hWnd, _static_prop_name );
		static int wmId, wmEvent;
		static PAINTSTRUCT ps;
		static HDC hdc;
		
		switch (message)
		{
			case WM_TIMER:
				__if_exists(T::OnTimer)
				{
					if (pT && pT->OnTimer((UINT)wParam, (TIMERPROC)lParam))
						return false;
				}
				break;

			case WM_CREATE:	
				{
					RECT rc;
					PIXELFORMATDESCRIPTOR pfd = {0};
					pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
					pfd.nVersion = 1;
					pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI;
					pfd.iPixelType = PFD_TYPE_RGBA;
					pfd.cColorBits = t_bitPerPixel;
					pfd.cDepthBits = t_depthBuffer;
					pfd.cStencilBits = t_stencilBuffer;
					pfd.iLayerType = PFD_MAIN_PLANE;

					pT = (T*) (((LPCREATESTRUCT) lParam)->lpCreateParams);
					SetProp(hWnd, _static_prop_name, HANDLE(pT));
					pT->_hWnd = hWnd;

					__if_exists( T::OnPixelFormatPrepare )
					{
						pT->OnPixelFormatPrepare( &pfd );
					}

					hdc = GetDC(hWnd);

					int iPixelFormat = ChoosePixelFormat( hdc, &pfd );

					SetPixelFormat( hdc, iPixelFormat, &pfd );

					pT->_hglrc = wglCreateContext(hdc);

					wglMakeCurrent( hdc, pT->_hglrc );

					if (wglCreateContextAttribsARB)
					{
						int attribs[] = {
							WGL_CONTEXT_MAJOR_VERSION_ARB, pT->_iMajor,
							WGL_CONTEXT_MINOR_VERSION_ARB, pT->_iMinor,
							WGL_CONTEXT_PROFILE_MASK_ARB, 
							pT->_bCoreProfile ? 
					WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB ,
							0 };

						HGLRC hglrc = wglCreateContextAttribsARB(hdc, NULL, attribs);

						if (hglrc)
						{
							wglMakeCurrent(hdc, NULL);
							wglDeleteContext(pT->_hglrc);
							pT->_hglrc = hglrc;
							wglMakeCurrent(hdc, pT->_hglrc);
						}
					}

					GetClientRect( hWnd, &rc );

					pT->OnSize( rc.right - rc.left, rc.bottom - rc.top, 0 );

					glClearColor( 1.0f, 1.0f,1.0f, 1.0f );
					
					__if_exists(T::OnInitialise)
					{
						if (!pT->OnInitialise(hdc))
						{
							DestroyWindow(hWnd);
							break;
						}
					}

					wglMakeCurrent( hdc, NULL );
					ReleaseDC( hWnd, hdc );
				}
				break;

			case WM_DESTROY:

				__if_exists(T::OnUnInitialise)
				{
					hdc = GetDC(hWnd);
					wglMakeCurrent( hdc, pT->_hglrc );
					pT->OnUnInitialise();
					wglMakeCurrent( hdc, NULL );
					ReleaseDC( hWnd, hdc );
				}

				wglDeleteContext( pT->_hglrc );
				RemoveProp(hWnd, _static_prop_name );
				PostQuitMessage(0);
				return 0;

			case WM_SYSCOMMAND:
				if( wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER )
					return 0;
				break;

			case WM_COMMAND:
				__if_exists(T::OnCommand)
				{
					wmId    = LOWORD(wParam);
					wmEvent = HIWORD(wParam);

					if( pT->OnCommand( wmId, wmEvent ) )
						return 0;
				}
				break;

			case WM_SIZE:
				{
					bool bRet(false);

					hdc = GetDC(hWnd); 
					wglMakeCurrent( hdc, pT->_hglrc );
					bRet = pT->OnSize(LOWORD(lParam),HIWORD(lParam), wParam);
					wglMakeCurrent( hdc, NULL );
					ReleaseDC( hWnd, hdc );

					if( bRet == true)
						return 0;
				}
				break;

			case WM_PAINT:
				hdc = BeginPaint(hWnd, &ps);

				wglMakeCurrent( hdc, pT->_hglrc );
				glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);

				__if_exists(T::OnRender)
				{
					pT->OnRender();
				}

				SwapBuffers(hdc);
				wglMakeCurrent( hdc, NULL );
				
				EndPaint(hWnd, &ps);
				return 0;

			case WM_CHAR:
				
				__if_exists(T::OnChar)
				{
					if( pT->OnChar(wParam) )
						return false;
				}
				break;

			case WM_SYSCHAR:
				
				__if_exists(T::OnSysChar)
				{
					if( pT->OnSysChar(wParam,lParam) )
						return false;
				}
				break;

			case WM_KEYDOWN:
				
				__if_exists(T::OnKeyDown)
				{
					if( pT->OnKeyDown(wParam) )
						return false;
				}
				if( wParam == VK_ESCAPE )
				{
					DestroyWindow( pT->_hWnd );
				}
				break;

			case WM_MOUSEMOVE:
				__if_exists(T::OnMouseMove)
				{
					if( pT->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam) )
						return false;
				}

				break;

			case WM_LBUTTONDOWN:
				__if_exists(T::OnLButtonDown)
				{
					if( pT->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam) )
						return false;
				}
				break;

			case WM_LBUTTONUP:
				__if_exists(T::OnLButtonUp)
				{
					if( pT->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam) )
						return false;
				}
				break;

			case WM_RBUTTONDOWN:
				__if_exists(T::OnRButtonDown)
				{
					if( pT->OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam) )
						return false;
				}
				break;

			case WM_RBUTTONUP:
				__if_exists(T::OnRButtonUp)
				{
					if( pT->OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam) )
						return false;
				}
				break;

			case WM_MOUSEWHEEL:
				__if_exists(T::OnMouseWheel)
				{
					if( pT->OnMouseWheel(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam)) )
						return false;
				}
				break;

			case WM_SETCURSOR:
				__if_exists(T::OnSetCursor)
				{
					if(pT->OnSetCursor( (HWND)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) )
						return true;
				}
				break;

			case WM_ACTIVATE:
				{
					::InvalidateRect( pT->_hWnd, NULL, true );
					break;
				}

			case WM_ACTIVATEAPP:
				{
					::InvalidateRect( pT->_hWnd, NULL, true );
					break;
				}
		}
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	T* _this;
	//LARGE_INTEGER _tickLastTickCount;
	static const char * _static_prop_name;
private:
	double _dt;
	bool _running;
	bool _bCoreProfile;
	int _iMajor;
	int _iMinor;
};

TEMPLATE_DECL const char * OpenGLHWND<TEMPLATE_IMPL>::_static_prop_name = "OBJECT";

