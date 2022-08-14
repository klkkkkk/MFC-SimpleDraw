# MFC-SimpleDraw
基于MFC的简易绘图程序，支持绘制各种图形及文本、修改已有图形、撤回、保存等。附有详细的功能实现过程。

## 基本功能
* 拥有含标题条、菜单、工具条、和绘图工作区的窗口界面。
* 支持修改画笔的粗细和颜色。
* 支持绘制各种基本图形，包含直线、矩形、正方形、圆、椭圆、三角形、五边形、六边形。
* 支持多种画笔模式，包含铅笔、橡皮、填充、文本框。
* 支持框选部分区域并可对选中区域进行拖动、放大缩小、删除操作。
* 支持撤回操作。
* 支持保存当前图像文件和打开已有图像文件（bmp格式）。

## 创建项目
该绘图程序开发及运行环境为 __Microsoft Visual Studio 2022__。打开Visual Studio，选择创建新项目，选择创建 __MFC应用__。将项目命名为SimpleDraw，选择MFC的应用程序类型为 __单文档__，项目样式选择为 __MFC standard__，点击完成，即可等待系统自动建立新项目。

## 基本框架
项目创建完成后，其中已自动包含了一些文件。其类结构大致如下：
![](/picture/1.png)
这里只需关心其中两个类。第一， ``CSettingDlg``用来编辑类的消息响应函数，例如当我们创建一个新的对话框时，就需要用到该类的相关函数。第二， ``CSimpleDrawView``代表了程序运行的可视化窗口，它包含工具栏和菜单栏的命令响应函数以及鼠标的消息响应函数。实现绘图过程中各个功能的代码都将在这个类中进行编写。<br />
根据上面列出的基本功能，应事先在 ``CSimpleDrawView``类中添加如下成员：
* 该程序允许用户调整画笔的粗细，因此增加一个代表画笔粗细的变量 ``int Pen_Size``。
* 用户可以调整画笔的颜色以及使用填充功能，因此增加画笔颜色和填充颜色 ``COLORREF Pen_Color, Brush_Color``。
* 图像需要随着用户的鼠标移动实时展现在工作区，因此需要有记录用户鼠标位置的变量 ``CPoint BeginPoint, EndPoint``。它们分别代表用户按下鼠标的点和抬起鼠标的点。
* 用户需要用特定的模式完成对特定图形的绘制，而且这些模式都互不兼容（即不能同时在两种模式下进行绘画），因此创建了一个枚举类型，其中包含了14种不同的模式如下：
```
enum class Shape {//形状
		Line,Rectangle,Square,Circle,Ellipse,//直线，矩形，正方形，圆，椭圆，
		Triangle,Pentagon,Hexagon,//三角形，五边形，六边形
		Pencil,Fill,Text,Eraser,Choose,LImage//画笔，填充, 文本，橡皮，选择，加载图片
	}m_Shape;
```
* 程序允许撤回操作，用一个列表 ``list<HBITMAP> SaveSeries``暂存用户工作区先前的图像。与此同时设置 ``int savenum``代表用户可撤回的最大步数。
* 为实现插入文本框功能，为文本框指定一个编号 ``int Textid``、文本框指针 ``CEdit* m_Edit``以及文本框位置 ``CPoint Text_Pos``。<br />

它们的作用将在后面的分析中详细介绍。


## 具体功能分析和实现
### 添加菜单选项
在该工程的资源视图下找到 __Menu__ 文件夹，就可以打开菜单界面，并添加所需的选项。<br />
![](/picture/2.png)<br />
根据上述提到的基本功能，在 __文件__ 一栏保留新建、打开、保存三个选项， __编辑__ 一栏仅保留撤销选项。画图所用的工具和选项均在 __图形/工具__ 一栏，图形包含直线、矩形、多边形（其中含三角形、五边形、六边形）、圆、椭圆，工具包含铅笔、填充、文本框、橡皮、选择区域。关于画笔属性的设置单独新建一栏，其中包含粗细和颜色。菜单栏设置完毕，接下来只需要再为每一个选项编写事件处理程序，就能在点击这些选项时实现对应的功能。

