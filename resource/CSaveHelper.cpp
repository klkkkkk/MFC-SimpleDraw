#include "pch.h"
#include "CSaveHelper.h"
CSaveHelper::CSaveHelper()
{
}
CSaveHelper::~CSaveHelper()
{
}
int CSaveHelper::Save(CWnd* pWnd, CString filePath)
{
	CClientDC dc(pWnd);
	CRect rect;
	GetClientRect(pWnd->GetSafeHwnd(), &rect);//获取画布大小
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//创建兼容位图
	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);//将屏幕DC图像复制到内存DC
	CImage image;
	image.Attach(hbitmap);                //将位图转化为一般图像

	HRESULT hResult = image.Save(filePath);     //保存图像
	if (FAILED(hResult))
	{
		return -1;
	}
	else
	{
		return 1;
	}
	image.Detach();
	SelectObject(hdc, hOldMap);
	return 0;
}
