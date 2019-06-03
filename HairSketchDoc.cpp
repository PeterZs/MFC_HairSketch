
// HairSketchDoc.cpp: CHairSketchDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HairSketch.h"
#endif

#include "HairSketchDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHairSketchDoc

IMPLEMENT_DYNCREATE(CHairSketchDoc, CDocument)

BEGIN_MESSAGE_MAP(CHairSketchDoc, CDocument)
END_MESSAGE_MAP()


// CHairSketchDoc 생성/소멸

CHairSketchDoc::CHairSketchDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_InImg = NULL;
	m_OutImg = NULL;
	m_flag = NULL;
	for (int i = 0; i < 256; i++)
	{
		palRGB[i].rgbBlue = palRGB[i].rgbGreen = palRGB[i].rgbRed = i;
		palRGB[i].rgbReserved = 0;
	}
	set_rv = 0; set_gv = 0; set_bv = 0;
}

CHairSketchDoc::~CHairSketchDoc()
{
	if (m_InImg) delete[]m_InImg;
	if (m_OutImg) delete[]m_OutImg;
	if (m_flag) delete[]m_flag;
}

BOOL CHairSketchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CHairSketchDoc serialization

void CHairSketchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CHairSketchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CHairSketchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHairSketchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHairSketchDoc 진단

#ifdef _DEBUG
void CHairSketchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHairSketchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHairSketchDoc 명령


BOOL CHairSketchDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CImage Image;
	HRESULT hResult = Image.Load(lpszPathName);
	Image.Save(TEXT("test.bmp"), Gdiplus::ImageFormatBMP);

	CFile hFile;
	hFile.Open(TEXT("test.bmp"), (CFile::modeRead) | (CFile::typeBinary));
	hFile.Read(&dibHf, sizeof(BITMAPFILEHEADER));

	// 0x4d42=="BM"
	if (dibHf.bfType != 0x4d42)
	{
		AfxMessageBox(_T("Not BMP File!!"));
		return FALSE;
	}
	hFile.Read(&dibHi, sizeof(BITMAPINFOHEADER));
	if ((dibHi.biBitCount != 8) && (dibHi.biBitCount != 24))
	{
		AfxMessageBox(_T("Gray/True Color Possible!!"));
		return FALSE;
	}
	if (dibHi.biBitCount == 8)
	{
		hFile.Read(palRGB, sizeof(RGBQUAD) * 256);
	}

	//int ImgSize;
	if (dibHi.biBitCount == 8)
		ImgSize = (int)hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
	else if (dibHi.biBitCount == 24)
		ImgSize = (int)hFile.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	m_InImg = new unsigned char[ImgSize];
	m_OutImg = new unsigned char[ImgSize];
	m_flag = new unsigned char[ImgSize];
	hFile.Read(m_InImg, ImgSize);
	hFile.Close();
	height = dibHi.biHeight;
	width = dibHi.biWidth;

	if (dibHi.biBitCount == 24) return TRUE;

	// Gray
	int i, j, index;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	for (i = 0; i < height; i++)
	{
		index = i * rwsize;
		for (j = 0; j < width; j++)
			m_InImg[index + j] = (unsigned char)palRGB[(int)m_InImg[index + j]].rgbBlue;
	}
	return TRUE;
}


BOOL CHairSketchDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile hFile;
	if (!hFile.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) return FALSE;
	hFile.Write(&dibHf, sizeof(BITMAPFILEHEADER));
	hFile.Write(&dibHi, sizeof(BITMAPINFOHEADER));
	if (dibHi.biBitCount == 8) hFile.Write(palRGB, sizeof(RGBQUAD) * 256);
	hFile.Write(m_InImg, dibHi.biSizeImage);
	hFile.Close();
	return CDocument::OnSaveDocument(lpszPathName);
}
