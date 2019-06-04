
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
	ON_COMMAND(ID_DYEING_COLOR, &CHairSketchDoc::OnDyeingColor)
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
	::OpenClipboard(NULL);
	if (!IsClipboardFormatAvailable(CF_DIB)) return FALSE;
	HGLOBAL m_hImage = ::GetClipboardData(CF_DIB);
	::CloseClipboard();

	LPSTR pDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hImage);

	memcpy(&dibHi, pDIB, sizeof(BITMAPINFOHEADER));
	height = dibHi.biHeight;
	width = dibHi.biWidth;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize * height * sizeof(char);
	m_InImg = new unsigned char[dibHi.biSizeImage];
	m_OutImg = new unsigned char[dibHi.biSizeImage];
	m_flag = new unsigned char[dibHi.biSizeImage];

	if (dibHi.biBitCount == 8)
	{
		memcpy(palRGB, pDIB + sizeof(BITMAPINFOHEADER), sizeof(RGBQUAD) * 256);
		memcpy(m_InImg, pDIB + dwBitsSize - dibHi.biSizeImage, dibHi.biSizeImage);
	}
	else memcpy(m_InImg, pDIB + sizeof(BITMAPINFOHEADER), dibHi.biSizeImage);

	dibHf.bfType = 0x4d42;
	dibHf.bfSize = dwBitsSize + sizeof(BITMAPFILEHEADER);
	if (dibHi.biBitCount == 24) dibHf.bfSize -= sizeof(RGBQUAD) * 256;
	dibHf.bfOffBits = dibHf.bfSize - rwsize * height * sizeof(char);
	dibHf.bfReserved1 = dibHf.bfReserved2 = 0;
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
	Image.Save(TEXT("original.bmp"), Gdiplus::ImageFormatBMP);

	CFile hFile;
	hFile.Open(TEXT("original.bmp"), (CFile::modeRead) | (CFile::typeBinary));
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


int CHairSketchDoc::getClickedR(int x, int y)
{
	int j = x, i = height - y;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	int index = i * rwsize;
	return (int)m_InImg[index + j * 3 + 2];
}
int CHairSketchDoc::getClickedG(int x, int y)
{
	int j = x, i = height - y;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	int index = i * rwsize;
	return (int)m_InImg[index + j * 3 + 1];
}
int CHairSketchDoc::getClickedB(int x, int y)
{
	int j = x, i = height - y;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	int index = i * rwsize;
	return (int)m_InImg[index + j * 3 + 0];
}
int CHairSketchDoc::getABS(int num)
{
	int ret;
	if (num >= 0)
		ret = num;
	else
		ret = -num;
	return ret;
}
void CHairSketchDoc::setR(int rvalue)
{
	set_rv = rvalue;
}
void CHairSketchDoc::setG(int gvalue)
{
	set_gv = gvalue;
}
void CHairSketchDoc::setB(int bvalue)
{
	set_bv = bvalue;
}
void CHairSketchDoc::setXY(int xp, int yp)
{ 
	set_x = xp; set_y = height - yp;
}
void CHairSketchDoc::setSP(int n) 
{ 
	sp = n; 
}

