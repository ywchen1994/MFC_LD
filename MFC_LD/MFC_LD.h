
// MFC_LD.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMFC_LDApp: 
// �аѾ\��@�����O�� MFC_LD.cpp
//

class CMFC_LDApp : public CWinApp
{
public:
	CMFC_LDApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMFC_LDApp theApp;