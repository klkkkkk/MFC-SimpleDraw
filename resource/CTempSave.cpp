#include "pch.h"
#include "CTempSave.h"


int TempSave(CWnd* pWnd, list<HBITMAP>& SaveSeries, int savenum) {
	CClientDC dc(pWnd);
	CRect rect;
	GetClientRect(pWnd->GetSafeHwnd(), &rect);//��ȡ������С
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//��������λͼ
	HDC hdc = CreateCompatibleDC(dc);      //��������DC���Ա㽫ͼ�񱣴�Ϊ��ͬ�ĸ�ʽ
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);//��λͼѡ��DC�������淵��ֵ 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);//����ĻDCͼ���Ƶ��ڴ�DC
	if (savenum > MAX_SAVE) {
		SaveSeries.pop_front();
	}
	SaveSeries.push_back(hbitmap);
	SelectObject(hdc, hOldMap);
	return 1;
}
int OpenTemp(CWnd* pWnd, list<HBITMAP>& SaveSeries) {
	Redraw(pWnd, SaveSeries.back(), 0, 0);
	SaveSeries.pop_back();
	return 1;
}
int Redraw(CWnd* pWnd,HBITMAP savemap, int x, int y) {
	CBitmap loadImageBitmap;
	loadImageBitmap.Attach(savemap);
	BITMAP bitmap;
	loadImageBitmap.GetBitmap(&bitmap);

	CBrush newBrush, * oldBrush;
	CPen newPen, * oldPen;
	newBrush.CreatePatternBrush(&loadImageBitmap);
	newPen.CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	CClientDC dc(pWnd);
	oldBrush = (CBrush*)dc.SelectObject(&newBrush);
	oldPen = dc.SelectObject(&newPen);
	dc.Rectangle(x, y, x + bitmap.bmWidth, y + bitmap.bmHeight);
	dc.SelectObject(oldBrush);
	dc.SelectObject(oldPen);
	return 1;
}