void CHairSketchDoc::OnDyeingColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int temp = 0, max = 255, min = 0, count = 1, comp_avg, comp_r, comp_g, comp_b;
	int Vin_R, Vin_G, Vin_B, In_avg, t_avg, average, i, j, index;
	int avg_B = 0, avg_G = 0, avg_R = 0, V_R, V_G, V_B;
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width);
	setSP(0);
	comp_avg = (set_rv + set_gv + set_bv) / 3;
	comp_r = set_rv - comp_avg;
	comp_g = set_gv - comp_avg;
	comp_b = set_bv - comp_avg;

	if (quick == false) {
		CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
		if (dlg.DoModal() == IDOK)
		{
			COLORREF color = dlg.GetColor();

			for (i = 0; i < height; i++) {
				index = i * rwsize;
				for (j = 0; j < width; j++) {
					In_avg = ((int)m_InImg[index + j * 3 + 0] + (int)m_InImg[index + j * 3 + 1] + (int)m_InImg[index + j * 3 + 2]) / 3;
					Vin_R = (int)m_InImg[index + j * 3 + 2] - In_avg;
					Vin_G = (int)m_InImg[index + j * 3 + 1] - In_avg;
					Vin_B = (int)m_InImg[index + j * 3 + 0] - In_avg;
					if (abs(Vin_R - comp_r) <= 12
						&& abs(Vin_G - comp_g) <= 12
						&& abs(Vin_B - comp_b) <= 12
						&& abs(In_avg - comp_avg) <= 100)
					{
						m_flag[index + j * 3] = 1;
						count++;
						temp += In_avg;
					}
					else
					{
						m_flag[index + j * 3] = 0;
					}
				}
			}

			sketchArea(set_x, set_y, 2, 1);

			average = temp / count;
			t_avg = (GetRValue(color) + GetGValue(color) + GetBValue(color)) / 3;
			V_R = GetRValue(color) - t_avg;
			V_G = GetGValue(color) - t_avg;
			V_B = GetBValue(color) - t_avg;

			for (i = 0; i < height; i++) {
				index = i * rwsize;
				for (j = 0; j < width; j++) {
					In_avg = ((int)m_InImg[index + j * 3 + 0] + (int)m_InImg[index + j * 3 + 1] + (int)m_InImg[index + j * 3 + 2]) / 3;

					if (m_flag[index + j * 3] == 2) {
						//B
						temp = t_avg + (In_avg - average) + V_B;
						if (temp > 255) m_OutImg[index + j * 3 + 0] = (unsigned char)max;
						else if (temp < 0)	m_OutImg[index + j * 3 + 0] = (unsigned char)min;
						else m_OutImg[index + j * 3 + 0] = (unsigned char)temp;

						//G
						temp = t_avg + (In_avg - average) + V_G;
						if (temp > 255) m_OutImg[index + j * 3 + 1] = (unsigned char)max;
						else if (temp < 0)	m_OutImg[index + j * 3 + 1] = (unsigned char)min;
						else m_OutImg[index + j * 3 + 1] = (unsigned char)temp;

						//R
						temp = t_avg + (In_avg - average) + V_R;
						if (temp > 255) m_OutImg[index + j * 3 + 2] = (unsigned char)max;
						else if (temp < 0)	m_OutImg[index + j * 3 + 2] = (unsigned char)min;
						else m_OutImg[index + j * 3 + 2] = (unsigned char)temp;
					}
					else
					{
						m_OutImg[index + j * 3 + 0] = m_InImg[index + j * 3 + 0];
						m_OutImg[index + j * 3 + 1] = m_InImg[index + j * 3 + 1];
						m_OutImg[index + j * 3 + 2] = m_InImg[index + j * 3 + 2];
					}
				}
			}
		}
	}
	else {

		for (i = 0; i < height; i++) {
			index = i * rwsize;
			for (j = 0; j < width; j++) {
				In_avg = ((int)m_InImg[index + j * 3 + 0] + (int)m_InImg[index + j * 3 + 1] + (int)m_InImg[index + j * 3 + 2]) / 3;
				Vin_R = (int)m_InImg[index + j * 3 + 2] - In_avg;
				Vin_G = (int)m_InImg[index + j * 3 + 1] - In_avg;
				Vin_B = (int)m_InImg[index + j * 3 + 0] - In_avg;
				if (abs(Vin_R - comp_r) <= 12
					&& abs(Vin_G - comp_g) <= 12
					&& abs(Vin_B - comp_b) <= 12
					&& abs(In_avg - comp_avg) <= 100)
				{
					m_flag[index + j * 3] = 1;
					count++;
					temp += In_avg;
				}
				else
				{
					m_flag[index + j * 3] = 0;
				}
			}
		}

		sketchArea(set_x, set_y, 2, 1);

		average = temp / count;
		t_avg = (GetRValue(quickColor) + GetGValue(quickColor) + GetBValue(quickColor)) / 3;
		V_R = GetRValue(quickColor) - t_avg;
		V_G = GetGValue(quickColor) - t_avg;
		V_B = GetBValue(quickColor) - t_avg;

		for (i = 0; i < height; i++) {
			index = i * rwsize;
			for (j = 0; j < width; j++) {
				In_avg = ((int)m_InImg[index + j * 3 + 0] + (int)m_InImg[index + j * 3 + 1] + (int)m_InImg[index + j * 3 + 2]) / 3;

				if (m_flag[index + j * 3] == 2) {
					//B
					temp = t_avg + (In_avg - average) + V_B;
					if (temp > 255) m_OutImg[index + j * 3 + 0] = (unsigned char)max;
					else if (temp < 0)	m_OutImg[index + j * 3 + 0] = (unsigned char)min;
					else m_OutImg[index + j * 3 + 0] = (unsigned char)temp;

					//G
					temp = t_avg + (In_avg - average) + V_G;
					if (temp > 255) m_OutImg[index + j * 3 + 1] = (unsigned char)max;
					else if (temp < 0)	m_OutImg[index + j * 3 + 1] = (unsigned char)min;
					else m_OutImg[index + j * 3 + 1] = (unsigned char)temp;

					//R
					temp = t_avg + (In_avg - average) + V_R;
					if (temp > 255) m_OutImg[index + j * 3 + 2] = (unsigned char)max;
					else if (temp < 0)	m_OutImg[index + j * 3 + 2] = (unsigned char)min;
					else m_OutImg[index + j * 3 + 2] = (unsigned char)temp;
				}
				else
				{
					m_OutImg[index + j * 3 + 0] = m_InImg[index + j * 3 + 0];
					m_OutImg[index + j * 3 + 1] = m_InImg[index + j * 3 + 1];
					m_OutImg[index + j * 3 + 2] = m_InImg[index + j * 3 + 2];
				}
			}
		}
	}
}

