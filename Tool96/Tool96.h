
// Tool96.h : Tool96 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CTool96App:
// �� Ŭ������ ������ ���ؼ��� Tool96.cpp�� �����Ͻʽÿ�.
//

class CTool96App : public CWinApp
{
public:
	CTool96App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTool96App theApp;