### 画笔粗细和颜色设置
#### 画笔粗细
实现调整画笔粗细功能的思路如下：点击菜单中的画笔设置-粗细，弹出一个对话框，用户输入粗细的数值，点击确认即可更改画笔粗细。因此我们需要一个能够处理用户输入的对话框程序。在资源视图中的 __Dialog__ 文件夹添加一个类型为 ``Dialog``（对话框）的资源，如下所示：
![](/picture/3.png)<br />
添加资源后，在左侧工具箱中，将一个 ``Edit Control``（输入文本）和 ``Static Text``（文字说明）控件添加进对话框，更改 ``Static Text``的内容，设置粗细的对话框便设置完毕。<br />
![](/picture/4.png)<br />
为了调用这个对话框，需要为该对话框添加一个类：<br />
![](/picture/5.png)<br />
为输入文本框添加以下变量：<br />
![](/picture/6.png)<br />
在 ``CSetPenSizeDialog``类中添加成员 ``int Pen_Size = 1``,然后在 ``CSetPenSizeDialog::DoDataExchange``函数中添加如下代码，即可将输入的数值转变为画笔粗细：<br />
```
CString str;
Edit.GetWindowTextW(str);
Pen_Size = _ttoi(str);
```
接下来为菜单栏的 __粗细__ 添加事件处理程序，即可在点击该选项后调用上面的对话框：
```
void CSimpleDrawView::OnSetPenSize()
{
	// TODO: 在此添加命令处理程序代码
	CSetPenSizeDialog dlg;
	if (IDOK == dlg.DoModal()) {
		this->Pen_Size = dlg.Pen_Size;
	}
}
```
上面的语句将 ``CSimpleDrawView``类中的 ``Pen_Size``设置为用户在对话框中输入的 ``Pen_Size``，这样设置画笔粗细功能就实现了。
#### 颜色设置
更改画笔颜色，只需调用项目自带的 ``CColorDialog``类的对话框。为 __颜色__ 选项添加事件处理程序如下，即可调用该对话框：
```
void CSimpleDrawView::OnSetColor()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog dlg;
	if (IDOK == dlg.DoModal()) {
		if (m_Shape != Shape::Fill) {//未选择填充时默认选择的是画笔颜色
			Pen_Color = dlg.GetColor();
		}
		else {
			Brush_Color = dlg.GetColor();
		}
	}
}
```
注意，当使用的工具为 __填充__ ``Shape::Fill``时，此处设置的颜色为填充所用的颜色，而在其他情况下设置的才是画笔的颜色。
### 各种图形
#### 直线
首先为 __直线__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Line``，这样当我们在工作区绘画时，只有判断 ``m_Shape``为 ``Shape::Line``时才会执行画直线的操作。
为实现在工作区绘画，需要添加鼠标消息函数。在类向导中添加相应的鼠标消息处理程序如下：<br />
![](/picture/7.png)<br />
``OnMouseMove``为鼠标移动时会执行的操作， ``OnLButtonDown``为鼠标左键按下时会执行的操作， ``OnLButtonUp``为鼠标左键抬起时会执行的操作， ``OnRButtonDown``、 ``OnRButtonUp``则为右键按下和抬起时会执行的操作。以上是绘画前的准备工作。<br />
当我们按下鼠标左键时，将 ``BeginPoint``和 ``EndPoint``初始化为当前鼠标位置。根据习惯，当我们按住左键在工作区中拖动时，就会在工作区中显示相应的图形，当我们抬起左键，一个图形就完成了。因此在 ``OnMouseMove``函数中设置判断条件，只有在 ``nFlags & MK_LBUTTON``（鼠标移动且左键被按下）的情况下才会进行绘画。我们需要先创建一个句柄 ``CClientDC dc(this)``，由它帮助我们实现绘画的动作。接下来再为它选择合适的画笔，代码如下（以下代码均在``OnMouseMove``函数中）：
```
CPen newPen, choosePen, * oldPen;
newPen.CreatePen(PS_SOLID, Pen_Size, Pen_Color);
oldPen = dc.SelectObject(&newPen);
```
定义的画笔为实线，粗细和颜色为当前设定的。这里之所以需要 ``oldPen``，是为了防止内存泄露。然后判断当前的模式 ``m_Shape``，这里使用 ``switch``语句（因为后面还有很多其他图形），当 ``m_Shape``为 ``Shape:: Line``时开始画直线。然后使用dc自带的 ``MoveTo``和 ``LineTo``函数，它代表从 ``MoveTo``的点到 ``LineTo``的点画一条直线。<br />
但是仅仅将这两个函数调用一遍还不够，因为只要我们移动鼠标，程序就会不断执行这个函数，从而会看到屏幕随着鼠标的移动显示出“无数”直线，这与实际不符。为了实现实际画图中的“橡皮筋”效果，我们必须设置画笔模式为 ``R2_NOTXORPEN``，它的意思是将画笔颜色与屏幕像素值取异或再取反。即，在鼠标移动时，先将上次画出的直线擦除，再画出新的直线，这样就达到了直线随着我们的鼠标移动的效果。代码如下：
```
case Shape::Line:
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.MoveTo(BeginPoint);
	dc.LineTo(EndPoint);
  
	dc.MoveTo(BeginPoint);
	dc.LineTo(point);
	EndPoint = point;
	break;
```
前3条语句将上一步画出的线擦除，后2条语句画出新的直线。``point``为我们当前鼠标位置。每次绘画完毕，用 ``EndPoint``储存当前鼠标的位置，以便下次擦除上次的绘画痕迹。<br />
实际测试时还会发现如下问题，当有多条直线相交时，相交部分会变为白色。这是我们画笔模式选择了 ``R2_NOTXORPEN``的缘故。因此当我们抬起鼠标时，应当将这片白色的部分重新补回正常的颜色。所以我们还要在 ``OnLButtonUp``中添加如下代码，重新绘画一遍相交部分：
```
case Shape::Line:
	dc.MoveTo(BeginPoint);
	dc.LineTo(point);
	break;
```
至此直线功能完成。
#### 矩形
为 __矩形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Rectangle``。<br />
画矩形需要用到dc的 ``Rectangle``函数，它有多个重载，此处使用的是将一个已经确定的矩形绘画在相应的位置。此处定义的矩形为 ``CRect rectP(BeginPoint, EndPoint)`` ，它代表以 ``BeginPoint``为左上角， ``EndPoint``为右下角的一个矩形。使用与画直线时相同的策略，即可实现画矩形：
```
case Shape::Rectangle: {
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.SelectStockObject(PS_NULL);
	CRect rectP(BeginPoint, EndPoint);
	dc.Rectangle(rectP);
	CRect rectP2(BeginPoint, point);
	dc.Rectangle(rectP2);
	EndPoint = point;
	break;
}
```
注意实际操作时会发现一个问题：新画的矩形会盖住先前的图案，所以为解决此问题，在上面代码中增加一行： ``dc.SelectStockObject(PS_NULL)``，将图案设置为透明即可。<br />
同样地，最后要在 ``OnLButtonUp``中重新绘画重合的部分：
```
case Shape::Rectangle: {
	dc.SelectStockObject(PS_NULL);
	CRect rectP2(BeginPoint, point);
	dc.Rectangle(rectP2);
	break;
}
```
至此矩形功能完成。
#### 正方形
为 __正方形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Square``。<br />
正方形的画法由矩形改编而来。它的终止点不再由鼠标的当前横纵坐标确定而只由鼠标当前的纵坐标决定，因为正方形的长和宽相等，故只需用起始点和终止点之间纵坐标的差距就可同时确定长和宽。代码如下：
```
case Shape::Square: {
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.SelectStockObject(PS_NULL);
	int len1 = EndPoint.y - BeginPoint.y;
	if (EndPoint.x < BeginPoint.x) {//终点在起点左边
		EndPoint.x = BeginPoint.x - abs(len1);
	}
	else {//终点在起点右边
		EndPoint.x = BeginPoint.x + abs(len1);
	}
	CRect rectP(BeginPoint, EndPoint);
	dc.Rectangle(rectP);
	int len2 = point.y - BeginPoint.y;
	if (point.x < BeginPoint.x) {//终点在起点左边
		EndPoint.x = BeginPoint.x - abs(len2);
	}
	else {//终点在起点右边
		EndPoint.x = BeginPoint.x + abs(len2);
	}
	EndPoint.y = point.y;
	CRect rectP2(BeginPoint, EndPoint);
	dc.Rectangle(rectP2);
	EndPoint = point;
	break;
}
```
此处为了能向任意方向绘制正方形，增加了一个判断条件。抬起鼠标后，仍像之前那样重绘重合部分，这里不再赘述。至此正方形功能完成。
#### 椭圆
为 __椭圆__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Ellipse``。<br />
椭圆可用dc自带的 ``Ellipse``函数直接绘画。它的定义是一个矩形的内接椭圆，所以只需像画矩形那样先定义一个矩形，再将该矩形作为参数传入 ``Ellipse``即可。代码如下：
```
case Shape::Ellipse: {
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.SelectStockObject(PS_NULL);
	CRect rectP(BeginPoint, EndPoint);
	dc.Ellipse(rectP);
	CRect rectP2(BeginPoint, point);
	dc.Ellipse(rectP2);
	EndPoint = point;
	break;
}
```
重合部分的处理不再赘述。至此椭圆功能完成。
#### 圆形
为 __圆形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Circle``。<br />
圆形既可以看作是椭圆的特殊情况处理，也可参照正方形的情况处理。此处圆形的定义就是一个正方形的内接圆，所以在正方形的基础上，只需将绘画时用的 ``Rectangle``函数更改为 ``Ellipse``函数即可画出圆形。代码如下：
```
case Shape::Circle: {
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.SelectStockObject(PS_NULL);
	int len1 = EndPoint.y - BeginPoint.y;
	if (EndPoint.x < BeginPoint.x) {//终点在起点左边
		EndPoint.x = BeginPoint.x - abs(len1);
	}
	else {//终点在起点右边
		EndPoint.x = BeginPoint.x + abs(len1);
	}
	CRect rectP(BeginPoint, EndPoint);
	dc.Ellipse(rectP);

	int len2 = point.y - BeginPoint.y;
	if (point.x < BeginPoint.x) {//终点在起点左边
		EndPoint.x = BeginPoint.x - abs(len2);
	}
	else {//终点在起点右边
		EndPoint.x = BeginPoint.x + abs(len2);
	}
	EndPoint.y = point.y;
	CRect rectP2(BeginPoint, EndPoint);
	dc.Ellipse(rectP2);
	EndPoint = point;
	break;
}
```
重合部分的处理不再赘述。至此圆形功能完成。
#### 多边形
多边形包含三种图案：三角形、五边形、六边形（正方形和矩形已单独列出，故不在此范围内），每种图案有各自的画法，具体如下：<br />
首先为 __三角形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Triangle``。<br />
绘画三角形的思路是，以鼠标左键按下的点为起点，鼠标的当前位置为终点。三角形的第一个顶点就是起点，第二个顶点定义如下：其横坐标为起点横坐标 __加上__ 终点与起点的横坐标之差的绝对值，纵坐标为终点的纵坐标。第三个顶点定义如下：其横坐标为起点横坐标 __减去__ 终点与起点的横坐标之差的绝对值，纵坐标为终点的纵坐标。因此由定义可以看出，本系统只能画出等腰的三角形。通过dc中的 ``Polygon``函数将已定义的几个点顺次连接，就得到了三角形，代码如下：
```
case Shape::Triangle: {
	dc.SetROP2(R2_NOTXORPEN);//画笔颜色与屏幕像素值的异或再反色，达到画笔移动的效果
	dc.SelectStockObject(PS_NULL);
	int lenx = abs(EndPoint.x - BeginPoint.x);
	int leny = EndPoint.y - BeginPoint.y;
	CPoint pt[3] = { CPoint(BeginPoint.x,BeginPoint.y),
				    CPoint(BeginPoint.x + lenx,BeginPoint.y + leny),
					CPoint(BeginPoint.x - lenx,BeginPoint.y + leny) };
	dc.Polygon(pt,3);

	int lenx2 = abs(point.x - BeginPoint.x);
	int leny2 = point.y - BeginPoint.y;
	pt[0] = CPoint(BeginPoint.x, BeginPoint.y);
	pt[1] = CPoint(BeginPoint.x + lenx2, BeginPoint.y + leny2);
	pt[2] = CPoint(BeginPoint.x - lenx2, BeginPoint.y + leny2);
	dc.Polygon(pt, 3);
	EndPoint = point;
	break;
}
```
重合部分的处理不再赘述。至此三角形功能完成。

