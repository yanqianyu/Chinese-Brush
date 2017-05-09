// OpenGLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenGL.h"
#include "OpenGLDlg.h"
#include "afxdialogex.h"

#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#pragma warning( disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

														// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenGLDlg 对话框



COpenGLDlg::COpenGLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENGL_DIALOG, pParent)
{
	PixelFormat = 0;
	m_yRotate = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenGLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIZE_SCROLLBAR, m_size);
	DDX_Control(pDX, IDC_ALPHA_SCROLLBAR, m_alpha);
	DDX_Control(pDX, IDC_SPREAD_SCROLLBAR, m_spread);
}

BEGIN_MESSAGE_MAP(COpenGLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_COLOR_BUTTON, &COpenGLDlg::OnBnClickedColorButton)
	//ON_NOTIFY(NM_THEMECHANGED, IDC_SIZE_SCROLLBAR, &COpenGLDlg::OnNMThemeChangedSizeScrollbar)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &COpenGLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &COpenGLDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COpenGLDlg 消息处理程序

BOOL COpenGLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

									// TODO: 在此添加额外的初始化代码
	m_size.SetScrollRange(1, 100);
	m_alpha.SetScrollRange(1, 10);
	m_spread.SetScrollRange(1, 100);

	m_size.SetScrollPos(50);
	m_alpha.SetScrollPos(5);
	m_spread.SetScrollPos(50);

	SetDlgItemInt(IDC_SIZE_EDIT, 50);
	SetDlgItemInt(IDC_ALPHA_EDIT, 5);
	SetDlgItemInt(IDC_SPREAD_EDIT, 50);

	CWnd *wnd = GetDlgItem(IDC_RENDER);//OpenGL初始化
	hrenderDC = ::GetDC(wnd->m_hWnd);

	CRect rect;
	wnd->GetClientRect(&rect);
	m_width = rect.Width();
	m_height = rect.Height();
	m_pointNum = 0;
	m_simPoint = 0;
	m_LeftButtonDown = false;
	InitializeOpenGL(hrenderDC);

	SetTimer(0, 10, NULL);
	SetDlgItemInt(IDC_R_EDIT, 0.0);//初始化颜色框

	SetDlgItemInt(IDC_G_EDIT, 0.0);

	SetDlgItemInt(IDC_B_EDIT, 0.0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenGLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenGLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenGLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL COpenGLDlg::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
	if (PixelFormat == 0) // Choose default
	{
		PixelFormat = 1;
		if (DescribePixelFormat(hDC, PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}

	if (SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE)

	{
		return FALSE;
	}

	return TRUE;
}

BOOL COpenGLDlg::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);

	if (hrenderRC == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, hrenderRC) == FALSE)
		return FALSE;



	return TRUE;
}

BOOL COpenGLDlg::InitializeOpenGL(HDC hDC) {
	SetWindowPixelFormat(hDC);
	hrenderRC = ::wglCreateContext(hDC);
	if (hrenderRC == NULL) {
		return false;
	}
	if ((::wglMakeCurrent(hDC, hrenderRC)) == FALSE)
		return false;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, m_width, m_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-m_width / 2, m_width / 2, -m_height / 2, m_height / 2, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	SwapBuffers(hrenderDC);
	return true;
}

void COpenGLDlg::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBegin(GL_QUADS); //画布背景
	glColor3f(0.9, 0.9, 0.85);
	glVertex2f(-m_width / 2, m_height / 2);
	glVertex2f(m_width / 2, m_height / 2);
	glVertex2f(m_width / 2, -m_height / 2);
	glVertex2f(-m_width / 2, -m_height / 2);
	glEnd();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLfloat twoPi = 2.0f*3.1415926f;//圆模拟笔头
	int pNum = 0;
	for (int i = 0; i < m_pointNum; i++) {
		pNum = i % 10000;
		glColor4fv(m_point[pNum].color);
		glBegin(GL_POLYGON);
		for(int j=0;j<20;j++)
			glVertex2f(m_point[pNum].x + m_point[pNum].size*cos(j*twoPi/20), m_point[pNum].y + m_point[pNum].size*sin(j*twoPi/20));
		glEnd();
	}

	//Bezier();

	/*for (int i = 0; i < m_point2.size(); i++) {
		glColor4fv(m_point2[i].color);
		glBegin(GL_POLYGON);
		for (int j = 0; j<20; j++)
			glVertex2f(m_point2[i].x + m_point2[i].size*cos(j*twoPi / 20), m_point2[i].y + m_point2[i].size*sin(j*twoPi / 20));
		glEnd();
	}*/

	glDisable(GL_BLEND);

	SwapBuffers(hrenderDC);///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void COpenGLDlg::OnTimer(UINT nIDEvent) //实时绘制场景
{
	// TODO: Add your message handler code here and/or call default
	RenderScene();
	Spread();
	CDialog::OnTimer(nIDEvent);
}

