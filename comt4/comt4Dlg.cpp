
// comt4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "comt4.h"
#include "comt4Dlg.h"
#include "pngLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccomt4Dlg 对话框




Ccomt4Dlg::Ccomt4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ccomt4Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bShowPic = FALSE;
}

void Ccomt4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ccomt4Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, &Ccomt4Dlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &Ccomt4Dlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &Ccomt4Dlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &Ccomt4Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Ccomt4Dlg 消息处理程序

BOOL Ccomt4Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ccomt4Dlg::OnPaint()
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
        if(m_bShowPic)
        {
            // png格式图片不支持啊
            ::CoInitialize(NULL);
            CPaintDC dc(this);
            HRESULT hr;
            CFile file;
            file.Open( L"test.png", CFile::modeRead | CFile::shareDenyNone ); // 读入文件内容
            DWORD dwSize = file.GetLength();
            HGLOBAL hMem = ::GlobalAlloc( GMEM_MOVEABLE, dwSize );
            LPVOID lpBuf = ::GlobalLock( hMem );
            file.Read( lpBuf, dwSize );
            file.Close();
            ::GlobalUnlock( hMem );
            IStream * pStream = NULL;
            IPicture * pPicture = NULL;
            // 由 HGLOBAL 得到 IStream，参数 TRUE 表示释放 IStream 的同时，释放内存
            hr = ::CreateStreamOnHGlobal( hMem, TRUE, &pStream );
            ASSERT ( SUCCEEDED(hr) );

            /* 这部分显示可以显示bmp,jpg格式
            hr = ::OleLoadPicture( pStream, dwSize, TRUE, IID_IPicture, ( LPVOID
                * )&pPicture );
            ASSERT(hr==S_OK);
            long nWidth,nHeight; // 宽高，MM_HIMETRIC 模式，单位是0.01 毫米
            pPicture->get_Width( &nWidth ); // 宽
            pPicture->get_Height( &nHeight ); // 高
            ////////原大显示//////
            CSize sz( nWidth, nHeight );
            dc.HIMETRICtoDP( &sz ); // 转换 MM_HIMETRIC 模式单位为 MM_TEXT 像素单位
            pPicture->Render(dc.m_hDC,0,0,sz.cx,sz.cy,
                0,nHeight,nWidth,-nHeight,NULL);
            ////////按窗口尺寸显示////////
            // CRect rect; GetClientRect(&rect);
            // pPicture->Render(pDC->m_hDC,0,0,rect.Width(),rect.Height(),
            // 0,nHeight,nWidth,-nHeight,NULL);
            if ( pPicture ) pPicture->Release();// 释放 IPicture 指针
            */

            // 显示png
            CRect rect; GetClientRect(&rect);
            ImageShowFromStream(pStream,dc.m_hDC,rect.Width() - 180 ,rect.Height() - 180,180,180,0,0,180,180);

            if ( pStream ) pStream->Release(); // 释放 IStream 指针，同时释放了 hMem
            ::CoUninitialize();
        }

		CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ccomt4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Ccomt4Dlg::OnBnClickedButton1()
{
    ::CoInitialize(NULL);

    HRESULT hr;
    
    // {000209FF-0000-0000-C000-000000000046} = Word.Application.12
    CLSID clsid =  { 0x000209FF, 0x0, 0x0, { 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46 } };
    LPOLESTR lpwProgID = NULL;

    // com内部申请了内存,把地址放在lpwProgID
    hr = ::ProgIDFromCLSID(clsid, &lpwProgID);
    // 也可以在HKEY_CLASSES_ROOT\CLSID\{000209FF-0000-0000-C000-000000000046}\ProgID找到

    if (SUCCEEDED(hr))
    {
        ::MessageBoxW(NULL, lpwProgID, L"ProgID", MB_OK);

        // 释放lpwProgID指向的内存
        IMalloc *pMalloc = NULL;
        hr = CoGetMalloc(1, &pMalloc);
        if (SUCCEEDED(hr))
        {
            //因为是unicode编码,所以"Word.Application.12"是38个字节 + 2个字节的结束符=40字节
            SIZE_T uSize = pMalloc->GetSize(lpwProgID);
            pMalloc->Free(lpwProgID);
            pMalloc->Release();
        }
    }

    ::CoUninitialize();
}

