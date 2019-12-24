
// MusicPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
//#include "WinBase.h "




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicPlayerDlg �Ի���



CMusicPlayerDlg::CMusicPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MUSICPLAYER_DIALOG, pParent)
	, m_volume(0)
	, m_playTime(_T(""))
{
	//��ȡͼ��
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_DeviceID = 0;
	//��ʼ����¼���ֵ�ID
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


// CMusicPlayerDlg ��Ϣ�������

BOOL CMusicPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	/*��������*/
	//���û�����Χ
	m_slider.SetRange(0, 1000);

	m_listCtrl.SetExtendedStyle(LVS_EX_FLATSB
		| LVS_EX_FULLROWSELECT
		| LVS_EX_HEADERDRAGDROP
		| LVS_EX_ONECLICKACTIVATE
		| LVS_EX_GRIDLINES);
	//���ó�ʼֵ
	m_slider.SetPos(1000);
	m_listCtrl.DeleteAllItems();
	m_listCtrl.InsertColumn(0, _T("������"), LVCFMT_CENTER, 180);
	m_listCtrl.InsertColumn(1, _T("������С"), LVCFMT_CENTER, 80);
	m_listCtrl.InsertColumn(2, _T("����·��"), LVCFMT_CENTER, 200);


	SetDlgItemText(IDC_EDIT2, _T("00:00"));
	SetDlgItemText(IDC_PLAY_TIME, _T("00:00:00"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMusicPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CPaintDC dc(this);
		//��ȡ�ͻ��˵ĳߴ�
		CRect rect;
		GetClientRect(&rect);

		//���Ʊ�����
		CRect captionRect = rect;
		captionRect.bottom = 50;//�߶�
		dc.FillSolidRect(&captionRect,RGB(225, 225, 225));//��ɫ
		
		//����ǳ�
		CFont font;
		font.CreatePointFont(110,L"����");
		dc.SetTextColor(RGB(255, 1, 1));
		dc.SelectObject(&font);
		dc.TextOut(48, 17, L"Clay");

		//���ͼ�꣨ͷ��
		DrawIconEx(dc.m_hDC, 10, 10, m_hIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK);
		

		//����ͼƬ
		CBitmap bmp;
		bmp.LoadBitmap(IDB_MAIN_BG);

		//��ȡͼƬ�ߴ�
		BITMAP bmpSize;
		bmp.GetBitmap(&bmpSize);

		//�����ڴ滭��
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&bmp);
		//����
		//�޷�ȫͼdc.BitBlt(0, 0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
		
		//������ڴ�DC�ģ�0��0��λ�õ�ͼƬ��ԭ�ߴ�����ڿͻ��˳ߴ�ģ�0��0����
		dc.SetStretchBltMode(HALFTONE);//�������Լ�ڴ�ķ�ʽ����
		dc.StretchBlt(0, 50, rect.Width(), rect.Height(), &memDC, 0, 0, bmpSize.bmWidth,
			bmpSize.bmHeight,SRCCOPY);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��
void CMusicPlayerDlg::OnBnClickedBtnOpen()
{
	GetFileList(FicowGetDirectory());
}

//����
void CMusicPlayerDlg::Play()
{
	//����λ��
	MCI_PLAY_PARMS mciPlayParms;
	mciPlayParms.dwCallback = NULL;
	//��ͷ��ʼ����
	mciPlayParms.dwFrom = 0;
	mciSendCommand(m_DeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, 
		(DWORD)&mciPlayParms);
	SetTimer(1, 1000, NULL);
	
}

//��ͣ������
void CMusicPlayerDlg::OnBnClickedBtnPause()
{
	//��ȡ��ť����
	CString str;
	GetDlgItemText(IDC_BTN_PAUSE,str);

	if (str == L"��ͣ")
	{	
		KillTimer(1);
		mciSendCommand(m_DeviceID,MCI_PAUSE,0,0);
		SetDlgItemText(IDC_BTN_PAUSE, L"����");
	}
	else if (str == L"����")
	{
		SetTimer(1, 1000, NULL);
		mciSendCommand(m_DeviceID, MCI_RESUME, 0, 0);
		SetDlgItemText(IDC_BTN_PAUSE, L"��ͣ");
	}

}

//ֹͣ
void CMusicPlayerDlg::OnBnClickedBtnStop()
{
	KillTimer(1);
	SetDlgItemText(IDC_PLAY_TIME, _T("00:00"));
	mciSendCommand(m_DeviceID, MCI_STOP, 0, 0);
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);
	
}

//�ļ���ק����
void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	//hDropInfo��ק���

	//��ȡ��ק�ļ�·��
	wchar_t szMusicPath[MAX_PATH];//�ַ�����
	DragQueryFile(hDropInfo,0, szMusicPath,MAX_PATH);
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��ȡ·���������
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);

	//������
	MCI_OPEN_PARMS mciOpenParms;
	mciOpenParms.lpstrElementName = szMusicPath;
	//DWORD unsigned long
	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT,
		(DWORD)&mciOpenParms);

	//�ж������Ƿ����
	if (mciError)
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);
		AfxMessageBox(szErrorMsg);
		return;
	}

	//��ǰ����ID
	m_DeviceID = mciOpenParms.wDeviceID;
	//���ò���
	Play();

	CDialogEx::OnDropFiles(hDropInfo);
}