至于五边形，首先为 __五边形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Pentagon``。<br />
五边形的第一个顶点仍以起点作为定义。然后将其余四个点划分为两组，第一组的两个点横坐标为起点的横坐标加上/减去终点横坐标与起点横坐标差的绝对值，纵坐标为终点的纵坐标。另一组的两个点为起点的横坐标加上/减去终点横坐标与起点横坐标差的绝对值的一定倍数，这个倍数模糊估计为1+1/1.6（是一个 _magic number_）。纵坐标为起点的纵坐标加上终点纵坐标与起点纵坐标差的绝对值的一定倍数，这个倍数模糊估计为1/2.6（是一个 _magic number_）。同样使用 ``Polygon``将已定义的五个点顺次连接，就得到了五边形，代码较长，详见源代码。重合部分的处理不再赘述。至此五边形功能完成。

至于六边形，首先为 __六边形__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Hexagon``。<br />
六边形的第一个顶点以起点作为定义，第二个顶点的横坐标与起点横坐标相同，纵坐标与终点纵坐标相同。然后将其余四个点划分为两组，第一组的两个点横坐标为起点的横坐标加上/减去终点横坐标与起点横坐标差的绝对值，纵坐标为起点的纵坐标 __加上__ 终点纵坐标与起点纵坐标差的绝对值的一定倍数，这个倍数模糊估计为1/4。另一组的两个点横坐标则与第一组两个点相同，但纵坐标为另一倍数，这个倍数模糊估计为3/4。这样，再使用 ``Polygon``将已定义的六个点顺次连接，就得到了六边形，代码较长，详见源代码。重合部分的处理不再赘述。至此六边形功能完成。

