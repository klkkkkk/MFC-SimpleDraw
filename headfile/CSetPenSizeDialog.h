#pragma once
#include "afxdialogex.h"


// CSetPenSizeDialog 对话框

class CSetPenSizeDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetPenSizeDialog)

public:
	CSetPenSizeDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetPenSizeDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit Edit;
	int Pen_Size = 1;
	afx_msg void OnEnChangeEdit1();
};
