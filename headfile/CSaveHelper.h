#pragma once
class CSaveHelper
{
public:
	CSaveHelper();
	~CSaveHelper();

public:
	int Save(CWnd* pWnd, CString filePath);
};

