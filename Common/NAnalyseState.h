#pragma once

#ifdef UNITS_EXPORTS
#define UNITS_API __declspec(dllexport)
#else
#define UNITS_API __declspec(dllimport)
#endif

class UNITS_API NAnalyseState
{
public:
	NAnalyseState(void);
	~NAnalyseState(void);
	void SetCurVolume(double Vol);
public:
	double InitVolume;
	double CurVolume;
	double CutVolume;
};