void CHairSketchDoc::sketchArea(int x, int y, int nFillColor, int nSelColor)
{
	int rwsize = WIDTHBYTES(dibHi.biBitCount*width), index;
	index = y * rwsize;
	if (nSelColor < 0)
		nSelColor = m_flag[index + x * 3];
	if (m_flag[index + x * 3] != nSelColor)
		return;
	else
	{
		m_flag[index + x * 3] = nFillColor;
		sketchArea(x - 1, y, nFillColor, nSelColor);
		sketchArea(x + 1, y, nFillColor, nSelColor);
		sketchArea(x, y - 1, nFillColor, nSelColor);
		sketchArea(x, y + 1, nFillColor, nSelColor);
	}
}

void CHairSketchDoc::CopyClipboard(unsigned char *m_CpyImg, int height, int width, int biBitCount)
{
	int rwsize = WIDTHBYTES(biBitCount*width);
	DWORD dwBitsSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + rwsize * height * sizeof(char);

	HGLOBAL m_hImage = (HGLOBAL)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	LPSTR pDIB = (LPSTR)::GlobalLock((HGLOBAL)m_hImage);

	BITMAPINFOHEADER dibCpyHi;
	memcpy(&dibCpyHi, &dibHi, sizeof(BITMAPINFOHEADER));
	dibCpyHi.biBitCount = biBitCount;
	dibCpyHi.biHeight = height;
	dibCpyHi.biWidth = width;
	dibCpyHi.biSizeImage = height * rwsize;
	if (biBitCount == 8) dibCpyHi.biClrUsed = dibCpyHi.biClrImportant = 256;

	memcpy(pDIB, &dibCpyHi, sizeof(BITMAPINFOHEADER));
	if (biBitCount == 8)
	{
		memcpy(pDIB + sizeof(BITMAPINFOHEADER), palRGB, sizeof(RGBQUAD) * 256);
		memcpy(pDIB + dwBitsSize - dibCpyHi.biSizeImage, m_CpyImg, dibCpyHi.biSizeImage);
	}
	else memcpy(pDIB + sizeof(BITMAPINFOHEADER), m_CpyImg, dibCpyHi.biSizeImage);

	::OpenClipboard(NULL);
	::SetClipboardData(CF_DIB, m_hImage);
	::CloseClipboard();

	::GlobalUnlock((HGLOBAL)m_hImage);
	GlobalFree(m_hImage);
}