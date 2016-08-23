#pragma once

#include <windows.h>
#include <mmsystem.h>

class HRTimer 
{
public:
	HRTimer();

	double GetFrequency(void);
	void Restart(void);
	void Start(void) ;
    float Elapsed(void);
    float ElapsedMilliSeconds(void);

private:
	LARGE_INTEGER _start;
	LARGE_INTEGER _stop;
	double _frequency = 0.0;
};

inline HRTimer::HRTimer(void) : _start(), _stop(), _frequency()
{
	_frequency = GetFrequency();
	_start.QuadPart = 0;
	_stop.QuadPart = 0;
}

inline double HRTimer::GetFrequency(void)
{
	LARGE_INTEGER proc_freq;

	if (!::QueryPerformanceFrequency(&proc_freq)) 
		return 0.0;

	return (double)proc_freq.QuadPart;
}

inline void HRTimer::Restart(void)
{
	_start = _stop;
}

inline void HRTimer::Start(void)
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&_start);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
}

inline float HRTimer::Elapsed(void)
{
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	::QueryPerformanceCounter(&_stop);
	::SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	return ((_stop.QuadPart - _start.QuadPart) / _frequency );
}

inline float HRTimer::ElapsedMilliSeconds(void)
{
    return Elapsed()*1000;
}
