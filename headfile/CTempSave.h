#pragma once
#include <list>
#define MAX_SAVE 10
using namespace std;
int TempSave(CWnd* pWnd, list<HBITMAP>& SaveSeries, int savenum);
int OpenTemp(CWnd* pWnd, list<HBITMAP>& SaveSeries);
int Redraw(CWnd* pWnd, HBITMAP savemap, int x, int y);
//class CTempSave
//{
//public:
	//CTempSave();
	//~CTempSave();

//public:
	

//};

