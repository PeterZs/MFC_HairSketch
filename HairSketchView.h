
// HairSketchView.h: CHairSketchView 클래스의 인터페이스
//

#pragma once


class CHairSketchView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CHairSketchView() noexcept;
	DECLARE_DYNCREATE(CHairSketchView)

// 특성입니다.
public:
	CHairSketchDoc* GetDocument() const;

// 작업입니다.
public:
	BITMAPINFO *BmInfo;
	int height;
	int width;
	int rwsize;
	int x, y, r, g, b;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CHairSketchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HairSketchView.cpp의 디버그 버전
inline CHairSketchDoc* CHairSketchView::GetDocument() const
   { return reinterpret_cast<CHairSketchDoc*>(m_pDocument); }
#endif

