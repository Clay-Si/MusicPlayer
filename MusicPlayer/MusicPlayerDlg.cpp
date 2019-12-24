
// MusicPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
//#include "WinBase.h "




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicPlayerDlg 对话框



CMusicPlayerDlg::CMusicPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MUSICPLAYER_DIALOG, pParent)
	, m_volume(0)
	, m_playTime(_T(""))
{
	//获取图标
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_DeviceID = 0;
	//初始化记录音乐的ID
}

void CMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_EDIT1, m_volume);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CMusicPlayerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMusicPlayerDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CMusicPlayerDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMusicPlayerDlg::OnBnClickedBtnStop)
	ON_WM_DROPFILES()
	ON_WM_NCHITTEST()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMusicPlayerDlg::OnCustomdrawSlider1)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMusicPlayerDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BUTTON1, &CMusicPlayerDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMusicPlayerDlg::OnDbItemPlay)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMusicPlayerDlg 消息处理程序

BOOL CMusicPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/*控制音量*/
	//设置滑动范围
	m_slider.SetRange(0, 1000);

	m_listCtrl.SetExtendedStyle(LVS_EX_FLATSB
		| LVS_EX_FULLROWSELECT
		| LVS_EX_HEADERDRAGDROP
		| LVS_EX_ONECLICKACTIVATE
		| LVS_EX_GRIDLINES);
	//设置初始值
	m_slider.SetPos(1000);
	m_listCtrl.DeleteAllItems();
	m_listCtrl.InsertColumn(0, _T("歌曲名"), LVCFMT_CENTER, 180);
	m_listCtrl.InsertColumn(1, _T("歌曲大小"), LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(2, _T("歌曲路径"), LVCFMT_CENTER, 200);


	SetDlgItemText(IDC_EDIT2, _T("00:00"));
	SetDlgItemText(IDC_PLAY_TIME, _T("00:00:00"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMusicPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CPaintDC dc(this);
		//获取客户端的尺寸
		CRect rect;
		GetClientRect(&rect);

		//绘制标题栏
		CRect captionRect = rect;
		captionRect.bottom = 50;//高度
		dc.FillSolidRect(&captionRect,RGB(225, 225, 225));//颜色
		
		//输出昵称
		CFont font;
		font.CreatePointFont(110,L"黑体");
		dc.SetTextColor(RGB(255, 1, 1));
		dc.SelectObject(&font);
		dc.TextOut(48, 17, L"Clay");

		//输出图标（头像）
		DrawIconEx(dc.m_hDC, 10, 10, m_hIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK);
		

		//加载图片
		CBitmap bmp;
		bmp.LoadBitmap(IDB_MAIN_BG);

		//获取图片尺寸
		BITMAP bmpSize;
		bmp.GetBitmap(&bmpSize);

		//创建内存画板
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&bmp);
		//绘制
		//无法全图dc.BitBlt(0, 0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
		
		//拉伸从内存DC的（0，0）位置的图片以原尺寸绘制在客户端尺寸的（0，0）处
		dc.SetStretchBltMode(HALFTONE);//不以最节约内存的方式拉伸
		dc.StretchBlt(0, 50, rect.Width(), rect.Height(), &memDC, 0, 0, bmpSize.bmWidth,
			bmpSize.bmHeight,SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开
void CMusicPlayerDlg::OnBnClickedBtnOpen()
{
	GetFileList(FicowGetDirectory());
}

//播放
void CMusicPlayerDlg::Play()
{
	//播放位置
	MCI_PLAY_PARMS mciPlayParms;
	mciPlayParms.dwCallback = NULL;
	//从头开始播放
	mciPlayParms.dwFrom = 0;
	mciSendCommand(m_DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, 
		(DWORD)&mciPlayParms);
	SetTimer(1, 1000, NULL);
	
}

//暂停、继续
void CMusicPlayerDlg::OnBnClickedBtnPause()
{
	//获取按钮文字
	CString str;
	GetDlgItemText(IDC_BTN_PAUSE,str);

	if (str == L"暂停")
	{	
		KillTimer(1);
		mciSendCommand(m_DeviceID,MCI_PAUSE,0,0);
		SetDlgItemText(IDC_BTN_PAUSE, L"继续");
	}
	else if (str == L"继续")
	{
		SetTimer(1, 1000, NULL);
		mciSendCommand(m_DeviceID, MCI_RESUME, 0, 0);
		SetDlgItemText(IDC_BTN_PAUSE, L"暂停");
	}

}

//停止
void CMusicPlayerDlg::OnBnClickedBtnStop()
{
	KillTimer(1);
	SetDlgItemText(IDC_PLAY_TIME, _T("00:00"));
	mciSendCommand(m_DeviceID, MCI_STOP, 0, 0);
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);
	
}

//文件拖拽播放
void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	//hDropInfo拖拽句柄

	//获取拖拽文件路径
	wchar_t szMusicPath[MAX_PATH];//字符数组
	DragQueryFile(hDropInfo,0, szMusicPath,MAX_PATH);
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//获取路径后打开音乐
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);

	//打开音乐
	MCI_OPEN_PARMS mciOpenParms;
	mciOpenParms.lpstrElementName = szMusicPath;
	//DWORD unsigned long
	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT,
		(DWORD)&mciOpenParms);

	//判断音乐是否出错
	if (mciError)
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);
		AfxMessageBox(szErrorMsg);
		return;
	}

	//当前歌曲ID
	m_DeviceID = mciOpenParms.wDeviceID;
	//调用播放
	Play();

	CDialogEx::OnDropFiles(hDropInfo);
}

