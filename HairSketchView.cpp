
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
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_DYEING_COLOR, &CHairSketchView::OnDyeingColor)
ON_COMMAND(ID_COLOR_BLACK, &CHairSketchView::OnColorBlack)
ON_COMMAND(ID_COLOR_BROWN, &CHairSketchView::OnColorBrown)
ON_COMMAND(ID_COLOR_BRIGHTBROWN, &CHairSketchView::OnColorBrightbrown)
ON_COMMAND(ID_COLOR_DARKBROWN, &CHairSketchView::OnColorDarkbrown)
ON_COMMAND(ID_COLOR_DARKGRAY, &CHairSketchView::OnColorDarkgray)
ON_COMMAND(ID_COLOR_GRAY, &CHairSketchView::OnColorGray)
ON_COMMAND(ID_COLOR_BRIGHTGRAY, &CHairSketchView::OnColorBrightgray)
ON_COMMAND(ID_EDIT_COPY, &CHairSketchView::OnEditCopy)
ON_COMMAND(ID_EDIT_PASTE, &CHairSketchView::OnEditPaste)
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CHairSketchView::OnUpdateEditPaste)
ON_COMMAND(ID_COLOR_BLUE, &CHairSketchView::OnColorBlue)
ON_COMMAND(ID_COLOR_SKY, &CHairSketchView::OnColorSky)
ON_COMMAND(ID_COLOR_BLUEGREEN, &CHairSketchView::OnColorBluegreen)
ON_COMMAND(ID_COLORJAJURED, &CHairSketchView::OnColorjajured)
ON_COMMAND(ID_COLOR_BORARED, &CHairSketchView::OnColorBorared)
ON_COMMAND(ID_COLOR_RED, &CHairSketchView::OnColorRed)
ON_COMMAND(ID_COLOR_BRIGHTRED, &CHairSketchView::OnColorBrightred)
END_MESSAGE_MAP()

// CHairSketchView 생성/소멸

CHairSketchView::CHairSketchView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	bmpInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	for (int i = 0; i < 256; i++)
	{
		bmpInfo->bmiColors[i].rgbRed = bmpInfo->bmiColors[i].rgbGreen = bmpInfo->bmiColors[i].rgbBlue = i;
		bmpInfo->bmiColors[i].rgbReserved = 0;
	}
	x = 0; y = 0;
	r = 0; g = 0; b = 0;
}

CHairSketchView::~CHairSketchView()
{
	if (bmpInfo)
		delete bmpInfo;
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
	CHairSketchDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	height = doc->dibHi.biHeight;
	width = doc->dibHi.biWidth;
	resize = WIDTHBYTES(doc->dibHi.biBitCount*doc->dibHi.biWidth);
	bmpInfo->bmiHeader = doc->dibHi;
	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, doc->m_InImg, bmpInfo, DIB_RGB_COLORS);
}

void CHairSketchView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: 이 뷰의 전체 크기를 계산합니다.
	CHairSketchDoc* doc = GetDocument();
	CSize sizeTotal;
	sizeTotal.cx = doc->width;
	sizeTotal.cy = doc->height;
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


void CHairSketchView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CHairSketchDoc* doc = GetDocument();
	x = point.x;
	y = point.y;
	doc->setXY(x, y);
	r = doc->getClickedR(x, y);
	g = doc->getClickedG(x, y);
	b = doc->getClickedB(x, y);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CHairSketchView::OnDyeingColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->setR(r);
	doc->setG(g);
	doc->setB(b);
	doc->OnDyeingColor();
	doc->CopyClipboard(doc->m_OutImg, height, width, 24);
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CHairSketchView::OnColorBlack()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(25, 25, 25);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorDarkbrown()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(53, 0, 0);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBrown()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(64, 32, 32);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBrightbrown()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(74, 37, 0);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorDarkgray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(47, 47, 47);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(63, 63, 63);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBrightgray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(80, 80, 80);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBlue()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(0, 0, 64);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorSky()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(0, 53, 106);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBluegreen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(32, 64, 64);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorjajured()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(53, 0, 27);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBorared()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(70, 0, 35);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorRed()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(60, 0, 0);
	OnDyeingColor();
	doc->quick = false;
}


void CHairSketchView::OnColorBrightred()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->quick = true;
	doc->quickColor = RGB(80, 0, 0);
	OnDyeingColor();
	doc->quick = false;
}

void CHairSketchView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CHairSketchDoc* doc = GetDocument();
	doc->CopyClipboard(doc->m_InImg, height, width, doc->dibHi.biBitCount);
}


void CHairSketchView::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_NEW);
}


void CHairSketchView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(IsClipboardFormatAvailable(CF_DIB));
}