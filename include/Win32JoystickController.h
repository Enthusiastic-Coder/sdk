#pragma once

#include <windows.h>
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#define POSNORM(t,T) \
    DWORD t##Pos()	{ return _joyInfoEx.dw##T##pos; } \
    float t##PosNorm() \
{ \
    DWORD delta = (_joyCaps.w##T##max - _joyCaps.w##T##min);\
    if(!delta ) return 0.0f;\
    return ( _joyInfoEx.dw##T##pos / float(delta) - 0.5f )*2;\
    }

class Win32JoystickController
{
public:
    Win32JoystickController()
    {
        _bAvailable = FALSE;
        ZeroMemory( &_joyCaps, sizeof(_joyCaps ) );
        ZeroMemory( &_joyInfoEx, sizeof(_joyInfoEx ) );
    }

    bool init()
    {
        if( !joyGetNumDevs() ) return FALSE;
        _bAvailable = (JOYERR_NOERROR == joyGetDevCaps( JOYSTICKID1, &_joyCaps, sizeof(_joyCaps) ));
        return _bAvailable;
    }

    bool update()
    {
        _joyInfoEx.dwFlags = JOY_RETURNALL;
        _joyInfoEx.dwSize = sizeof(_joyInfoEx);
        _bAvailable = (joyGetPosEx(JOYSTICKID1, &_joyInfoEx ) == JOYERR_NOERROR);
        return _bAvailable;
    }

    bool lastAvailable() { return _bAvailable; }

    POSNORM(x,X);
    POSNORM(y,Y);
    POSNORM(z,Z);
    POSNORM(r,R);
    POSNORM(u,U);
    POSNORM(v,V);

    DWORD getPOV()					{ return _joyInfoEx.dwPOV; }
    DWORD numberofButtonsPressed()	{ return _joyInfoEx.dwButtonNumber; }
    DWORD buttonFlagPressed()		{ return _joyInfoEx.dwButtons; }
    UINT numberOfAxisFound()		{ return _joyCaps.wNumAxes; }
    JOYCAPS& caps()					{ return _joyCaps; }
    JOYINFOEX& info()				{ return _joyInfoEx; }

private:
    JOYCAPS _joyCaps;
    JOYINFOEX _joyInfoEx;
    bool _bAvailable;
};
