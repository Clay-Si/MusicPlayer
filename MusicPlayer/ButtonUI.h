#pragma once
#include "afxwin.h"
class CButtonUI :
	public CButton
{
public:
	CButtonUI();
	~CButtonUI();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