void COpenGLDlg::OnBnClickedColorButton()
{
	// TODO: 在此添加控件通知处理程序代码
	COLORREF color = RGB(255, 255, 255);
	CColorDialog colorDlg(color);

	if (IDOK == colorDlg.DoModal()) {
		color = colorDlg.GetColor();
		SetDlgItemInt(IDC_R_EDIT, GetRValue(color));

		SetDlgItemInt(IDC_G_EDIT, GetGValue(color));

		SetDlgItemInt(IDC_B_EDIT, GetBValue(color));
	}
}

void COpenGLDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pScrollBar->GetDlgCtrlID() == IDC_ALPHA_SCROLLBAR) {
		int pos = pScrollBar->GetScrollPos();
		switch (nSBCode) {
		case SB_LINELEFT:
			pos -= 1;
			break;
		case SB_LINERIGHT:
			pos += 1;
			break;
		case SB_PAGELEFT:
			pos -= 2;
			break;
		case SB_PAGERIGHT:
			pos += 2;
			break;
		case SB_LEFT:
			pos = 1;
			break;
		case SB_RIGHT:
			pos = 10;
			break;
		case SB_THUMBPOSITION:
			pos = nPos;
			break;
		default:
				SetDlgItemInt(IDC_ALPHA_EDIT, pos);
			break;
		}
		pScrollBar->SetScrollPos(pos);
	}
	else {
		int pos = pScrollBar->GetScrollPos();
		switch (nSBCode) {
		case SB_LINELEFT:
			pos -= 1;
			break;
		case SB_LINERIGHT:
			pos += 1;
			break;
		case SB_PAGELEFT:
			pos -= 10;
			break;
		case SB_PAGERIGHT:
			pos += 10;
			break;
		case SB_LEFT:
			pos = 1;
			break;
		case SB_RIGHT:
			pos = 100;
			break;
		case SB_THUMBPOSITION:
			pos = nPos;
			break;
		default:
			if (pScrollBar->GetDlgCtrlID() == IDC_SIZE_SCROLLBAR)
				SetDlgItemInt(IDC_SIZE_EDIT, pos);
			else if (pScrollBar->GetDlgCtrlID() == IDC_ALPHA_SCROLLBAR)
				SetDlgItemInt(IDC_ALPHA_EDIT, pos);
			else if (pScrollBar->GetDlgCtrlID() == IDC_SPREAD_SCROLLBAR)
				SetDlgItemInt(IDC_SPREAD_EDIT, pos);
			break;
		}
		pScrollBar->SetScrollPos(pos);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COpenGLDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftButtonDown = true;
	m_MouseDownPos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}

void COpenGLDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认
	if (m_LeftButtonDown) {
		CRect rect;
		(this->GetDlgItem(IDC_RENDER))->GetWindowRect(&rect);
		ScreenToClient(&rect);//picture控件的绝对坐标

		double dx = m_MouseDownPos.x - point.x;
		double dy = m_MouseDownPos.y - point.y;
		double distance = sqrt(dx*dx + dy*dy);
		for (int i = 0; i < (int)(distance) + 1; i++) {
			int pNum = m_pointNum % 10000;
			
			m_point[pNum].x = m_MouseDownPos.x - m_width / 2 - rect.left + dx / distance*i;//按照距离的插点
			m_point[pNum].y = m_height / 2 - m_MouseDownPos.y + rect.top - dy / distance*i;

			CString sSIZE;
			GLfloat fSIZE;
			GetDlgItemText(IDC_SIZE_EDIT, sSIZE);
			fSIZE = _ttof(sSIZE);//getsize
			m_point[pNum].size = fSIZE*0.2;//粗细

			CString sLIFE;
			GLfloat fLIFE;
			GetDlgItemText(IDC_SPREAD_EDIT, sLIFE);
			fLIFE = _ttof(sLIFE);
			m_point[pNum].life = fLIFE;//颜色的晕染效果

			CString sR;
			GLfloat fR = 255.0;
			GetDlgItemText(IDC_R_EDIT, sR);//getcolor
			fR = _ttof(sR);
			CString sG;
			GLfloat fG = 255.0;
			GetDlgItemText(IDC_G_EDIT, sG);//getcolor
			fG = _ttof(sG);
			CString sB;
			GLfloat fB = 255.0;
			GetDlgItemText(IDC_B_EDIT, sB);//getcolor
			fB = _ttof(sB);

			CString sALPHA;
			GLfloat fALPHA;
			GetDlgItemText(IDC_ALPHA_EDIT, sALPHA);
			fALPHA = _ttof(sALPHA);
			m_point[pNum].color[0] = fR / 255.0;//笔尖的初始颜色
			m_point[pNum].color[1] = fG / 255.0;
			m_point[pNum].color[2] = fB / 255.0;
			m_point[pNum].color[3] = fALPHA *0.01;
			
			//m_point.push_back(temp);
			m_pointNum ++;
		}
	}
	m_MouseDownPos = point;
	CDialogEx::OnMouseMove(nFlags, point);
}
/*
void COpenGLDlg::Bezier() {
	float scale = 0.6;
	ColorPoint *midpoints = new ColorPoint[m_pointNum];
	//生成中点
	for (int i = 0; i < m_pointNum-1; i++) {
		int nexti = (i + 1) % m_pointNum;
		midpoints[i].x = (m_point[i].x + m_point[nexti].x) / 2;
		midpoints[i].y = (m_point[i].y + m_point[nexti].y) / 2;
	}
	//平移中点
	ColorPoint *extrapoint = new ColorPoint[m_pointNum  * 2];
	for (int i = 0; i < m_pointNum-1; i++) {
		int nexti = (i + 1) % m_pointNum;
		int backi = (i + m_pointNum - 1) % m_pointNum;
		ColorPoint midnmid;
		midnmid.x = (midpoints[i].x + midpoints[backi].x) / 2.0;
		midnmid.y = (midpoints[i].y + midpoints[backi].y) / 2.0;
		int offsetx = m_point[i].x - midnmid.x;
		int offsety = m_point[i].y - midnmid.y;
		int extraindex = 2 * i;
		extrapoint[extraindex].x = midpoints[backi].x + offsetx;
		extrapoint[extraindex].y = midpoints[backi].y + offsety;

		int addx = (extrapoint[extraindex].x - m_point[i].x)*scale;
		int addy = (extrapoint[extraindex].y - m_point[i].y)*scale;
		extrapoint[extraindex].x = m_point[i].x + addx;
		extrapoint[extraindex].y = m_point[i].y + addy;

		int extranexti = (extraindex + 1) % (2 * m_pointNum);
		extrapoint[extranexti].x = midpoints[i].x + offsetx;
		extrapoint[extranexti].y = midpoints[i].y + offsety;

		addx = (extrapoint[extraindex].x - m_point[i].x)*scale;
		addy = (extrapoint[extraindex].y - m_point[i].y)*scale;
		extrapoint[extraindex].x = m_point[i].x + addx;
		extrapoint[extraindex].y = m_point[i].y + addy;
	}

	ColorPoint controlPoint[4];
	for (int i = 0; i < m_pointNum; i++) {
		controlPoint[0] = m_point[i];
		int extraindex = 2 * i;
		controlPoint[1] = extrapoint[extraindex + 1];
		int extraindexi = (extraindex + 2) % (2 * m_pointNum);
		controlPoint[2] = extrapoint[extraindexi];
		int nexti = (i + 1) % m_pointNum;
		controlPoint[3] = m_point[nexti];
		float u = 1;
		while (u >= 0) {
			int px = controlPoint[0].x*u*u*u + 3 * controlPoint[1].x*u*u*(1 - u) +
				3 * controlPoint[2].x*u*(1 - u)*(1 - u) + controlPoint[3].x*(1 - u)*(1 - u)*(1 - u);
			int py = controlPoint[0].y*u*u*u + 3 * controlPoint[1].y*u*u*(1 - u) +
				3 * controlPoint[2].y*u*(1 - u)*(1 - u) + controlPoint[3].y*(1 - u)*(1 - u)*(1 - u);

			u -= 0.05;

			ColorPoint temp;
			temp.x = px;
			temp.y = py;
			CString sSIZE;
			GLfloat fSIZE;
			GetDlgItemText(IDC_SIZE_EDIT, sSIZE);
			fSIZE = _ttof(sSIZE);//getsize
			temp.size = fSIZE*0.2;//粗细

			CString sLIFE;
			GLfloat fLIFE;
			GetDlgItemText(IDC_SPREAD_EDIT, sLIFE);
			fLIFE = _ttof(sLIFE);
			temp.life = fLIFE;//颜色的晕染效果

			CString sR;
			GLfloat fR = 255.0;
			GetDlgItemText(IDC_R_EDIT, sR);//getcolor
			fR = _ttof(sR);
			CString sG;
			GLfloat fG = 255.0;
			GetDlgItemText(IDC_G_EDIT, sG);//getcolor
			fG = _ttof(sG);
			CString sB;
			GLfloat fB = 255.0;
			GetDlgItemText(IDC_B_EDIT, sB);//getcolor
			fB = _ttof(sB);

			CString sALPHA;
			GLfloat fALPHA;
			GetDlgItemText(IDC_ALPHA_EDIT, sALPHA);
			fALPHA = _ttof(sALPHA);
			temp.color[0] = fR / 255.0;//笔尖的初始颜色
			temp.color[1] = fG / 255.0;
			temp.color[2] = fB / 255.0;
			temp.color[3] = fALPHA /1000.0;
			m_point.push_back(temp);
		}
	}
}*/

void COpenGLDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftButtonDown = false;
	//m_pointNum = 0;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void COpenGLDlg::Spread() {
	for (int i = m_simPoint; i < m_pointNum; i++) {
		int pNum = i%10000;
		m_point[pNum].size *= 1.012;// 晕染效果
		m_point[pNum].color[0] += 0.003;//晕染过程颜色的变化
		m_point[pNum].color[1] += 0.003;
		m_point[pNum].color[2] += 0.003;
		m_point[pNum].color[3] -= 0.00001;
		m_point[pNum].life--;
		if(m_point[pNum].life <= 0)
			m_simPoint = i + 1;
	}
}

void COpenGLDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i <= 10000; i++) {//清空
		m_point[i].color[0] = 0.9;
		m_point[i].color[1] = 0.9;
		m_point[i].color[2] = 0.85;
		m_point[i].color[3] = 1;
	}
}

static char head[54] = {
	0x42,//0  
	0x4d,//1  
	0x66,//2  
	0x75,//3  
	0x00,//4  
	0x00,//5  
	0x00,//6  
	0x00,//7  
	0x00,//8  
	0x00,//9  
	0x36,//a  
	0x00,//b  
	0x00,//c  
	0x00,//d  
	0x28,//e  
	0x00,//f  
	0x00,//0  
	0x00,//1  
	0x64,//2  
	0x00,//3  
	0x00,//4  
	0x00,//5  
	0x64,//6  
	0x00,//7  
	0x00,//8  
	0x00,//9  
	0x01,//a  
	0x00,//b  
	0x18,//c  
	0x00,//d  
	0x00,//e  
	0x00,//f  
	0x00,//0  
	0x00,//1  
	0x30,//2  
	0x75//3  
};
//函数功能:OpengGL抓图  
//参数说明:width OpenGL窗体宽  
//                  height OpeGL窗体高  
void COpenGLDlg::grab(GLint width, GLint height)
{


	GLint pixelLength;
	GLubyte * pixelDate;
	FILE * wfile;
	//打开文件  
	wfile = fopen("grab.bmp", "wb");
	fwrite(head, 54, 1, wfile);
	//更改grab.bmp的头文件中的高度和宽度  
	fseek(wfile, 0x0012, SEEK_SET);
	fwrite(&width, sizeof(width), 1, wfile);
	fwrite(&height, sizeof(height), 1, wfile);
	//为像素分配内存  
	pixelLength = width * 3;
	if (pixelLength % 4 != 0)
	{
		pixelLength += 4 - pixelLength % 4;
	}
	pixelLength *= height;
	pixelDate = (GLubyte *)malloc(pixelLength);
	if (pixelDate == 0)
	{
		printf("/a/n分配内存失败!");
	}
	//读取窗口像素并存储  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelDate);
	//写入像素数据  
	fseek(wfile, 0, SEEK_END);
	fwrite(pixelDate, pixelLength, 1, wfile);
	fclose(wfile);
	free(pixelDate);

}
void COpenGLDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	grab(m_width, m_height);
	AfxMessageBox(L"grab.bmp saved");
}
