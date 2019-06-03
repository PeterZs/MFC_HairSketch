
// HairSketchView.cpp: CHairSketchView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HairSketch.h"
#endif

#include "HairSketchDoc.h"
#include "HairSketchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHairSketchView

IMPLEMENT_DYNCREATE(CHairSketchView, CScrollView)

BEGIN_MESSAGE_MAP(CHairSketchView, CScrollView)
END_MESSAGE_MAP()

// CHairSketchView 생성/소멸

CHairSketchView::CHairSketchView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}
	x = 0; y = 0; r = 0; g = 0; b = 0;
}

CHairSketchView::~CHairSketchView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CHairSketchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CHairSketchView 그리기

void CHairSketchView::OnDraw(CDC* pDC)
{
	CHairSketchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	height = pDoc->dibHi.biHeight;
	width = pDoc->dibHi.biWidth;
	rwsize = WIDTHBYTES(pDoc->dibHi.biBitCount*pDoc->dibHi.biWidth);
	BmInfo->bmiHeader = pDoc->dibHi;

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InImg, BmInfo, DIB_RGB_COLORS);
}

void CHairSketchView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	CHairSketchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	sizeTotal.cx = pDoc->width;
	sizeTotal.cy = pDoc->height;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CHairSketchView 진단

#ifdef _DEBUG
void CHairSketchView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHairSketchView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CHairSketchDoc* CHairSketchView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHairSketchDoc)));
	return (CHairSketchDoc*)m_pDocument;
}
#endif //_DEBUG


// CHairSketchView 메시지 처리기