void Ccomt4Dlg::OnBnClickedButton2()
{
    CString strPath = BrowseFolder(m_hWnd,L"文件夹浏览器");
    ::MessageBoxW(NULL, strPath, L"path", MB_OK);
}

void Ccomt4Dlg::OnBnClickedButton3()
{
    m_bShowPic = m_bShowPic==TRUE?FALSE:TRUE;
    Invalidate();
}

void Ccomt4Dlg::OnBnClickedButton4()
{
    CreateShortcut(
        _T("C:/WINDOWS/system32/notepad.exe"), // 记事本程序。注意，你的系统是否也是这个目录？
        _T("C:/Documents and Settings/Administrator/桌面/我的记事本.lnk")
        );
    // 桌面上建立快捷方式(lnk)文件的全路径名。注意，你的系统是否也是这个目录？
    // 如果用程序实现寻找桌面的路径，则可以查注册表
    // HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
}

CString Ccomt4Dlg::BrowseFolder( HWND hWnd, LPCTSTR lpTitle )
{
    // 调用 SHBrowseForFolder 取得目录(文件夹)名称
    // 参数 hWnd: 父窗口句柄
    // 参数 lpTitle: 窗口标题
    TCHAR szPath[MAX_PATH]={0};
    BROWSEINFO m_bi;
    m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    m_bi.hwndOwner = hWnd;
    m_bi.pidlRoot = NULL;
    m_bi.lpszTitle = lpTitle;
    m_bi.lpfn = NULL;
    m_bi.lParam = NULL;
    m_bi.pszDisplayName = szPath;
    // 这里内部申请了内存
    LPITEMIDLIST pidl = ::SHBrowseForFolder( &m_bi );
    if ( pidl )
    {
        if( !::SHGetPathFromIDList ( pidl, szPath ) ) szPath[0]=0;
        IMalloc * pMalloc = NULL;
        if ( SUCCEEDED ( ::SHGetMalloc( &pMalloc ) ) ) // 取得IMalloc 分配器接口
        {
            pMalloc->Free( pidl );  // 释放内存
            pMalloc->Release();     // 释放接口
        }
    }
    return szPath;
}

void Ccomt4Dlg::CreateShortcut( LPCTSTR lpszExe, LPCTSTR lpszLnk )
{
    // 建立块捷方式
    // 参数 lpszExe: EXE 文件全路径名
    // 参数 lpszLnk: 快捷方式文件全路径名
    ::CoInitialize( NULL );
    IShellLink * psl = NULL;
    IPersistFile * ppf = NULL;
    HRESULT hr = ::CoCreateInstance( // 启动组件
        CLSID_ShellLink, // 快捷方式 CLSID
        NULL, // 聚合用(注4)
        CLSCTX_INPROC_SERVER, // 进程内(Shell32.dll)服务
        IID_IShellLink, // IShellLink 的 IID
        (LPVOID *)&psl ); // 得到接口指针
    if ( SUCCEEDED(hr) )
    {
        psl->SetPath( lpszExe ); // 全路径程序名
        // psl->SetArguments(); // 命令行参数
        // psl->SetDescription(); // 备注
        // psl->SetHotkey(); // 快捷键
        // psl->SetIconLocation(); // 图标
        // psl->SetShowCmd(); // 窗口尺寸
        // 根据 EXE 的文件名，得到目录名
        TCHAR szWorkPath[ MAX_PATH ];
        ::lstrcpy( szWorkPath, lpszExe );
        LPTSTR lp = szWorkPath;
        while( *lp ) lp++;
        while( '/' != *lp ) lp--;
        *lp=0;
        //设置 EXE 程序的默认工作目录
        psl->SetWorkingDirectory( szWorkPath );
        hr = psl->QueryInterface( // 查找持续性文件接口指针
            IID_IPersistFile, // 持续性接口 IID
            (LPVOID *)&ppf ); // 得到接口指针
        if ( SUCCEEDED(hr) )
        {
            USES_CONVERSION; // 转换为 UNICODE 字符串
            ppf->Save( T2COLE( lpszLnk ), TRUE ); // 保存
        }
    }
    if ( ppf ) ppf->Release();
    if ( psl ) psl->Release();
    ::CoUninitialize();
}
