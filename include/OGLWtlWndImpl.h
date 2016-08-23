#pragma once

template<int t_FOVY=75, int t_ZNEAR=1, int t_ZFAR=1000, int t_COLOR=24, int t_DEPTH=8, int t_STENCIL=0>
class COpenGLWndTrait
{
public:
	static int FOVY() { return t_FOVY; }
	static int ZNEAR() { return t_ZNEAR; }
	static int ZFAR() { return t_ZFAR; }
	static int Color() { return t_COLOR; }
	static int Depth() { return t_DEPTH; }
	static int Stencil() { return t_STENCIL; }
};


typedef COpenGLWndTrait<> STANDARD_OPENGL_TRAITS;

template<class T, class TRAITS=STANDARD_OPENGL_TRAITS>
class ATL_NO_VTABLE COpenGLWndImpl : public CMessageMap
{
public:
	COpenGLWndImpl() : _hglrc( NULL ) {}
	virtual ~COpenGLWndImpl() { }

	virtual void OnInitialisationOpenGL() = 0;
	virtual void OnRender() = 0;
	
	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkGnd)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CClientDC dc( static_cast<T*>(this)->m_hWnd );
		SetupPixelFormat( dc );
		if( _hglrc = wglCreateContext( dc ) )
		{
			wglMakeCurrent( dc, _hglrc );
			OnInitialisationOpenGL();
			wglMakeCurrent( dc, NULL );
		}
		bHandled = FALSE;
		return 0L;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CClientDC dc( static_cast<T*>(this)->m_hWnd );
		wglMakeCurrent( dc, NULL );
		wglDeleteContext( _hglrc );
		_hglrc = NULL;
		bHandled = FALSE;
		return 0L;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		int dx = LOWORD(lParam), dy = HIWORD(lParam);
		CClientDC dc( static_cast<T*>(this)->m_hWnd );
		wglMakeCurrent( dc, _hglrc );
		glViewport( 0, 0, dx, dy );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective(TRAITS::FOVY(), float(dx) / dy, TRAITS::ZNEAR(), TRAITS::ZFAR() );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		wglMakeCurrent( dc, NULL );
		bHandled = FALSE;
		return 0L;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(static_cast<T*>(this)->m_hWnd);
		wglMakeCurrent( dc, _hglrc );
		OnRender();
		glFlush();
		SwapBuffers(dc);
		wglMakeCurrent( dc, NULL );
		return 0L;
	}

	LRESULT OnEraseBkGnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1L;
	}

private:

	void SetupPixelFormat(HDC hDC)
	{
		PIXELFORMATDESCRIPTOR pfd = {0};

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
		pfd.iPixelType= PFD_TYPE_RGBA;
		pfd.cColorBits = TRAITS::Color();
		pfd.cDepthBits = TRAITS::Depth();
		pfd.cStencilBits = TRAITS::Stencil();
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat( hDC, &pfd );
		SetPixelFormat( hDC, iPixelFormat, &pfd );
	}

	HGLRC _hglrc;
};

//////////////////////////////////////////////////////////////////////

class CFullScreenImpl
{
public:
	static void SwitchToFullScreen( int* dx=0, int *dy=0, int color=0, BOOL bInherit=TRUE )
	{
		if( bInherit )
		{
			*dx = GetSystemMetrics( SM_CXSCREEN );
			*dy = GetSystemMetrics( SM_CYSCREEN );

			CClientDC dc( NULL );
			color = dc.GetDeviceCaps(BITSPIXEL);
		}

		DEVMODE dev = { 0 };

		dev.dmSize = sizeof(dev);
		dev.dmBitsPerPel = color;
		dev.dmPelsWidth = *dx;
		dev.dmPelsHeight = *dy;
		dev.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if( ChangeDisplaySettings( &dev, CDS_FULLSCREEN )!=DISP_CHANGE_SUCCESSFUL )
		{
			MessageBox( NULL, L"Failed", L"Full Screen", MB_OK|MB_ICONERROR );
		}
	}

	static void Restore()
	{
		ChangeDisplaySettings(NULL,0);
	}
};