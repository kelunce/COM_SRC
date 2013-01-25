
// comt4Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comt4.h"
#include "comt4Dlg.h"
#include "pngLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccomt4Dlg �Ի���




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


// Ccomt4Dlg ��Ϣ�������

BOOL Ccomt4Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Ccomt4Dlg::OnPaint()
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
        if(m_bShowPic)
        {
            // png��ʽͼƬ��֧�ְ�
            ::CoInitialize(NULL);
            CPaintDC dc(this);
            HRESULT hr;
            CFile file;
            file.Open( L"test.png", CFile::modeRead | CFile::shareDenyNone ); // �����ļ�����
            DWORD dwSize = file.GetLength();
            HGLOBAL hMem = ::GlobalAlloc( GMEM_MOVEABLE, dwSize );
            LPVOID lpBuf = ::GlobalLock( hMem );
            file.Read( lpBuf, dwSize );
            file.Close();
            ::GlobalUnlock( hMem );
            IStream * pStream = NULL;
            IPicture * pPicture = NULL;
            // �� HGLOBAL �õ� IStream������ TRUE ��ʾ�ͷ� IStream ��ͬʱ���ͷ��ڴ�
            hr = ::CreateStreamOnHGlobal( hMem, TRUE, &pStream );
            ASSERT ( SUCCEEDED(hr) );

            /* �ⲿ����ʾ������ʾbmp,jpg��ʽ
            hr = ::OleLoadPicture( pStream, dwSize, TRUE, IID_IPicture, ( LPVOID
                * )&pPicture );
            ASSERT(hr==S_OK);
            long nWidth,nHeight; // ��ߣ�MM_HIMETRIC ģʽ����λ��0.01 ����
            pPicture->get_Width( &nWidth ); // ��
            pPicture->get_Height( &nHeight ); // ��
            ////////ԭ����ʾ//////
            CSize sz( nWidth, nHeight );
            dc.HIMETRICtoDP( &sz ); // ת�� MM_HIMETRIC ģʽ��λΪ MM_TEXT ���ص�λ
            pPicture->Render(dc.m_hDC,0,0,sz.cx,sz.cy,
                0,nHeight,nWidth,-nHeight,NULL);
            ////////�����ڳߴ���ʾ////////
            // CRect rect; GetClientRect(&rect);
            // pPicture->Render(pDC->m_hDC,0,0,rect.Width(),rect.Height(),
            // 0,nHeight,nWidth,-nHeight,NULL);
            if ( pPicture ) pPicture->Release();// �ͷ� IPicture ָ��
            */

            // ��ʾpng
            CRect rect; GetClientRect(&rect);
            ImageShowFromStream(pStream,dc.m_hDC,rect.Width() - 180 ,rect.Height() - 180,180,180,0,0,180,180);

            if ( pStream ) pStream->Release(); // �ͷ� IStream ָ�룬ͬʱ�ͷ��� hMem
            ::CoUninitialize();
        }

		CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

    // com�ڲ��������ڴ�,�ѵ�ַ����lpwProgID
    hr = ::ProgIDFromCLSID(clsid, &lpwProgID);
    // Ҳ������HKEY_CLASSES_ROOT\CLSID\{000209FF-0000-0000-C000-000000000046}\ProgID�ҵ�

    if (SUCCEEDED(hr))
    {
        ::MessageBoxW(NULL, lpwProgID, L"ProgID", MB_OK);

        // �ͷ�lpwProgIDָ����ڴ�
        IMalloc *pMalloc = NULL;
        hr = CoGetMalloc(1, &pMalloc);
        if (SUCCEEDED(hr))
        {
            //��Ϊ��unicode����,����"Word.Application.12"��38���ֽ� + 2���ֽڵĽ�����=40�ֽ�
            SIZE_T uSize = pMalloc->GetSize(lpwProgID);
            pMalloc->Free(lpwProgID);
            pMalloc->Release();
        }
    }

    ::CoUninitialize();
}

