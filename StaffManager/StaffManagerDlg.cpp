
// StaffManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StaffManager.h"
#include "StaffManagerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStaffManagerDlg 对话框




CStaffManagerDlg::CStaffManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaffManagerDlg::IDD, pParent)
{
	m_RegisterDlg = NULL;
	m_RecognizeDlg = NULL;
	m_InfoDlg = NULL;
	m_DataDlg = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nSel = 0;
}

CStaffManagerDlg::~CStaffManagerDlg()
{
	if (NULL != m_RegisterDlg)
	{
		delete m_RegisterDlg;
	}
	if (NULL != m_RecognizeDlg)
	{
		delete m_RecognizeDlg;
	}
	if (NULL != m_InfoDlg)
	{
		delete m_InfoDlg;
	}
	if (NULL != m_DataDlg)
	{
		delete m_DataDlg;
	}
}
void CStaffManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStaffManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INPUT_FACE_BUTTON, &CStaffManagerDlg::OnBnClickedInputFaceButton)
	ON_BN_CLICKED(IDC_RECOGNIZE_BUTTON, &CStaffManagerDlg::OnBnClickedRecognizeButton)
	ON_BN_CLICKED(IDC_DATA_BUTTON, &CStaffManagerDlg::OnBnClickedDataButton)
	ON_BN_CLICKED(ID_INFO_BUTTON, &CStaffManagerDlg::OnBnClickedInfoButton)
END_MESSAGE_MAP()


// CStaffManagerDlg 消息处理程序

BOOL CStaffManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (NULL == m_InfoDlg)
	{
		m_InfoDlg = new CInfoManageDlg();
		m_InfoDlg->Create(IDD_INFO_DIALOG,this);
	}
	if (NULL == m_RegisterDlg)
	{
		m_RegisterDlg = new CRegisterFaceDlg();
		m_RegisterDlg->Create(IDD_INPUT_FACE_DIALOG,this);
	}
	if (NULL == m_RecognizeDlg)
	{
		m_RecognizeDlg = new CRecognitionDlg();
		m_RecognizeDlg->Create(IDD_RECOGNITION_DIALOG,this);
	}
	if (NULL == m_DataDlg)
	{
		m_DataDlg = new CDataStatisticsDlg();
		m_DataDlg->Create(IDD_DATAMANAGEDIALOG,this);
	}
	
	GetClientRect(rect);
	GetDlgItem(ID_INFO_BUTTON)->GetWindowRect(rt);
	this->ScreenToClient(rt);

	rect.top = rt.bottom + 10;
	//m_AddDlg->MoveWindow(rect);
	m_InfoDlg->MoveWindow(rect);
	m_RegisterDlg->MoveWindow(rect);
	m_RecognizeDlg->MoveWindow(rect);
	m_DataDlg->MoveWindow(rect);

	ps[0] = m_InfoDlg;
	ps[1] = m_RegisterDlg;
	ps[2] = m_RecognizeDlg;
	ps[3] = m_DataDlg;

	m_InfoDlg->ShowWindow(SW_SHOW);
	
	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStaffManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStaffManagerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStaffManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStaffManagerDlg::OnBnClickedInfoButton()
{
	m_nSel = 0;
	SelectPage();
}


void CStaffManagerDlg::OnBnClickedInputFaceButton()
{
	m_nSel = 1;
	m_RegisterDlg->m_list->DeleteAllItems();
	m_RegisterDlg->readStaff(m_RegisterDlg->m_list);
	SelectPage();
}

void CStaffManagerDlg::OnBnClickedRecognizeButton()
{
	m_nSel = 2;
	m_RecognizeDlg->SetDlgItemText(IDC_NAME_EDIT,"");
	m_RecognizeDlg->SetDlgItemText(IDC_NO_EDIT,"");
	m_RecognizeDlg->SetDlgItemText(IDC_SEX_EDIT,"");
	m_RecognizeDlg->SetDlgItemText(IDC_DUTY_EDIT,"");
	m_RecognizeDlg->SetDlgItemText(IDC_TEL_EDIT,"");
	
	m_RecognizeDlg->model = Ptr<FaceRecognizer>();
	m_RecognizeDlg->GetFacesModel(m_RecognizeDlg->model);
	SelectPage();
	
}


void CStaffManagerDlg::OnBnClickedDataButton()
{
	m_nSel = 3;
	m_DataDlg->m_list->DeleteAllItems();
	m_DataDlg->ReadAllLogs(m_DataDlg->m_list);
	SelectPage();
}


void CStaffManagerDlg::SelectPage()
{
	int i = 0;
	while(i < sizeof(ps)/sizeof(ps[0])){
		ps[i++]->ShowWindow(i == m_nSel ? SW_SHOW:SW_HIDE);
	}
}
