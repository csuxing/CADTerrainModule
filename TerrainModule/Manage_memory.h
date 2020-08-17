#pragma once
#include <fstream>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
//#include"windows.h"
//#include "afx.h"
//#include"afxwin.h"
//#define _CRTDBG_MAP_ALLOC
//#include"stdlib.h"
//#include<crtdbg.h>
//#ifdef _DEBUG
//#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif
using namespace std;
void static showMemoryInfo()
{
	static int count=0;
	count++;
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	//cout << "内存使用:" << pmc.WorkingSetSize / 1000 << "K/" << pmc.PeakWorkingSetSize / 1000 << "K + " << pmc.PagefileUsage / 1000 << "K/" << pmc.PeakPagefileUsage / 1000 << "K" << endl;
	//CString show;
	//show.Format(_T("%.2f,%.2f"),pmc.WorkingSetSize/(1024.0*1024.0),pmc.PeakWorkingSetSize/(1024.0*1024.0));
	//AfxMessageBox(show);
	ofstream out;
	out.open("D:\\内存占用.txt", ios::app);
	out <<count<<","<< pmc.WorkingSetSize / (1024.0*1024.0) << endl;
	out.close();
}

