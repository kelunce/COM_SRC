
// comt4.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ccomt4App:
// �йش����ʵ�֣������ comt4.cpp
//

class Ccomt4App : public CWinAppEx
{
public:
	Ccomt4App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ccomt4App theApp;