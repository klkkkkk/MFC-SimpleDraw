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
	GetClientRect(pWnd->GetSafeHwnd(), &rect);//��ȡ������С
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//��������λͼ
	HDC hdc = CreateCompatibleDC(dc);      //��������DC���Ա㽫ͼ�񱣴�Ϊ��ͬ�ĸ�ʽ
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);//��λͼѡ��DC�������淵��ֵ 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);//����ĻDCͼ���Ƶ��ڴ�DC
	CImage image;
	image.Attach(hbitmap);                //��λͼת��Ϊһ��ͼ��

	HRESULT hResult = image.Save(filePath);     //����ͼ��
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
