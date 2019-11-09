
// MFCAppView.cpp: CMFCAppView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApp.h"
#endif

#include "MFCAppDoc.h"
#include "MFCAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCAppView

IMPLEMENT_DYNCREATE(CMFCAppView, CView)

BEGIN_MESSAGE_MAP(CMFCAppView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_RESET, &CMFCAppView::OnReset)
	ON_COMMAND(ID_1STEP, &CMFCAppView::On1step)
	ON_COMMAND(ID_FINAL, &CMFCAppView::OnFinal)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMFCAppView 생성/소멸

CMFCAppView::CMFCAppView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCAppView::~CMFCAppView()
{
}

BOOL CMFCAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCAppView 그리기

void CMFCAppView::OnDraw(CDC* pDC)
{
	CMFCAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!m_bReset) return;

	int col_count = 10;
	int row_count = 6;
	int x, y;
	for (int iy = 0, i = 0; iy < row_count; iy++)
	{
		for (int ix = 0; ix < col_count; ix++, i++)
		{
			if (i < m_arrBmp.GetCount())
			{
				x = ix * m_arrBmp[i]->Width();
				y = iy * m_arrBmp[i]->Height();

				if (m_arrBmp[i]->bFront)
					m_arrBmp[i]->Blit(pDC, x, y);
				else
					m_bmpBack.Blit(pDC, x, y);					
			}
			else
			{
				break;
				iy = row_count;
			}
		}
	}
}


// CMFCAppView 인쇄

BOOL CMFCAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCAppView 진단

#ifdef _DEBUG
void CMFCAppView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCAppDoc* CMFCAppView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCAppDoc)));
	return (CMFCAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCAppView 메시지 처리기


void CMFCAppView::OnReset()
{
	m_bReset = TRUE;
	m_nStep = 1;

	// 모든 카드상태 초기화
	for (int i = 0; i < m_arrBmp.GetCount(); i++)
	{
		m_arrBmp[i]->bFront = TRUE;
	}
	// 화면갱신 (WM_PAINT->OnDraw)
	Invalidate();
}


void CMFCAppView::On1step()
{
	BOOL multiple_state;
	
	m_nStep++;
	for (int i = 0; i < m_arrBmp.GetCount(); i++)
	{
		// 배수 상태를 구한다
		multiple_state = ((i + 1) % m_nStep) == 0 ? TRUE : FALSE;
		// i+1이 해당 배수이면 bFront를 flip
		if (multiple_state) m_arrBmp[i]->bFront = !m_arrBmp[i]->bFront;
	}
	Invalidate();
}


void CMFCAppView::OnFinal()
{
	BOOL multiple_state;
	// 모든 배수에 맞게 카드의 bFront 상태를 갱신한다
	for (; m_nStep < m_arrBmp.GetCount();)
	{
		m_nStep++;
		for (int i = 0; i < m_arrBmp.GetCount(); i++)
		{
			multiple_state = (i + 1) % m_nStep == 0 ? TRUE : FALSE;
			if (multiple_state) m_arrBmp[i]->bFront = !m_arrBmp[i]->bFront;
		}
	}
	Invalidate();
}


int CMFCAppView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CString strFile;
	CDC* pDC = GetDC();
	// 모든 카드를 bitmap으로 load하여 CCard 객체를 생성
	for (int i = 0; i < MAX_CARD; i++)
	{
		CCard* bmpCard = new CCard;
		strFile.Format(L".\\cards\\%02d.bmp", i + 1); // 경로를 포함한 파일이름
		bmpCard->Open(pDC, strFile);
		m_arrBmp.Add(bmpCard);		// 동적배열에 추가
	}	
	// 뒷면카드 load
	m_bmpBack.Open(pDC, L".\\cards\\53.bmp");
	ReleaseDC(pDC);

	// 초기화
	m_bReset = FALSE;
	m_nStep = 1;

	return 0;
}


void CMFCAppView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// m_arrBmp의 모든 요소를 제거
	for (int i = 0; i < m_arrBmp.GetCount(); i++)
	{
		delete m_arrBmp[i]; // CCard 객체 메모리해제
	}
	// 동적배열을 해제
	m_arrBmp.RemoveAll();
}
