
// comt4Dlg.h : 头文件
//

#pragma once


// Ccomt4Dlg 对话框
class Ccomt4Dlg : public CDialog
{
// 构造
public:
	Ccomt4Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COMT4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);
    void    CreateShortcut(LPCTSTR lpszExe, LPCTSTR lpszLnk);
    BOOL m_bShowPic;
};
