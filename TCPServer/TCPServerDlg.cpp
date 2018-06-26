
// TCPServerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TCPServer.h"
#include "TCPServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CWinThread* pRecvThread = NULL;
//UINT RecvThread(LPVOID pParam);

SOCKET listen_sock;
SOCKET sock;

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


// CTCPServerDlg 对话框



CTCPServerDlg::CTCPServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCPSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT1, send_edit);
	//DDX_Control(pDX, IDC_EDIT2, show_edit);
}

BEGIN_MESSAGE_MAP(CTCPServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CTCPServerDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTCPServerDlg 消息处理程序

BOOL CTCPServerDlg::OnInitDialog()
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

	AfxBeginThread(&Server_Th, 0);
	show_edit = (CEdit *)GetDlgItem(IDC_EDIT2);
	send_edit = (CEdit *)GetDlgItem(IDC_EDIT1);
	send_edit->SetFocus();

	char name[80];
	CString IP;
	hostent * pHost;
	gethostname(name, 128);//获得主机名  
	pHost = gethostbyname(name);//获得主机结构  
	IP = inet_ntoa(*(in_addr *)pHost->h_addr);
	Update("本机IP地址：" + IP);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPServerDlg::OnPaint()
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
HCURSOR CTCPServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTCPServerDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
void CTCPServerDlg::Update(CString str)
{
	show_edit->ReplaceSel(str + "\r\n");
}

//UINT RecvThread(LPVOID pParam)
//{
//	CTCPServerDlg* pThis = (CTCPServerDlg*)pParam;
//	SOCKET sock;
//	while (1)
//	{
//		SOCKADDR_IN m_addrClient;
//		int len = sizeof(SOCKADDR);
//		sock = accept(pThis->m_sockServer, (SOCKADDR*)&m_addrClient, &len);
//		pThis->m_addrClient = sock;
//
//		char buff[4096] = "";
//		int ret = 0;
//		while (sock != NULL)
//		{
//			ret = recv(sock, buff, 4096, 0);
//			if (ret == 0 || ret == SOCKET_ERROR)
//			{
//				closesocket(sock);
//				break;
//			}
//			pThis->m_strData += buff;
//			pThis->GetDlgItem(IDC_EDIT2)->SetWindowText(pThis->m_strData);
//		}
//	}
//	return 0;
//}
UINT Server_Th(LPVOID p)
{
	WSADATA wsaData;

	WORD wVersion;

	wVersion = MAKEWORD(2, 2);

	WSAStartup(wVersion, &wsaData);

	SOCKADDR_IN local_addr;
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	int res;
	char msg[2048];
	CTCPServerDlg * dlg = (CTCPServerDlg *)AfxGetApp()->GetMainWnd();
	//CTCPServerDlg* pThis = (CTCPServerDlg*)pParam;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(9000);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		dlg->Update("创建监听失败");
	}
	if (bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)))
	{
		dlg->Update("绑定错误");
	}
	listen(listen_sock, 1);
	if ((sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	{
		dlg->Update("accept 失败");
	}
	else
	{
		CString port;
		port.Format("%d", int(ntohs(client_addr.sin_port)));
		dlg->Update("已连接来自：" + CString(inet_ntoa(client_addr.sin_addr)) + "  端口：" + port);
	}

	//接收数据  
	while (1)
	{
		if ((res = recv(sock, msg, 1024, 0)) == -1)
		{
			dlg->Update("失去连接");
			break;
		}
		else
		{
			msg[res] = '\0';
			dlg->Update("client:" + CString(msg));
		}
	}
	return 0;
}

void CTCPServerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	char * msg;
	send_edit->GetWindowText(str);
	msg = str.GetBuffer(str.GetLength());

	if (send(sock, msg, strlen(msg), 0) == SOCKET_ERROR)
	{
		show_edit->ReplaceSel("发送失败\r\n");
	}

	else if (str == "")
	{
		MessageBox("请输入信息!");
	}
	else
	{
		show_edit->ReplaceSel("server:" + str + "\r\n");//消息上屏，清空输入，并重获焦点
		send_edit->SetWindowText("");
		send_edit->SetFocus();
	}
}
