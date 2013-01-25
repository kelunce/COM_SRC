/*
//http://blog.sina.com.cn/s/blog_5396c96c0100ul6h.html
其实在windows注册表里是可以查到windows自带解码器的蛛丝马迹的，打开注册表编辑器，查找
HKEY_CLASSES_ROOT\MIME\Database\Content Type\image/png，可以发现一个Image Filter CLSID的键，
指向的{A3CCEDF7-2DE2-11D0-86F4-00A0C913F750}就是解码器的GUID，
同理在image/jpg/gif/bmp下也可以查到相应的键值，指向不同的解码器，
现在我们就是要用微软提供的解码器来读出我们需要的png文件。
这个例子只是实现了png的解码，本方法还可以解码gif, bmp, jpg，只要指定相应的CLSID就可以了。
其中解码gif还要另外实现IDirectDrawPalette接口。
*/
#pragma once

#include <ddraw.h>
#include <ocmm.h>
#include <atlbase.h>

#define DEFINE_DD_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID name \
    = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

// 解码器id
DEFINE_DD_GUID(CLSID_CoPNGFilter,    0xA3CCEDF7,0x2DE2,0x11D0,0x86,0xF4,0x00,0xA0,0xC9,0x13,0xF7,0x50);

// 虽然ddraw.h里有这个guid，但还是得自己定义，只为一个guid却多链接一个ddraw.dll进来，实在很不划算。
DEFINE_DD_GUID(IID_IDirectDrawSurfacex, 0x6C14DB81,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60 );
                
// DIB对象,在内存中生成一张位图
struct DIB
{
    int width;
    int height;
    int bpp;        // BitsPixel
    int wb;         // Width Bytes, bytes in each scan line
    void* bits;
    HBITMAP bmp;
};

void dib_create(DIB& dib, int w, int h, int bpp, RGBQUAD* ppal, int npal)
{
    struct
    {
        BITMAPINFO bi;
        RGBQUAD rgb[256];
    } head = { sizeof(BITMAPINFOHEADER), w, h, 1, bpp };
    head.bi.bmiHeader.biClrUsed = npal;
    if (ppal && bpp <= 8)
        memcpy (head.bi.bmiColors, ppal, 256 * sizeof(RGBQUAD));
    void* pData = 0;
    HBITMAP hBmp = ::CreateDIBSection(0, &head.bi, DIB_RGB_COLORS, &pData, 0, 0);
    dib.bmp = hBmp;
    dib.bits = pData;
    dib.width = w;
    dib.height = h;
    dib.bpp = bpp;
    #define DWORD_WBYTES(x)          ((((x)+31UL)>>5)<<2)
    dib.wb = DWORD_WBYTES(w * bpp);
}


