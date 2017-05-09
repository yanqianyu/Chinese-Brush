
// OpenGLDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <vector>
using namespace std;
// COpenGLDlg 对话框
class COpenGLDlg : public CDialogEx
{
	// 构造
public:
	COpenGLDlg(CWnd* pParent = NULL);	// 标准构造函数

										// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENGL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	BOOL SetWindowPixelFormat(HDC hDC);   //设定象素格式
	BOOL CreateViewGLContext(HDC hDC);
	BOOL InitializeOpenGL(HDC hDC);
	//void LoadTextures();
	//View GL Context
	void RenderScene();                                //绘制场景

	HDC hrenderDC;      //DC
	HGLRC hrenderRC;  //RC
	float m_yRotate;       //转速

	int PixelFormat;

	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIEVENT);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedColorButton();
	CScrollBar m_size;
	CScrollBar m_alpha;
	CScrollBar m_spread;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	int m_width, m_height;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool m_LeftButtonDown;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg
	void Bezier();
	void OnLButtonUp(UINT nFlags, CPoint point);
	void grab(GLint width, GLint height);
	void Spread();//墨水晕染的效果

	CPoint m_MouseDownPos;//鼠标的落点
	struct ColorPoint {
		double x, y;//坐标 
		GLfloat color[4];//颜色
		double size;//大小
		int life;//晕染
	};

	int m_pointNum;//点的数量
	int m_simPoint;//笔尖后晕染部分的开始
	//vector<ColorPoint> m_point;
	ColorPoint m_point[10000];
	//vector<ColorPoint> m_point2;//插值得到的点
	//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};
