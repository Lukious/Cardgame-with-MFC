#include "stdafx.h"
#include "BmpSurface.h"

///////////////////////////////////////////////////////////////////

CBmpSurface::CBmpSurface()
{
	m_pMemDC = nullptr;
	m_ColorKey = 0;
}

CBmpSurface::~CBmpSurface()
{
	DeleteSurface();
}

void CBmpSurface::DeleteSurface()
{
	if (m_pMemDC)
	{
		m_pMemDC->SelectObject(m_pOldBmp);	// 비트맵 선택해제
		DeleteObject();						// 비트맵 삭제
		m_pMemDC->DeleteDC();				// 메모리 dc 삭제
		delete m_pMemDC;
		m_pMemDC = nullptr;
	}
}

void CBmpSurface::Open(CDC* pDC, CString file, COLORREF colorKey)
{
	// 비트맵파일을 읽는다
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap) Attach(hBitmap);

	// 메모리 dc를 생성하고 메모리 dc에 비트맵을 올린다
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pOldBmp = (CBitmap*)m_pMemDC->SelectObject(this);

	BITMAP bmpInfo;
	GetBitmap(&bmpInfo);
	m_nWidth = bmpInfo.bmWidth;
	m_nHeight = bmpInfo.bmHeight;

	m_ColorKey = colorKey;
}

void CBmpSurface::Blit(CDC* pDC, int x, int y)
{
	// 컬러키 지정이 있으면 스프라이트 출력
	if (m_ColorKey)
	{
		pDC->TransparentBlt(x, y, m_nWidth, m_nHeight, m_pMemDC, 0, 0, m_nWidth, m_nHeight, m_ColorKey);
	}
	else // 컬러키 지정이 없으면 이미지 출력
	{
		pDC->BitBlt(x, y, m_nWidth, m_nHeight, m_pMemDC, 0, 0, SRCCOPY);								
	}
}