### 文本框
为 __文本框__ 选项添加事件处理程序，当我们点击该选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Text``。<br />
我们调用项目中自带的 ``CEdit``类完成文本框的绘制。当鼠标左键按下时用 ``Text_Pos``记录文本框位置。鼠标移动时，创建一个 ``CEdit``指针 ``CEdit* m_edit = new CEdit()``，通过 ``Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(BeginPoint, point), this, Textid)``函数创建一个文本框，四个参数分别为文本框风格，文本框大小（从鼠标左键按下的起始点到鼠标当前坐标点），当前工作窗口，文本框编号（任意数字，初始化为100）。再使用 ``ShowWindow(SW_SHOW)``将其显示在屏幕上即可。属于 ``CSimpleDrawView``类的 ``m_Edit``时刻指向当前文本框，故每次更新 ``m_Edit``时都应先将其原本指向的对象释放，以免内存泄漏。屏幕上始终只会显示一个文本框，故不需要做额外处理。鼠标抬起后，再次重绘一遍当前文本框，就可将文本框长时间显示在屏幕上。<br />
接下来我们便可在文本框中输入一些文字，只要按下回车，就能将这些文字显示在屏幕上。为了实现该功能，我们必须借助类向导添加一个虚函数 ``PreTranslateMessage(MSG* pMsg)``，它负责响应我们的键盘消息。所以，我们将在该函数中添加代码，当我们按下回车键时即 ``(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)``，如果我们正在使用 ``Shape::Text``模式，就会用 ``GetWindowTextW``函数提取出文本框中的文字，然后将文本框释放，再用 ``TextOutW``仅将文字显示在屏幕上。注意当屏幕上存在文本框时如果我们切换到了其他绘画工具，根据习惯同样应该释放文本框，所以这时要进行和按下回车键一样的操作。具体代码如下：
```
//按下回车或从文本切换到了其他模式
if ((m_Shape != Shape::Text || (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && m_Shape == Shape::Text)) && m_Edit != nullptr) {
	CString pStr;
	m_Edit->GetWindowTextW(pStr);
	delete m_Edit;
	m_Edit = nullptr;
	CClientDC dc(this);
	dc.TextOutW(Text_Pos.x, Text_Pos.y, pStr);
	return TRUE;
}
```
至此文本框功能实现。
### 铅笔与橡皮
首先为 __铅笔__ 和 __橡皮__ 选项添加事件处理程序，当我们点击选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员分别更改为 ``Shape::Pencil``和 ``Shape::Eraser``。<br />
以铅笔为例，只需要让窗口实时跟踪我们鼠标的轨迹然后用某种颜色画出来即可。这和画直线十分相似，用 ``MoveTo``和 ``LineTo``即可实现，与直线不同的是，我们的起始点始终为鼠标上一时刻的坐标，这样就可实时跟踪轨迹。具体代码如下：
```
case Shape::Pencil: {
	BeginPoint = EndPoint;//终点做新起点
	EndPoint = point;
	dc.MoveTo(BeginPoint);
	dc.LineTo(EndPoint);
	break;
}
```
橡皮的操作与铅笔完全相同，只不过画笔颜色不一样，使用橡皮时强制将画笔颜色设置为背景色，这样鼠标经过之处均变为背景色，相当于擦除了原本的图案。橡皮的粗细用画笔的粗细代表。代码如下：
```
case Shape::Eraser: {
	COLORREF pColor = dc.GetBkColor();
	CPen newPen(PS_SOLID, Pen_Size, pColor);
	dc.SelectObject(&newPen);

	BeginPoint = EndPoint;//终点做新起点
	EndPoint = point;
	dc.MoveTo(BeginPoint);
	dc.LineTo(EndPoint);

	break;
}
```
至此铅笔与橡皮功能实现。
### 填充
为 __填充__ 选项添加事件处理程序，当我们点击选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Fill``。<br />
填充只需在鼠标按下的那一刻执行即可，所以相关代码将在 ``OnLButtonDown``中添加。当我们按下鼠标左键，如果是在 ``Shape::Fill``模式，就使用我们预先设定的填充颜色 ``Brush_Color``，然后利用 ``ExtFloodFill(BeginPoint.x, BeginPoint.y, RGB(255,255,255), FLOODFILLSURFACE)``函数进行填充。其中前两个参数代表我们鼠标按下的点。 ``FLOODFILLSURFACE``代表从按下的点开始，填充所有颜色为前一个参数的像素，直到碰到不是该颜色的像素为止。这样填充功能就完成了，代码如下：
```
if (m_Shape == Shape::Fill) {
	CClientDC Dc(this);
	CBrush cBr(Brush_Color);
	Dc.SelectObject(&cBr);
	Dc.ExtFloodFill(BeginPoint.x, BeginPoint.y, RGB(255,255,255), FLOODFILLSURFACE);
}
```
### 选择
选择功能的实现较为复杂。首先为 __选择区域__ 选项添加事件处理程序，当我们点击选项时，将 ``CSimpleDrawView``类中的 ``m_Shape``成员更改为 ``Shape::Choose``。以下功能均在该模式下执行。<br />
#### 选择某一区域
选择功能对于用户来说就是画出一个矩形框住一个范围，所以实现方法与画矩形相同。为避免与矩形混淆，我们用虚线画矩形，而且强制画笔粗细为最细，颜色为黑色。代码如下：
```
//该部分代码在OnMouseMove中
if (!Chosen) {
	CRect rectP(BeginPoint, EndPoint);
	FastRect(rectP);
	CRect rectP2(BeginPoint, point);
	FastRect(rectP2);
	EndPoint = point;
}
```
此处用到了快速描绘选框矩形的一个函数 ``FastRect``，详见源代码。
当我们抬起鼠标左键的一刻即代表选择完毕，此时置一个 ``bool``变量 ``Chosen``（在 ``CSimpleDrawView``类中定义）为 ``True``。这意味着我们已经选择了一个区域，可以进行接下来的几个操作。此时如果再次点击区域以外的位置，将会重新选择区域。代码如下：
```
//该部分代码在LButtonDown中
if (Chosen) {
	CRect area(Chooselt, Choosebr);
	if (!area.PtInRect(point))
	{
		/*要进行的操作*/
		Chosen = false;
		ClearRect(area);
	}
}
```
``ClearRect``是快速清除选框的函数，详见源代码。此外，如果此时切换到其他绘画模式，选框也会立即清除，在 ``PreTranslateMessage``中执行。

