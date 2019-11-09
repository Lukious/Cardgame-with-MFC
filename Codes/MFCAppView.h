
// MFCAppView.h: CMFCAppView 클래스의 인터페이스
//

#pragma once

#include "BmpSurface.h"

#define MAX_CARD	52

// 카드 클래스 (CBmpSurface를 상속받아 비트맵출력을 가능하게한다)
class CCard : public CBmpSurface
{
public:
	CCard():bFront(TRUE){}

	BOOL bFront;	// 카드 전면의 상태여부(출력할때 사용)
};

// CCard*를 요소로 갖는 동적배열 CArrayBmp을 형정의
typedef CArray<CCard*, CCard*> CArrayBmp;

class CMFCAppView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCAppView() noexcept;
	DECLARE_DYNCREATE(CMFCAppView)

// 특성입니다.
public:
	CMFCAppDoc* GetDocument() const;

// 작업입니다.
public:

	CArrayBmp	m_arrBmp;	// 카드의 동적배열
	CBmpSurface	m_bmpBack;	// 뒷면카드(비트맵)
	BOOL		m_bReset;
	int			m_nStep;	// 배수 step

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnReset();
	afx_msg void On1step();
	afx_msg void OnFinal();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // MFCAppView.cpp의 디버그 버전
inline CMFCAppDoc* CMFCAppView::GetDocument() const
   { return reinterpret_cast<CMFCAppDoc*>(m_pDocument); }
#endif

