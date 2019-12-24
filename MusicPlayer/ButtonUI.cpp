#include "stdafx.h"
#include "ButtonUI.h"


CButtonUI::CButtonUI()
{
}


CButtonUI::~CButtonUI()
{
}

//自绘
void CButtonUI::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//CDC dc;
	//关联
	//dc.Attach(lpDrawItemStruct->hDC);//绘图句柄
	//画图
	
	//CRect rect = lpDrawItemStruct->rcItem;
	//dc.FillSolidRect(&rect,RGB(51,51,51));
	//分离
	//dc.Detach();
	// TODO:  添加您的代码以绘制指定项
}