#### 拖动某一区域
为了实现拖动功能，选择完区域的同时，我们需要对当前画面做一些处理。由于接下来拖动该区域后，这片区域将变为空白，为实现这个效果，我们需要用到其他两个dc（须事先在``CSimpleDrawView``类中定义）: ``HDC bc_hdc``保存当前图像。然后将该选框内图像填为空白，再用 ``HDC ac_hdc``保存此时图像。这样拖动选区时，先重绘 ``ac_hdc``中的图像到工作区，再重绘 ``bc_hdc``中选区的那一块图像到工作区，就实现了拖动功能。当然，这时还应记录下选区的坐标（左上角和右下角）以及初始化有关拖动的工作变量以便后续处理。代码如下：
```
//该部分代码在LButtonUp中
case Shape::Choose: {
	if (!Chosen) {
		CRect rectP2(BeginPoint, point);
		ClearRect(rectP2);
		CRect rect;
		GetClientRect(&rect);
		HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//创建兼容位图
		bc_hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式
		SelectObject(bc_hdc, hbitmap);//将位图选入DC，并保存返回值 
		BitBlt(bc_hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);//将屏幕DC图像复制到内存DC
		CBrush cBr(RGB(255,255,255));
		dc.FillRect(rectP2, &cBr);
		hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);//创建兼容位图
		ac_hdc = CreateCompatibleDC(dc);
		SelectObject(ac_hdc, hbitmap);
		BitBlt(ac_hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
		StretchBlt(dc,BeginPoint.x, BeginPoint.y, rectP2.Width(), rectP2.Height(),
		bc_hdc, BeginPoint.x, BeginPoint.y, rectP2.Width(), rectP2.Height(), SRCCOPY);
		FastRect(rectP2, false);
		EndPoint = point;
		Chosen = true;
		Chooselt = BeginPoint;
		Choosebr = point;
		Startlt = Chooselt;
		Startbr = Choosebr;
		Tempclt = Chooselt;
		Tempcbr = Choosebr;
        }
}
```
其中 ``CPoint Chooselt, Choosebr, Tempclt, Tempcbr, Startlt, Startbr``需要事先定义在 ``CSimpleDrawView``类中，它们记录选框的左上角坐标（lt）和右下角坐标（br）。<br />
上面是在我们首次选择某区域后抬起左键执行的操作。此后再次点击鼠标左键，如果落点在区域内，我们就可以按住鼠标拖动该区域（否则就是重新选区）。 ``Chooselt``和 ``Choosebr``为选框静止的位置， ``Tempclt``和 ``Tempcbr``为选框移动过程中实时的位置。松开鼠标左键时，更新选框静止位置。代码如下：
```
//该部分代码在OnMouseMove中
CRect area(Tempclt, Tempcbr);
ClearRect(area);
CRect rect;
GetClientRect(&rect);
StretchBlt(dc, 0, 0, rect.Width(), rect.Height(),
		  ac_hdc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
int lenx = point.x - BeginPoint.x;
int leny = point.y - BeginPoint.y;
Tempclt = CPoint(Chooselt.x + lenx, Chooselt.y + leny);
Tempcbr = CPoint(Choosebr.x + lenx, Choosebr.y + leny);
CRect newarea(Tempclt, Tempcbr);
StretchBlt(dc,Tempclt.x, Tempclt.y, newarea.Width(), newarea.Height(),bc_hdc, Startlt.x, Startlt.y, Startbr.x- Startlt.x, Startbr.y - Startlt.y, SRCCOPY);
FastRect(newarea);
```
拖动功能实现。