//�ޱ߿���϶�
LRESULT CMusicPlayerDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ǰ���������
	UINT nHitTest = CDialogEx::OnNcHitTest(point);

	//��ȡ�ͻ�����С
	CRect rect;
	GetClientRect(&rect);
	rect.bottom = 50;
	/*ֻ�б��������϶�*/
	//����ϵת��
	ScreenToClient(&point);
	//�ж����λ��
	if (rect.PtInRect(point))
	{
		//�ڿͻ�����ת����������
		if (nHitTest == HTCLIENT)
		{
			nHitTest = HTCAPTION;
		}
	}
	return nHitTest;
}

//������������ֵ
void CMusicPlayerDlg::OnCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ��ǰ�����ؼ�λ��
	m_volume = m_slider.GetPos()/10;
	//��ʾ
	UpdateData(FALSE);

	MCI_DGV_SETAUDIO_PARMS mciSetVolume;
	mciSetVolume.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	mciSetVolume.dwValue = m_slider.GetPos();
	mciSendCommand(m_DeviceID, MCI_SETAUDIO, 
		MCI_DGV_SETAUDIO_VALUE|MCI_DGV_SETAUDIO_ITEM,(DWORD)&mciSetVolume);

	*pResult = 0;
}

//������
BOOL CMusicPlayerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialogEx::OnEraseBkgnd(pDC);//��ɫ����
	return TRUE;
}

//����ر�
void CMusicPlayerDlg::OnBnClickedBtnClose()
{
	CString editContent;
	GetDlgItem(IDC_PLAY_TIME)->GetWindowText(editContent);
	int i = MessageBox(L"������������ʱ��Ϊ"+editContent+L"���Ƿ�رգ�",L"��ܰ��ʾ"
	,MB_OKCANCEL);
	CloseOrCancel(i);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}

//��С��
void CMusicPlayerDlg::OnBnClickedButton1()
{
	theApp.GetMainWnd()->ShowWindow(SW_MINIMIZE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//���Ҹ����б�
void CMusicPlayerDlg::GetFileList(CString path)
{
	//ɾ��list control��ʾ������
	m_listCtrl.DeleteAllItems();
	CFileFind finder;
	//�����������ü���ƥ���ַ���

	//�ļ��ڲ�����
	BOOL bWorking = finder.FindFile(path+_T("\\*.mp3"));
	//C++�д�������·�����Զ���\ת��Ϊ"\\"
	int index = 0;//����
	CString fileName,fileLen,createTime,filePath;
	CTime tempTime;
	//ѭ������
	while (bWorking)
	{
		//�ļ���
		bWorking = finder.FindNextFile();
		fileName = finder.GetFileName();
		index = m_listCtrl.InsertItem(0xfff, _T(""));
		m_listCtrl.SetItemText(index, 0, fileName);
		
		//�ļ���С
		ULONGLONG fileSize = finder.GetLength() / 1024 + 1;

		fileLen.Format(_T("%dKB"), fileSize);
		m_listCtrl.SetItemText(index, 1, (LPCTSTR)fileLen);
		/*
		//����ʱ��
		finder.GetCreationTime(tempTime);//����ʱ��
		createTime = tempTime.Format(_T("%Y/%m/%d %H:%M:%S"));
		m_listCtrl.SetItemText(index, 2, createTime);
		*/
		//�ļ�·��
		filePath = finder.GetFilePath();
		m_listCtrl.SetItemText(index, 2, filePath);
	}

}

//ѡ���ļ���
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

//˫������
void CMusicPlayerDlg::OnDbItemPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	/*POSITION ps = m_listCtrl.GetFirstSelectedItemPosition();
	int row = m_listCtrl.GetNextSelectedItem(ps);
	m_listCtrl.SetItemState(row, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);*/
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int m_Row = pNMListView->iItem;//���ѡ�е��� 
	
	//m_listCtrl.SetItemState(m_Row,LVS_EX_FULLROWSELECT , LVS_EX_FULLROWSELECT);
	CString musicPath;
	//��ȡ�б��еĵ�������Ϊ��ַ
	musicPath = m_listCtrl.GetItemText(m_Row, 2);
	
	//�ر���һ�����֣���ý���豸�ӿ�MCI��
	//����ID��¼����
	//mciSendCommand��������
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, 0);

	//������
	MCI_OPEN_PARMS mciOpenParms;
	mciOpenParms.lpstrElementName = musicPath;
	//DWORD unsigned long
	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT,
		(DWORD)&mciOpenParms);

	//�ж������Ƿ����
	if (mciError)
	{
		wchar_t szErrorMsg[256];
		mciGetErrorString(mciError, szErrorMsg, 256);
		AfxMessageBox(szErrorMsg);
		return;
	}

	//��ǰ����ID
	m_DeviceID = mciOpenParms.wDeviceID;

	//��ȡ����ʱ��
	GetMusicTime();
	
	//���ò���
	Play();
}


//��ȡ����ʱ��
int CMusicPlayerDlg::GetMusicTime()
{
	MCI_STATUS_PARMS mciStatusParms;
	mciStatusParms.dwItem = MCI_STATUS_LENGTH;
	mciSendCommand(m_DeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciStatusParms);
	lenTime = mciStatusParms.dwReturn;//ms
	//�����뻻����
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

//��ʱ��
void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 static UINT min = 0;
	 static UINT sec = 0;
	 static UINT hour = 0;

	CString str;
	//��ʱ��ID
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