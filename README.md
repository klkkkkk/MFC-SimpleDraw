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