#### 缩放某一区域
缩放功能是拖动功能的特殊情况，我们可以改变选区的大小。在拖动功能中，选区的左上角和右下角会随着鼠标移动。而在缩放功能中，固定左上角不动，只有右下角移动，就实现了选区大小的更改。此外 ``StretchBlt``函数也将自动将原选区的图像按比例画到新选区中，就实现了缩放功能。在操作上，本系统用按住鼠标右键移动的方式进行缩放。故还应对 ``OnRButtonDown``和 ``OnRButtonUp``函数添加一些必要代码。这里只列出缩放功能的代码：
```
//该部分代码在OnMouseMove中
if (nFlags & MK_RBUTTON) {
	CClientDC dc(this);
	if (m_Shape == Shape::Choose && Chosen) {
		CRect area(Tempclt, Tempcbr);
		ClearRect(area);
		CRect rect;
		GetClientRect(&rect);
		StretchBlt(dc, 0, 0, rect.Width(), rect.Height(),
			ac_hdc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
		int lenx = point.x - BeginPoint.x;
		int leny = point.y - BeginPoint.y;
		Tempcbr = CPoint(Choosebr.x + lenx, Choosebr.y + leny);
		CRect newarea(Tempclt, Tempcbr);
		StretchBlt(dc, Tempclt.x, Tempclt.y, newarea.Width(), newarea.Height(),
		bc_hdc, Startlt.x, Startlt.y, Startbr.x - Startlt.x, Startbr.y - Startlt.y, SRCCOPY);
		FastRect(newarea);
	}
}
```
#### 删除某一区域
本系统采用按下退格键删除区域的方式。故该部分在 ``PreTranslateMessage``中执行。只需将 ``ac_hdc``中保存的图像重绘到工作区并取消选框即可，代码如下：
```
if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_BACK) {//按下退格
	if (Chosen) {
		CClientDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		StretchBlt(dc, 0, 0, rect.Width(), rect.Height(),
				  ac_hdc, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
		Chosen = false;
		return TRUE;
	}
}
```
### 撤销
为 __撤销__ 选项添加事件处理程序。在 ``CSimpleDrawView``类中定义了一个列表 ``list<HBITMAP> SaveSeries``暂存所有的屏幕图像，当按下撤销快捷键时就调用该处理程序，取 ``SaveSeries``中最后一个图像绘制到工作区，并将其弹出即可。这里规定最大撤回步数为 _10_。<br />
具体代码放在了 ``CTempSave.cpp``中，在 ``CSimpleDrawView.cpp``中包含其头文件即可调用其中函数。此外，增加了一个限制条件，__在"选择区域"模式下不会暂存图像__ 。
### 保存与打开文件
为保存文件，额外添加一个 ``CSaveHelper``类，具体保存的方式与实现选择功能时的类似，使用项目自带的保存函数即可将图像保存。该系统仅支持保存为 ``bmp``格式。打开文件时从文件中加载图像，再将其绘制到工作区即可，仅支持打开 ``bmp``文件。打开文件时设置 ``m_Shape``成员为 ``Shape::LImage``，避免绘制出多余图案。详见源代码。
## 效果展示
![](/picture/8.png)<br />
#### 选择
![](/picture/9.png)<br />
#### 拖动
![](/picture/10.png)<br />
#### 修改大小
![](/picture/11.png)<br />
#### 保存
![](/picture/12.png)<br />

## 待完善的地方
* 鼠标在移动过程中图形会不停闪烁（需要使用双缓冲技术解决）。
* 文本框会遮盖住原本的图形，且其中文字不能修改大小。
* 清除选框时会使原本处在选框边界的线条消失。
* 尚未添加画正多边形的功能。
* 填充功能目前只支持填充白色背景。
* 代码存在较多冗余。