//无边框可拖动
LRESULT CMusicPlayerDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//当前点击的内容
	UINT nHitTest = CDialogEx::OnNcHitTest(point);

	//获取客户区大小
	CRect rect;
	GetClientRect(&rect);
	rect.bottom = 50;
	/*只有标题栏可拖动*/
	//坐标系转换
	ScreenToClient(&point);
	//判断鼠标位置
	if (rect.PtInRect(point))
	{
		//在客户区就转换到标题栏
		if (nHitTest == HTCLIENT)
		{
			nHitTest = HTCAPTION;
		}
	}
	return nHitTest;
}

//滑动控制音量值
void CMusicPlayerDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//获取当前滑动控件位置
	m_volume = m_slider.GetPos()/10;
	//显示
	UpdateData(FALSE);

	MCI_DGV_SETAUDIO_PARMS mciSetVolume;
	mciSetVolume.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetVolume.dwValue = m_slider.GetPos();
	mciSendCommand(m_DeviceID, MCI_SETAUDIO, 
		MCI_DGV_SETAUDIO_VALUE|MCI_DGV_SETAUDIO_ITEM,(DWORD)&mciSetVolume);

	*pResult = 0;
}

//画背景
BOOL CMusicPlayerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnEraseBkgnd(pDC);//灰色背景
	return TRUE;
}

//点击关闭
void CMusicPlayerDlg::OnBnClickedBtnClose()
{
	CString editContent;
	GetDlgItem(IDC_PLAY_TIME)->GetWindowText(editContent);
	int i = MessageBox(L"您本次听音乐时长为"+editContent+L"，是否关闭？",L"温馨提示"
	,MB_OKCANCEL);
	CloseOrCancel(i);
	// TODO: 在此添加控件通知处理程序代码

}

//最小化
void CMusicPlayerDlg::OnBnClickedButton1()
{
	theApp.GetMainWnd()->ShowWindow(SW_MINIMIZE);
	// TODO: 在此添加控件通知处理程序代码
}