// 用image filter接口加载图片需要用户提供IDirectSurface接口，我自己来实现这个接口：
class CDecode
    : public IImageDecodeEventSink
    , public IDirectDrawSurface
{
public:
    DIB m_dib;
public:
    CDecode ()
    {
    }
    //
    STDMETHOD ( QueryInterface ) ( REFIID iid, void ** ppvObject ) {  
        if ( IID_IImageDecodeEventSink == iid ) { *ppvObject = (IImageDecodeEventSink*)this; return S_OK; }
        if ( IID_IDirectDrawSurfacex == iid ) { *ppvObject = (IDirectDrawSurface*)this; return S_OK; }
        return E_NOINTERFACE;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        return 1;
    }

    virtual ULONG STDMETHODCALLTYPE Release()
    {
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////////
    // IImageDecodeEventSink
    virtual HRESULT STDMETHODCALLTYPE GetSurface (
        LONG nWidth,
        LONG nHeight,
        REFGUID bfid,
        ULONG nPasses,
        DWORD dwHints,
        IUnknown __RPC_FAR *__RPC_FAR * ppSurface )
    {
        int nBits = 0;

        if (!ppSurface)
            return E_POINTER;
        if (bfid == BFID_RGB_8 || bfid == BFID_GRAY_8)
            nBits = 8;
        else if (bfid == BFID_RGB_555 || bfid == BFID_RGB_565 || bfid == BFID_GRAY_16 )
            nBits = 16;
        else if (bfid == BFID_RGB_24)
            nBits = 24;
        else if (bfid == BFID_RGB_32 || bfid == BFID_RGBA_32)
            nBits = 32;
        else if (bfid == BFID_RGB_4)
            nBits = 4;
        else if (bfid == BFID_MONOCHROME)
            nBits = 1;
        else
            return E_NOINTERFACE;
        dib_create(m_dib, nWidth, nHeight, nBits, 0, 0);
        *ppSurface = (IDirectDrawSurface*)this;
        return S_OK;
    }

    virtual HRESULT __stdcall OnBeginDecode (
        unsigned long * pdwEvents,
        unsigned long * pnFormats,
        GUID** ppFormats )
    {
        if (!pdwEvents || !pnFormats || !ppFormats)
            return E_POINTER;
        *ppFormats = (GUID*)CoTaskMemAlloc (10 * sizeof(GUID));
        (*ppFormats)[0] = BFID_RGBA_32;
        (*ppFormats)[1] = BFID_RGB_24;
        (*ppFormats)[2] = BFID_RGB_32;
        (*ppFormats)[3] = BFID_RGB_8;
        (*ppFormats)[4] = BFID_RGB_555;
        (*ppFormats)[5] = BFID_RGB_565;
        (*ppFormats)[6] = BFID_MONOCHROME;
        (*ppFormats)[7] = BFID_RGB_4;
        (*ppFormats)[8] = BFID_GRAY_8;
        (*ppFormats)[9] = BFID_GRAY_16;
        *pdwEvents = 0;//IMGDECODE_EVENT_PALETTE|IMGDECODE_EVENT_BITSCOMPLETE | IMGDECODE_EVENT_USEDDRAW;
        *pnFormats = 10;
        return S_OK;
    }

    virtual HRESULT __stdcall OnBitsComplete () { return S_OK; }

    virtual HRESULT __stdcall OnDecodeComplete (HRESULT hrStatus)
    {
        if (m_dib.bpp!=32)
            return S_OK;
        // windows规定要把alpha值运算到RGB通道上，参看MSDN的alphablend函数说明
        // 这里假设p[3]为alpha通道，也有可能是p[0]，我给忘了，需要的自己做下实验。
        for (int i = 0; i < m_dib.width; ++i)
        {
            for (int j = 0; j < m_dib.height; ++j)
            {
                BYTE* p = dib_pixelpos(m_dib, i, j);
                p[0] = p[0] * p[3] / 256;
                p[1] = p[1] * p[3] / 256;
                p[2] = p[2] * p[3] / 256;
            }
        }
        return S_OK;
    }

    virtual HRESULT __stdcall OnPalette () { return S_OK; }
    virtual HRESULT STDMETHODCALLTYPE OnProgress (RECT * pBounds, BOOL bComplete)
    {
        return S_OK;
    }
    ////////////////////////////////////////////////////////////////////////////
    // IDirectDrawSurface
    STDMETHOD(AddAttachedSurface)(THIS_ LPDIRECTDRAWSURFACE) { return S_OK; }
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT) { return S_OK; }
    STDMETHOD(Blt)(THIS_ LPRECT,LPDIRECTDRAWSURFACE, LPRECT,DWORD, LPDDBLTFX) { return S_OK; }
    STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH, DWORD, DWORD ) { return S_OK; }
    STDMETHOD(BltFast)(THIS_ DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD) { return S_OK; }
    STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD,LPDIRECTDRAWSURFACE) { return S_OK; }
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID,LPDDENUMSURFACESCALLBACK) { return S_OK; }
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD,LPVOID,LPDDENUMSURFACESCALLBACK) { return S_OK; }
    STDMETHOD(Flip)(THIS_ LPDIRECTDRAWSURFACE, DWORD) { return S_OK; }
    STDMETHOD(GetAttachedSurface)(THIS_ LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *) { return S_OK; }
    STDMETHOD(GetBltStatus)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(GetCaps)(THIS_ LPDDSCAPS) { return E_NOTIMPL; }
    STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER FAR*) { return E_NOTIMPL; }
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY) { return E_NOTIMPL; }
    STDMETHOD(GetDC)(THIS_ HDC FAR *) { return E_NOTIMPL; }
    STDMETHOD(GetFlipStatus)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG ) { return E_NOTIMPL; }
    STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE FAR* p)
    {
        return E_NOTIMPL;
    }
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT) { return E_NOTIMPL; }
    STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC) { return E_NOTIMPL; }
    STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW, LPDDSURFACEDESC) { return S_OK; }
    STDMETHOD(IsLost)(THIS) { return S_OK; }
    STDMETHOD(ReleaseDC)(THIS_ HDC) { return S_OK; }
    STDMETHOD(Restore)(THIS) { return S_OK; }
    STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER) { return E_NOTIMPL; }

    STDMETHOD(SetColorKey)(DWORD dwFlags, LPDDCOLORKEY pDDColorKey)
    {
        return S_OK;
    }

    STDMETHOD(SetOverlayPosition)(THIS_ LONG, LONG ) { return E_NOTIMPL; }
    STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE) { return E_NOTIMPL; }
    STDMETHOD(Unlock)(THIS_ LPVOID) { return S_OK; }
    STDMETHOD(UpdateOverlay)(THIS_ LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX) { return E_NOTIMPL; }
    STDMETHOD(UpdateOverlayDisplay)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD, LPDIRECTDRAWSURFACE) { return E_NOTIMPL; }

    BYTE* dib_pixelpos(DIB& dib, int x, int y)
    {
        assert(x < dib.width && y < dib.height);
        return (BYTE*)dib.bits + (dib.height - y - 1) * dib.wb + x * dib.bpp / 8;
    }
    STDMETHOD(Lock)(THIS_ LPRECT prc,LPDDSURFACEDESC ddsd,DWORD dwFlag,HANDLE)
    {
        assert (prc->left >= 0 && prc->top >= 0);
        assert (prc->right <= m_dib.width && prc->bottom <= m_dib.height);
        ddsd->lpSurface = dib_pixelpos(m_dib, prc->left, prc->bottom - 1);
        ddsd->lPitch = -m_dib.wb;
        return S_OK;
    }

    // 读取文件
    bool LoadFile(const CLSID &guid, IStream* stream)
    {
        CComQIPtr<IImageDecodeFilter> ptr;
        ptr.CoCreateInstance(guid);
        if (ptr)
        {
            if (S_OK == ptr->Initialize(this))
            {
                ptr->Process(stream);
                ptr->Terminate(S_OK);
                return true;
            }
        }
        return false;
    }
};


//最终加载png的方法为：
void ImageShowFromStream( IStream* stream, HDC hdc, int xDst, int yDst, int wDst, int hDst, int xSrc, int ySrc, int wSrc, int hSrc )
{
    CDecode dec;
    if (dec.LoadFile(CLSID_CoPNGFilter, stream) && dec.m_dib.bmp)
    {
        HDC hCDC = CreateCompatibleDC(hdc);
        SelectObject(hCDC, dec.m_dib.bmp);
        BLENDFUNCTION bf;
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = 255;
        bf.AlphaFormat = (dec.m_dib.bpp == 32) ? 1 : 0;// AC_SRC_NO_PREMULT_ALPHA;
        AlphaBlend(hdc, xDst, yDst, wDst, hDst, hCDC, xSrc, ySrc, wSrc, hSrc, bf);
        DeleteDC(hCDC);
    }
}