void Ccomt4Dlg::OnBnClickedButton2()
{
    CString strPath = BrowseFolder(m_hWnd,L"�ļ��������");
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
        _T("C:/WINDOWS/system32/notepad.exe"), // ���±�����ע�⣬���ϵͳ�Ƿ�Ҳ�����Ŀ¼��
        _T("C:/Documents and Settings/Administrator/����/�ҵļ��±�.lnk")
        );
    // �����Ͻ�����ݷ�ʽ(lnk)�ļ���ȫ·������ע�⣬���ϵͳ�Ƿ�Ҳ�����Ŀ¼��
    // ����ó���ʵ��Ѱ�������·��������Բ�ע���
    // HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders
}

CString Ccomt4Dlg::BrowseFolder( HWND hWnd, LPCTSTR lpTitle )
{
    // ���� SHBrowseForFolder ȡ��Ŀ¼(�ļ���)����
    // ���� hWnd: �����ھ��
    // ���� lpTitle: ���ڱ���
    TCHAR szPath[MAX_PATH]={0};
    BROWSEINFO m_bi;
    m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
    m_bi.hwndOwner = hWnd;
    m_bi.pidlRoot = NULL;
    m_bi.lpszTitle = lpTitle;
    m_bi.lpfn = NULL;
    m_bi.lParam = NULL;
    m_bi.pszDisplayName = szPath;
    // �����ڲ��������ڴ�
    LPITEMIDLIST pidl = ::SHBrowseForFolder( &m_bi );
    if ( pidl )
    {
        if( !::SHGetPathFromIDList ( pidl, szPath ) ) szPath[0]=0;
        IMalloc * pMalloc = NULL;
        if ( SUCCEEDED ( ::SHGetMalloc( &pMalloc ) ) ) // ȡ��IMalloc �������ӿ�
        {
            pMalloc->Free( pidl );  // �ͷ��ڴ�
            pMalloc->Release();     // �ͷŽӿ�
        }
    }
    return szPath;
}

void Ccomt4Dlg::CreateShortcut( LPCTSTR lpszExe, LPCTSTR lpszLnk )
{
    // ������ݷ�ʽ
    // ���� lpszExe: EXE �ļ�ȫ·����
    // ���� lpszLnk: ��ݷ�ʽ�ļ�ȫ·����
    ::CoInitialize( NULL );
    IShellLink * psl = NULL;
    IPersistFile * ppf = NULL;
    HRESULT hr = ::CoCreateInstance( // �������
        CLSID_ShellLink, // ��ݷ�ʽ CLSID
        NULL, // �ۺ���(ע4)
        CLSCTX_INPROC_SERVER, // ������(Shell32.dll)����
        IID_IShellLink, // IShellLink �� IID
        (LPVOID *)&psl ); // �õ��ӿ�ָ��
    if ( SUCCEEDED(hr) )
    {
        psl->SetPath( lpszExe ); // ȫ·��������
        // psl->SetArguments(); // �����в���
        // psl->SetDescription(); // ��ע
        // psl->SetHotkey(); // ��ݼ�
        // psl->SetIconLocation(); // ͼ��
        // psl->SetShowCmd(); // ���ڳߴ�
        // ���� EXE ���ļ������õ�Ŀ¼��
        TCHAR szWorkPath[ MAX_PATH ];
        ::lstrcpy( szWorkPath, lpszExe );
        LPTSTR lp = szWorkPath;
        while( *lp ) lp++;
        while( '/' != *lp ) lp--;
        *lp=0;
        //���� EXE �����Ĭ�Ϲ���Ŀ¼
        psl->SetWorkingDirectory( szWorkPath );
        hr = psl->QueryInterface( // ���ҳ������ļ��ӿ�ָ��
            IID_IPersistFile, // �����Խӿ� IID
            (LPVOID *)&ppf ); // �õ��ӿ�ָ��
        if ( SUCCEEDED(hr) )
        {
            USES_CONVERSION; // ת��Ϊ UNICODE �ַ���
            ppf->Save( T2COLE( lpszLnk ), TRUE ); // ����
        }
    }
    if ( ppf ) ppf->Release();
    if ( psl ) psl->Release();
    ::CoUninitialize();
}
