
// SimpleDrawView.h: CSimpleDrawView 类的接口
//

#pragma once
#include <list>
#define MAX_SAVE 10
using namespace std;

class CSimpleDrawView : public CView
{
protected: // 仅从序列化创建
	CSimpleDrawView() noexcept;
	DECLARE_DYNCREATE(CSimpleDrawView)

// 特性
public:
	CSimpleDrawDoc* GetDocument() const;

// 操作
public:
	int Pen_Size;//画笔粗细
	COLORREF Pen_Color, Brush_Color;//画笔颜色，填充颜色
	CPoint BeginPoint, EndPoint, //图形起始点，图形终止点
		Chooselt, Choosebr, Tempclt, Tempcbr, Startlt, Startbr;//选框左上角，选框右下角

	enum class Shape {//形状
		Line,Rectangle,Square,Circle,Ellipse,//直线，矩形，正方形，圆，椭圆，
		Triangle,Pentagon,Hexagon,//三角形，五边形，六边形
		Pencil,Fill,Text,Eraser,Choose,LImage//画笔，填充, 文本，橡皮，选择，加载图片
	}m_Shape;

	list<HBITMAP> SaveSeries;//暂存图片列表

	int Textid = 100;
	int savenum = 0;//撤回计数
	bool Chosen = false;//是否已经选择了一个区域
	HDC bc_hdc,ac_hdc;//选中前画面，选中后画面
	CEdit* m_Edit=nullptr;
	CPoint Text_Pos=CPoint(0,0);//文本框位置
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void CSimpleDrawView::ClearRect(CRect rect);
	void CSimpleDrawView::FastRect(CRect rect,bool notxor = true);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSimpleDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDrawLine();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawEllipse();
	afx_msg void OnSetPencil();
	afx_msg void OnSetColor();
	afx_msg void OnSetPenSize();
	afx_msg void OnText();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetEraser();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnDrawSquare();
	afx_msg void OnFillColor();
	afx_msg void OnUndo();
	afx_msg void OnDrawTriangle();
	afx_msg void OnDrawPentagon();
	afx_msg void OnDrawHexagon();
	afx_msg void OnChoose();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // SimpleDrawView.cpp 中的调试版本
inline CSimpleDrawDoc* CSimpleDrawView::GetDocument() const
   { return reinterpret_cast<CSimpleDrawDoc*>(m_pDocument); }
#endif

