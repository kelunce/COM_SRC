
// comt4Dlg.h : ͷ�ļ�
//

#pragma once


// Ccomt4Dlg �Ի���
class Ccomt4Dlg : public CDialog
{
// ����
public:
	Ccomt4Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_COMT4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
