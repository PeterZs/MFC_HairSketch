
// HairSketchDoc.h: CHairSketchDoc 클래스의 인터페이스
//


#pragma once
#define WIDTHBYTES(bits)	(((bits)+31)/32*4)

class CHairSketchDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CHairSketchDoc() noexcept;
	DECLARE_DYNCREATE(CHairSketchDoc)

// 특성입니다.
public:

// 작업입니다.
public:
	BITMAPFILEHEADER dibHf;
	BITMAPINFOHEADER dibHi;
	RGBQUAD palRGB[256];
	unsigned char *m_InImg;
	unsigned char *m_OutImg;
	int height;
	int width;
	int ImgSize;
	unsigned char *m_flag;
	int stk[16777216];
	int sp;
	int set_rv, set_gv, set_bv, set_x, set_y;

	int get_Rvalue(int x, int y);
	int get_Gvalue(int x, int y);
	int get_Bvalue(int x, int y);
	int abs(int op);
	void set_R(int rvalue);
	void set_G(int gvalue);
	void set_B(int bvalue);
	void set_xy(int xp, int yp);
	void FloodFill(int x, int y, int nFillColor, int nSelColor);
	void set_sp(int n);
	int push(int n);
	int pop();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CHairSketchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
