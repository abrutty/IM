#pragma once

#include <atlimage.h>
//过渡性设计，可以方便业务代码使用
class CImageEx : public CImage
{
public:
	CImageEx(void);
	virtual ~CImageEx(void);

public:
	BOOL LoadFromFile(LPCTSTR pszFileName);
	BOOL LoadFromIStream(IStream* pStream);
	BOOL LoadFromBuffer(const BYTE* lpBuf, DWORD dwSize);
	BOOL LoadFromResource(HINSTANCE hInstance, LPCTSTR pszResourceName, LPCTSTR pszResType);
	BOOL LoadFromResource(HINSTANCE hInstance, UINT nIDResource, LPCTSTR pszResType);

	void SetNinePart(const RECT* lpNinePart);
	BOOL Draw2(HDC hDestDC, const RECT& rectDest);

	void GrayScale();	// 图像灰度化

private:
	BOOL AlphaPremultiplication();	// Alpha预乘
	BOOL DrawNinePartImage(int pleft, int ptop, int pright, int pbottom,
		HDC hDC, int height, int width, int left, int top, int right, int bottom);
	BOOL DrawNinePartImage(HDC hDC, int x, int y, int cx, int cy, 
		int nLeft, int nTop, int nRight, int nBottom);
	int GetFileType(LPCTSTR lpszFileName);

private:
	BOOL m_bIsNinePart;
	RECT m_rcNinePart;
};
