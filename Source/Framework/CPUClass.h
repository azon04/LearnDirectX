#ifndef _CPUCLASS_H_
#define _CPUCLASS_H_

// Linking
#pragma comment(lib, "pdh.lib")

// Includes
#include <pdh.h>

class CPUClass
{
public:
	CPUClass();
	~CPUClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCPUPercentage();

private:
	bool m_canReadCPU;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif
