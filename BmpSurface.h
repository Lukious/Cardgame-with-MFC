#pragma once

// 비트맵 파일을 메모리DC적재하여 blit하는 표면 클래스
// 컬러키는 투명색으로 설정되어 출력되지 않는다
class CBmpSurface : public CBitmap
{
public:	
	CBmpSurface();
	~CBmpSurface();
		
	void Open(CDC* pDC, CString file, COLORREF colorKey=0);	// 비트맵 파일 읽기
	void Blit(CDC* pDC, int x, int y);	// pDC로 Blit

	int	Width()  { return m_nWidth; }
	int	Height() { return m_nHeight; }	
	void SetColorKey(COLORREF colorKey) { m_ColorKey = colorKey; }

protected:
	void DeleteSurface();				// 메모리 DC 삭제

	CDC*		m_pMemDC;	// 비트맵이 저장된 메모리 DC
	int			m_nWidth;	// 비트맵의 크기
	int			m_nHeight;
	CBitmap*	m_pOldBmp;	// 비트맵 선택해제를 위해 사용
	COLORREF	m_ColorKey;	// 스프라이트 출력을 위한 컬러키(지정이 없으면 이미지 출력)
};
