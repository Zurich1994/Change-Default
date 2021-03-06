
// ChangeDefaultProgramsInWin10Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChangeDefaultProgramsInWin10.h"
#include "ChangeDefaultProgramsInWin10Dlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChangeDefaultProgramsInWin10Dlg 对话框



CChangeDefaultProgramsInWin10Dlg::CChangeDefaultProgramsInWin10Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEDEFAULTPROGRAMSINWIN10_DIALOG, pParent)
	, _fileExtension(_T(""))
	, _filePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChangeDefaultProgramsInWin10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, _fileExtension);
	DDX_Text(pDX, IDC_EDIT1, _filePath);
}

BEGIN_MESSAGE_MAP(CChangeDefaultProgramsInWin10Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CChangeDefaultProgramsInWin10Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeDefaultProgramsInWin10Dlg 消息处理程序

BOOL CChangeDefaultProgramsInWin10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	_fileExtension = "aaa";
	_filePath = "C:\\Windows\\System32\\notepad.exe";

	UpdateData(FALSE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChangeDefaultProgramsInWin10Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChangeDefaultProgramsInWin10Dlg::OnPaint()
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
HCURSOR CChangeDefaultProgramsInWin10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChangeDefaultProgramsInWin10Dlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (_fileExtension.Trim().IsEmpty()) {
		AfxMessageBox("请输入后缀名!");
		return;
	}

	if (_filePath.Trim().IsEmpty()) {
		AfxMessageBox("请输入默认应用程序!");
		return;
	}

	if (_fileExtension.GetAt(0) != '.') {
		_fileExtension.Insert(0, ".");
	}

	CString realFolder = "nt_extension" + _fileExtension;


	HKEY pKey;
	LSTATUS s = RegOpenKey(HKEY_CLASSES_ROOT, _fileExtension, &pKey);
	if (s == ERROR_SUCCESS) {
		if (IDOK != this->MessageBox("已经存在该文件后缀的记录，是否要覆盖记录?", "提示", MB_OKCANCEL)) {
			return;
		}

	
	}

	if (ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, _fileExtension, &pKey)) {
		AfxMessageBox("创建失败,是否已经使用管理员权限打开程序？");
		return;
	}

	{
		// 创建ROOT
		RegSetKeyValue(HKEY_CLASSES_ROOT, _fileExtension, "", REG_SZ,realFolder.GetBuffer(), realFolder.GetLength());
		// nt_extension.aaa
		RegCreateKey(HKEY_CLASSES_ROOT, realFolder + "\\shell", &pKey);
		RegCreateKey(HKEY_CLASSES_ROOT, realFolder + "\\shell\\open", &pKey);
		RegCreateKey(HKEY_CLASSES_ROOT, realFolder + "\\shell\\open\\command", &pKey);

		CString path = CString("\"") + _filePath + "\" \"%1\"";
		RegSetKeyValue(HKEY_CLASSES_ROOT, realFolder + "\\shell\\open\\command", "", REG_SZ,path.GetBuffer(), path.GetLength());
	}

	{
		// 创建CLASSES
		CString prefix = "Software\\Classes\\";
		RegDeleteKey(HKEY_CURRENT_USER, prefix + _fileExtension);
		RegCreateKey(HKEY_CURRENT_USER, prefix + _fileExtension, &pKey);
		RegSetKeyValue(HKEY_CURRENT_USER, prefix + _fileExtension, "", REG_SZ, realFolder.GetBuffer(), realFolder.GetLength());

		// nt_extension.aaa
		RegDeleteKey(HKEY_CURRENT_USER, prefix + realFolder + "\\shell");
		RegCreateKey(HKEY_CURRENT_USER, prefix + realFolder + "\\shell", &pKey);
		RegCreateKey(HKEY_CURRENT_USER, prefix + realFolder + "\\shell\\open", &pKey);
		RegCreateKey(HKEY_CURRENT_USER, prefix + realFolder + "\\shell\\open\\command", &pKey);

		CString path = CString("\"") + _filePath + "\" \"%1\"";
		RegSetKeyValue(HKEY_CURRENT_USER, prefix + realFolder + "\\shell\\open\\command", "", REG_SZ, path.GetBuffer(), path.GetLength());

	}
}
