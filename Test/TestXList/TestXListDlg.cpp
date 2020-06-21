// TestXListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestXList.h"
#include "TestXListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define   MAX_ITEMSIZE   64

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




const char szDlg[] = "TestXList";


#define   COL_STR		0
#define   COL_CASELESS 	1
#define   COL_INT        2
#define	COL_FLOAT		3
#define	COL_NUM		4
#define	COL_DATE		5
#define	COL_DOLLAR	6	
#define	NR_COLS		7
static const char *szHeadings[] = 
	{ "String", "caseless", "Int", "Float", "Number", "Date", "Dollar", NULL };


/*virtual*/ CXListCtrl::DataTypes CTestListCtrl::GetColumnType (int iCol)
{
     switch (iCol)
     {
          case COL_STR:		return TypeText;
          case COL_CASELESS:	return TypeCaselessText;
          case COL_INT:		return TypeInt;
		case COL_FLOAT:	return TypeFloat;
		case COL_DATE:		return TypeDate;
		case COL_NUM:		return TypeNumber;
		case COL_DOLLAR:	return TypeCurrency;
          default:            return TypeCaselessText;
     }
}





/////////////////////////////////////////////////////////////////////////////
// CTestXListDlg dialog

CTestXListDlg::CTestXListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestXListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestXListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestXListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestXListDlg)
	DDX_Control(pDX, IDC_LIST1, m_lc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestXListDlg, CDialog)
	//{{AFX_MSG_MAP(CTestXListDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestXListDlg message handlers

BOOL CTestXListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
     m_lc.SetExtendedStyle (LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

     m_lc.SetRegBase ("Software\\Thistle Software\\Test");
     m_lc.Initialise (szHeadings);


	typedef struct 
	{
		const char* _pszStr;
		const char* _pszCaseless;
		const char* _pszInt;
		const char* _pszFloat;
		const char* _pszNum;
		const char* _pszDate;
		const char* _pszDollar;
	}	Data;
	
	static Data arrData[] = 
	{	
		{  "aaa", "aaa", "1", "1.0", "1,000", "20 Feb 2004", "$123.00" },
		{  "abc", "abc", "2", "-1.0", "1,123", "29 Feb 2004", "-$123.00" },
		{  "ACC", "ACC", "3", "-1.1", "-1,123", "1 mar 2004", "$122.00" },
		{  "ABC", "ABC", "4", "1.3", "123,456", "5/4/2004", "-$122.00" },
		{  "DDD", "DDD", "5", "1.0e+2", "+1,000", "4/5/2004", "$0.00" },
		{  "ddd", "ddd", "1000", "1.0e-3", "-1,100", "4/4/2004", "$123" },
		{  "123", "123", "-7", "1.1e-3", "+1,100", "20 Feb 2003", "$1,300" },
		{  "_dd", "_dd", "7", "1.0", "1,000", "20 Feb 1999", "($123.00)" },
		{  "zzzzz", "zzzzz", "9", "1.0", "1,000", "26 Feb 1961", "$10.45" },
		{  "zzz", "zzz", "10","+1.0", "1,000", "20 Dec 04", "$10.44" },
		{  "zzz", "zzz", "-123","+1.1", "1,000", "19 dec 2004", "$10.00" },
		{  "zzz", "zzz", "-122","1.0", "1,000", "20 Feb 2004", "$123.00" },
		NULL
	};

	// Add children to the list box
	int iRow = 0;
	int activeIndex = -1;
	
	for (Data* pData = &arrData [0]; pData->_pszStr; pData++)
	{
          // Fill up an array of text for this row.  The order is fixed as 
          // 'Name', 'Path', 'Type', and 'Alt', ie the same as the headings.
          CStringArray arrText;
		arrText.Add (pData->_pszStr);
		arrText.Add (pData->_pszCaseless);
		arrText.Add (pData->_pszInt);
		arrText.Add (pData->_pszFloat);
		arrText.Add (pData->_pszNum);
		arrText.Add (pData->_pszDate);
		arrText.Add (pData->_pszDollar);

	     // Now insert the sub-items into the list...
     	LV_ITEM lvItem;
     
     	lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
     	lvItem.state = 0;      
     	lvItem.stateMask = 0;  
     	lvItem.iImage = 0;
     
     	lvItem.iItem = iRow;
     	lvItem.iSubItem = 0;

     	lvItem.pszText = (char*) (const char*) arrText [m_lc.GetColIndex (0)]; 
     	lvItem.cchTextMax = MAX_ITEMSIZE;
     
     	int iIndex = m_lc.InsertItem (&lvItem);
     	if ( iIndex != -1)
               for (int j = 1; j < m_lc.GetColCount(); j++)
          		m_lc.SetItemText (iIndex, j, 
          		          (char*) (const char*) arrText [m_lc.GetColIndex (j)]);
	}

    
     m_lc.Sort();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestXListDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestXListDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestXListDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