//查找歌曲列表
void CMusicPlayerDlg::GetFileList(CString path)
{
	//删除list control显示的数据
	m_listCtrl.DeleteAllItems();
	CFileFind finder;
	//建立对象设置检索匹配字符串

	//文件内部检索
	BOOL bWorking = finder.FindFile(path+_T("\\*.mp3"));
	//C++中传进来的路径会自动将\转换为"\\"
	int index = 0;//索引
	CString fileName,fileLen,createTime,filePath;
	CTime tempTime;
	//循环检索
	while (bWorking)
	{
		//文件名
		bWorking = finder.FindNextFile();
		fileName = finder.GetFileName();
		index = m_listCtrl.InsertItem(0xfff, _T(""));
		m_listCtrl.SetItemText(index, 0, fileName);
		
		//文件大小
		ULONGLONG fileSize = finder.GetLength() / 1024 + 1;

		fileLen.Format(_T("%dKB"), fileSize);
		m_listCtrl.SetItemText(index, 1, (LPCTSTR)fileLen);
		/*
		//创建时间
		finder.GetCreationTime(tempTime);//创建时间
		createTime = tempTime.Format(_T("%Y/%m/%d %H:%M:%S"));
		m_listCtrl.SetItemText(index, 2, createTime);
		*/
		//文件路径
		filePath = finder.GetFilePath();
		m_listCtrl.SetItemText(index, 2, filePath);
	}

}

//选择文件夹
CString CMusicPlayerDlg::FicowGetDirectory()
{
	CString m_fileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH * 2];
	szFolder[0] = _T('/0');

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}
		IMalloc *pMalloc = NULL;

		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}

	}
	m_fileDir = szFolder;
	return m_fileDir;

}

//双击播放
void CMusicPlayerDlg::OnDbItemPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	/*POSITION ps = m_listCtrl.GetFirstSelectedItemPosition();
	int row = m_listCtrl.GetNextSelectedItem(ps);
	m_listCtrl.SetItemState(row, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);*/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int m_Row = pNMListView->iItem;//获得选中的行 
	
	//m_listCtrl.SetItemState(m_Row,LVS_EX_FULLROWSELECT , LVS_EX_FULLROWSELECT);
	CString musicPath;
	//获取列表中的第三列作为地址
	musicPath = m_listCtrl.GetItemText(m_Row, 2);
	
	//关闭上一个音乐（多媒体设备接口MCI）
	//音乐ID记录音乐
	//mciSendCommand驱动声卡
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);

	//打开音乐
	MCI_OPEN_PARMS mciOpenParms;
	mciOpenParms.lpstrElementName = musicPath;
	//DWORD unsigned long
	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT,
		(DWORD)&mciOpenParms);

	//判断音乐是否出错
	if (mciError)
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);
		AfxMessageBox(szErrorMsg);
		return;
	}

	//当前歌曲ID
	m_DeviceID = mciOpenParms.wDeviceID;

	//获取歌曲时长
	GetMusicTime();
	
	//调用播放
	Play();
}


//获取音乐时长
int CMusicPlayerDlg::GetMusicTime()
{
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(m_DeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciStatusParms);
	lenTime = mciStatusParms.dwReturn;//ms
	//将毫秒换成秒
	int totalTime = lenTime / 1000;
	int minutes = totalTime / 60;
	int seconds = totalTime % 60;
	CString str;
	str.Format(_T("%02i:%02i"), minutes, seconds);
	SetDlgItemText(IDC_EDIT2, str);
	return lenTime;
}


void CMusicPlayerDlg::UpdateTime()
{	

}

//计时器
void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 static UINT min = 0;
	 static UINT sec = 0;
	 static UINT hour = 0;

	CString str;
	//计时器ID
	switch (nIDEvent)
	{
	case 1:
		
			sec++;
			if (sec == 60)
			{
				min++;
				sec = 0;

			}
			if (min == 60)
			{
				hour++;
				min = 0;
			}
			str.Format(_T("%02i:%02i:%02i"),hour, min, sec);
			SetDlgItemText(IDC_PLAY_TIME, str);
	}
	
	CDialogEx::OnTimer(nIDEvent);

}

void CMusicPlayerDlg::CloseOrCancel(int i)
{
	if (i == 1)
	{
		EndDialog(IDOK);
	}
	else {
		return;
	}
}