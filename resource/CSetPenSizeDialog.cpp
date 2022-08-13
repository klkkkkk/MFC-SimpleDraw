// CSetPenSizeDialog.cpp: 实现文件
//

#include "pch.h"
#include "SimpleDraw.h"
#include "afxdialogex.h"
#include "CSetPenSizeDialog.h"


// CSetPenSizeDialog 对话框

IMPLEMENT_DYNAMIC(CSetPenSizeDialog, CDialogEx)

CSetPenSizeDialog::CSetPenSizeDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CSetPenSizeDialog::~CSetPenSizeDialog()
{
}

void CSetPenSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit);

	CString str;
	Edit.GetWindowTextW(str);
	Pen_Size = _ttoi(str);
}


BEGIN_MESSAGE_MAP(CSetPenSizeDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CSetPenSizeDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CSetPenSizeDialog 消息处理程序


void CSetPenSizeDialog::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
