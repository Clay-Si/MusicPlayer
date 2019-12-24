
// MusicPlayerDlg.h : 头文件
//

#pragma once

#include <mmsystem.h>
#include <Digitalv.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "ButtonUI.h"
#pragma comment(lib,"Winmm.lib")
//连接静态库文件

// CMusicPlayerDlg 对话框
class CMusicPlayerDlg : public CDialogEx
{
// 构造
public:
	CMusicPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	MCIDEVICEID m_DeviceID;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	void Play();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CSliderCtrl m_slider;
	int m_volume;
	afx_msg void OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedButton1();
	void GetFileList(CString path);
	CString FicowGetDirectory();
	CButtonUI m_btnClose;
	DWORD curTime;
	long lenTime;
	CListCtrl m_listCtrl;
	afx_msg void OnDbItemPlay(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateTime();
	int GetMusicTime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void CloseOrCancel(int i);
	CString m_playTime;
};
