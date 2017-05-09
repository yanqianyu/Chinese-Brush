
// OpenGLDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <vector>
using namespace std;
// COpenGLDlg �Ի���
class COpenGLDlg : public CDialogEx
{
	// ����
public:
	COpenGLDlg(CWnd* pParent = NULL);	// ��׼���캯��

										// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENGL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	BOOL SetWindowPixelFormat(HDC hDC);   //�趨���ظ�ʽ
	BOOL CreateViewGLContext(HDC hDC);
	BOOL InitializeOpenGL(HDC hDC);
	//void LoadTextures();
	//View GL Context
	void RenderScene();                                //���Ƴ���

	HDC hrenderDC;      //DC
	HGLRC hrenderRC;  //RC
	float m_yRotate;       //ת��

	int PixelFormat;

	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	void Spread();//īˮ��Ⱦ��Ч��

	CPoint m_MouseDownPos;//�������
	struct ColorPoint {
		double x, y;//���� 
		GLfloat color[4];//��ɫ
		double size;//��С
		int life;//��Ⱦ
	};

	int m_pointNum;//�������
	int m_simPoint;//�ʼ����Ⱦ���ֵĿ�ʼ
	//vector<ColorPoint> m_point;
	ColorPoint m_point[10000];
	//vector<ColorPoint> m_point2;//��ֵ�õ��ĵ�
	//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
};
