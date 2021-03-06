
// TCPServerDlg.h: 头文件
//

#pragma once
UINT Server_Th(LPVOID p);

// CTCPServerDlg 对话框
class CTCPServerDlg : public CDialogEx
{
// 构造
public:
	CTCPServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCPSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	CEdit * show_edit;
	CEdit * send_edit;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	SOCKET m_sockServer;
	SOCKET m_sockClient;
	SOCKADDR_IN m_addrServer;
	SOCKADDR_IN m_addrClient;
	CString m_strData;
	void  Update(CString str);
	//CEdit send_edit;
	//CEdit show_edit;
	afx_msg void OnBnClickedButton1();
};
